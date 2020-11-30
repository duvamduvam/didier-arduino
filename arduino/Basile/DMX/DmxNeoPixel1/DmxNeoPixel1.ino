#include <DMXSerial.h>
#include <Adafruit_NeoPixel.h>

//------------------------------------------------------------------------------------------------------------
//                                                DMX_NEO_PIXEL
//------------------------------------------------------------------------------------------------------------
/*
 * Virtualiseur de projecteurs DMX
 * 
 * Le mapping peut se reprogrammer en DMX...
 * DIPSWITCH -> Adresse de configuration (ADD_CONFIG)
 * Si ADD_CONFIG=1...
 * ADD_CONFIG+1 : Nombre de projecteurs
 * ADD_CONFIG+2 : Largeur de projecteurs 
 * ADD_CONFIG+3 : Adresse DMX du premier projecteur (chaque projecteur à besoin de 3 cannaux de couleur
 * ADD_CONFIG+4 : Offset d'adresse DMX projecteurs (saut d'adresse, permet de s'adapter à un patch de projecteurs réels utilisant 4 cannaux par exemple)
 * 
 * Le programme duplique les projecteurs jusqu'a la fin du bandeau
 */

//------------------------------------------------------------------------------------------------------------
//                                                  HARDWARE
//------------------------------------------------------------------------------------------------------------
#define NB_PROJECTORS_MAX   20
#define NB_LEDS        120
#define PIN_NEOPIXEL    3


#define PIN_SW0        8
#define PIN_SW1        9
#define PIN_SW2        10
#define PIN_SW3        11
#define PIN_SW4        12
#define PIN_SW5        13
#define PIN_SW6        2
#define PIN_SW7        4
#define PIN_SW8        7


//------------------------------------------------------------------------------------------------------------
//                                               STRUCTURES
//------------------------------------------------------------------------------------------------------------
struct TColor
{
  byte r;
  byte g;
  byte b;
};

typedef struct VirtualProjector
{
  int dmxStartChannel;
  int location;
  int width;
  TColor color;
}VirtualProjector;

VirtualProjector Projectors[NB_PROJECTORS_MAX];
//------------------------------------------------------------------------------------------------------------
//                                                 GLOBAL VARS
//------------------------------------------------------------------------------------------------------------
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NB_LEDS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

int nbProjectors;

//------------------------------------------------------------------------------------------------------------
//                                                  SETUP
//------------------------------------------------------------------------------------------------------------

void setup () 
{
  DMXSerial.init(DMXReceiver);
 
  pinMode(PIN_SW0, INPUT_PULLUP);
  pinMode(PIN_SW1, INPUT_PULLUP);
  pinMode(PIN_SW2, INPUT_PULLUP);
  pinMode(PIN_SW3, INPUT_PULLUP);
  pinMode(PIN_SW4, INPUT_PULLUP);
  pinMode(PIN_SW5, INPUT_PULLUP);
  pinMode(PIN_SW6, INPUT_PULLUP);
  pinMode(PIN_SW7, INPUT_PULLUP);
  pinMode(PIN_SW8, INPUT_PULLUP);


  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  MapProjectors(4, 30, 2, 4);

}

//------------------------------------------------------------------------------------------------------------
//                                               PATCH DMX
//------------------------------------------------------------------------------------------------------------
int dmxAddress;
void ReadAddress()
{
  int a0 = 1-digitalRead(PIN_SW0);
  int a1 = 1-digitalRead(PIN_SW1);
  int a2 = 1-digitalRead(PIN_SW2);
  int a3 = 1-digitalRead(PIN_SW3);
  int a4 = 1-digitalRead(PIN_SW4);
  int a5 = 1-digitalRead(PIN_SW5);
  int a6 = 1-digitalRead(PIN_SW6);
  int a7 = 1-digitalRead(PIN_SW7);
  int a8 = 1-digitalRead(PIN_SW8);

  dmxAddress = (a8<<8)+(a7<<7)+(a6<<6)+(a5<<5)+(a4<<4)+(a3<<3)+(a2<<2)+(a1<<1)+a0;
}

//------------------------------------------------------------------------------------------------------------
//                                              PROJECTORS
//------------------------------------------------------------------------------------------------------------
void CreateProjector(int projectorIndex, int startChannel, int location, int width)
{
  int i = projectorIndex % NB_PROJECTORS_MAX;
  Projectors[i].dmxStartChannel = startChannel;
  Projectors[i].location = location;
  Projectors[i].width = width;
}


void MapProjectors(int nb, int width, int DmxstartAddress, int DmxOffset)
{
  for (int i=0;i<nb;i++)
  {
      CreateProjector(i, DmxstartAddress + DmxOffset*i, i*width, width);
  }
  nbProjectors = nb;
}

//------------------------------------------------------------------------------------------------------------
//                                               NEO PIXEL
//------------------------------------------------------------------------------------------------------------
void SetNeoPixelColor(int location, TColor color)
{
  uint32_t c = strip.Color(color.r,color.g,color.b);
  strip.setPixelColor(location, c);
}

void PaintNeoPixel()
{
  int pos=0;
  int nextLocation=0;
  for (int k=0;pos<NB_LEDS;k++)
  {
    for (int i=0;i<nbProjectors;i++)
    {
      VirtualProjector p = Projectors[i%NB_PROJECTORS_MAX];
      for (int j=0;j<p.width;j++)
      {
        if (nextLocation+p.location+j < NB_LEDS)
          SetNeoPixelColor(nextLocation+p.location+j, p.color);
        pos++;
      }
    }
    nextLocation = pos;
  }
  strip.show();
}

//------------------------------------------------------------------------------------------------------------
//                                                  COLOR 
//------------------------------------------------------------------------------------------------------------
TColor NewColor(byte r, byte g, byte b)
{

  TColor result;
  result.r = r;
  result.g = g;
  result.b = b;
  return result;
}

//------------------------------------------------------------------------------------------------------------
//                                                  DMX
//------------------------------------------------------------------------------------------------------------
void ParseDMX()
{

  byte confAddress = DMXSerial.read(dmxAddress);
  if (confAddress == 1)
  {
    byte nb = (DMXSerial.read(dmxAddress+1)) % NB_PROJECTORS_MAX;
    byte width = DMXSerial.read(dmxAddress+2);
    byte startAddress = DMXSerial.read(dmxAddress+3);
    byte offset = DMXSerial.read(dmxAddress+4);

    MapProjectors(nb,width,startAddress,offset);
  }


  for (int i=0;i<nbProjectors;i++)
  {
    VirtualProjector *p = &Projectors[i%NB_PROJECTORS_MAX];
    byte red = DMXSerial.read(p->dmxStartChannel);
    byte green = DMXSerial.read(p->dmxStartChannel+1);
    byte blue = DMXSerial.read(p->dmxStartChannel+2);
    p->color =  NewColor(red, green, blue);
  }
}
//------------------------------------------------------------------------------------------------------------
//                                                 LOOP
//------------------------------------------------------------------------------------------------------------
void loop() 
{
  ReadAddress();

  unsigned long lastPacket = DMXSerial.noDataSince();
  if (lastPacket < 5000) 
  {
    ParseDMX();
    PaintNeoPixel();
    delay(20);
  } 
}


