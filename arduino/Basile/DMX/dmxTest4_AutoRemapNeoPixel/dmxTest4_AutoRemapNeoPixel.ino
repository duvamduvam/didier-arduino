#include <DmxSimple.h>
//------------------------------------------------------------------------------------------------------------
//                                                  DMX_TEST
//------------------------------------------------------------------------------------------------------------
/*
 * Testeur d'effets, les 2 premiers BP changent la palette de couleur, les 2 suivants changent l'effet
 * ->Executeur de séquences
 * ->Executeur de sous séquences (effets progressifs)
 * ->Transformations couleurs: Color Weel, DimColor, LightColor
 * ->Générateur d'aléatoire
 * ->Lecture de paterns
 * ->AutoRemap NeoPixel (control DMX)
 */
 
//------------------------------------------------------------------------------------------------------------
//                                                  PROGRAM
//------------------------------------------------------------------------------------------------------------
#define DMX_NB_CHANNELS     30
#define NB_PALETTES       5
#define NB_EFFETS         10
#define DIM_NB_STEPS      20

//Projectors
#define NB_SINGLE_DMX     4
#define NB_COLOR_DMX      4
#define NB_CONTEST_DMX    4

//NeoPixel
#define DMX_NEOPIXEL_CONFIG_ADDRESS     20


//------------------------------------------------------------------------------------------------------------
//                                                  HARWARE
//------------------------------------------------------------------------------------------------------------
#define DMX_PIN_OUT         2
#define ANALOG_PIN_RYTHME   A0

#define BP_MATRICE_ROW_0    8
#define BP_MATRICE_ROW_1    9
#define BP_MATRICE_COL_0    10
#define BP_MATRICE_COL_1    11

//------------------------------------------------------------------------------------------------------------
//                                               STRUCTURES
//------------------------------------------------------------------------------------------------------------
struct TColor
{
  byte r;
  byte g;
  byte b;
};

typedef struct DmxColorProjector
{
  int rChannel;
  int gChannel;
  int bChannel;
}DmxColorProjector;

typedef struct DmxContestProjector
{
  int strobeChannel;
  int tiltChannel;
  int panChannel;
  int goboChannel;
}DmxContestProjector;

typedef struct ColorPalette
{
  TColor Colors[8];
  int nbColors;
}ColorPalette;

typedef struct Effet
{
  int nbSteps;
  int stepIndex;
}Effet;

//------------------------------------------------------------------------------------------------------------
//                                                  GLOBAL VARS
//------------------------------------------------------------------------------------------------------------

 
//RYTHME
uint32_t rythmeTimer;
uint32_t dimTimer;
int dimIndex;

//KEYBOARD
uint32_t keyTimer;
int lastKey;

//PROJECTORS
DmxColorProjector ColorProjectors[NB_COLOR_DMX];
DmxContestProjector ContestProjectors[NB_CONTEST_DMX];
byte SingleProjectors[NB_SINGLE_DMX];

//PAINT
ColorPalette Palettes[NB_PALETTES];
int paletteIndex;

//Effets
Effet Effets[NB_EFFETS];
int effetIndex;

//NeoPixel
int remapCounter;
int remapSequence;

//------------------------------------------------------------------------------------------------------------
//                                                  SETUP
//------------------------------------------------------------------------------------------------------------


void setup() 
{
  DmxSimple.usePin(DMX_PIN_OUT);
  DmxSimple.maxChannel(DMX_NB_CHANNELS);

  //Init Keyboard
  pinMode(BP_MATRICE_ROW_0, OUTPUT);
  pinMode(BP_MATRICE_ROW_1, OUTPUT);
  pinMode(BP_MATRICE_COL_0, INPUT_PULLUP);
  pinMode(BP_MATRICE_COL_1, INPUT_PULLUP);

  //Paint
  InitPalettes();

  //Patch DMX
  InitProjectors();

  //Effets
  InitEffets();
}

//------------------------------------------------------------------------------------------------------------
//                                               PATCH DMX
//------------------------------------------------------------------------------------------------------------

void InitProjectors()
{
  ColorProjectors[0] = NewProjector(2,3, 4);
  ColorProjectors[1] = NewProjector(6,7, 8);
  ColorProjectors[2] = NewProjector(10,11, 12);
  ColorProjectors[3] = NewProjector(14,15, 16);

  SingleProjectors[0] = 20;
  SingleProjectors[1] = 21;
  SingleProjectors[2] = 22;
  SingleProjectors[3] = 23;
}


//------------------------------------------------------------------------------------------------------------
//                                              COLOR PALETTE
//------------------------------------------------------------------------------------------------------------


void InitPalettes()
{
 
  Palettes[0].nbColors = 6;
  Palettes[0].Colors[0] = NewColor(255, 0, 0); //Red
  Palettes[0].Colors[1] = NewColor(255, 255, 0); //Yellow
  Palettes[0].Colors[2] = NewColor(0, 255, 0); //Green
  Palettes[0].Colors[3] = NewColor(0, 255, 255); //Cyan
  Palettes[0].Colors[4] = NewColor(0, 0, 255); //Blue
  Palettes[0].Colors[5] = NewColor(255, 0, 255); //Magenta

  Palettes[1].nbColors = 6;
  Palettes[1].Colors[5] = NewColor(255, 0, 0); //Red
  Palettes[1].Colors[4] = NewColor(255, 255, 0); //Yellow
  Palettes[1].Colors[3] = NewColor(0, 255, 0); //Green
  Palettes[1].Colors[2] = NewColor(0, 255, 255); //Cyan
  Palettes[1].Colors[1] = NewColor(0, 0, 255); //Blue
  Palettes[1].Colors[0] = NewColor(255, 0, 255); //Magenta

  Palettes[2].nbColors = 2;
  Palettes[2].Colors[0] = NewColor(255, 0, 0); //Red
  Palettes[2].Colors[1] = NewColor(0, 0, 255); //Blue

  Palettes[3].nbColors = 4;
  Palettes[3].Colors[0] = NewColor(255, 255, 255); //White
  Palettes[3].Colors[1] = NewColor(0, 0, 255); //Blue
  Palettes[3].Colors[2] = NewColor(255, 0, 255); //Magenta
  Palettes[3].Colors[3] = NewColor(255, 255, 255); //White

  Palettes[4].nbColors = 3;
  Palettes[4].Colors[0] = NewColor(255, 0, 0); //Red
  Palettes[4].Colors[1] = NewColor(255, 255, 0); //Yellow
  Palettes[4].Colors[2] = NewColor(0, 255, 0); //Green

}

TColor GetColorFromPalette(int palette, int colorIndex)
{
  int colorModulo = colorIndex % Palettes[palette].nbColors;
  return Palettes[palette % NB_PALETTES].Colors[colorModulo];
}

TColor GetColorFromCurrentPalette(int colorIndex)
{
  return GetColorFromPalette(paletteIndex, colorIndex);
}

//------------------------------------------------------------------------------------------------------------
//                                               PROJECTORS
//------------------------------------------------------------------------------------------------------------

DmxColorProjector NewProjector(byte rC, byte gC, byte bC)
{

  DmxColorProjector result;
  result.rChannel = rC;
  result.gChannel = gC;
  result.bChannel = bC;
  return result;
}


//------------------------------------------------------------------------------------------------------------
//                                                  DMX
//------------------------------------------------------------------------------------------------------------


void DMX_Clear()
{
  for (int i=0;i<DMX_NB_CHANNELS;i++)
    DmxSimple.write(i+1, 0);
}

void ColorProjectorSet(int projectorIndex, TColor color)
{
  DmxSimple.write(ColorProjectors[projectorIndex].rChannel, color.r);
  DmxSimple.write(ColorProjectors[projectorIndex].gChannel, color.g);
  DmxSimple.write(ColorProjectors[projectorIndex].bChannel, color.b);
}

void ColorProjectorSetPalette(int projectorIndex, int colorIndex)
{
  TColor color = GetColorFromCurrentPalette(colorIndex);
  ColorProjectorSet(projectorIndex,color);
}


void ColorProjectorOff(int projectorIndex)
{
  ColorProjectorSet(projectorIndex, NewColor(0, 0, 0));
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
//                                              COLOR TRANSFORM
//------------------------------------------------------------------------------------------------------------

TColor Wheel(byte WheelPos)
{
  WheelPos = WheelPos % 255;
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return NewColor(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return NewColor(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return NewColor(WheelPos * 3, 255 - WheelPos * 3, 0);
}



TColor DimColor(TColor Base, int Dimmer)
{
  int RColor;
  int GColor;
  int BColor;

  if (Base.r > Dimmer)
    RColor = Base.r - Dimmer;
  else
    RColor = 0;

  if (Base.g > Dimmer)
    GColor = Base.g - Dimmer;
  else
    GColor = 0;

  if (Base.b > Dimmer)
    BColor = Base.b - Dimmer;
  else
     BColor = 0;

  return NewColor(RColor, GColor, BColor);
}

TColor LightColor(TColor Base, int Light)
{
  int RColor;
  int GColor;
  int BColor;

  if (Base.r + Light < 0xFF)
    RColor = Base.r + Light;
  else
    RColor = 0xFF;

  if (Base.g + Light < 0xFF)
    GColor = Base.g + Light;
  else
    GColor = 0xFF;

  if (Base.b + Light < 0xFF)
    BColor = Base.b + Light;
  else
    BColor = 0xFF;

  return NewColor(RColor, GColor, BColor);
}


//------------------------------------------------------------------------------------------------------------
//                                                  RANDOM
//------------------------------------------------------------------------------------------------------------

unsigned int Rnd;
void GenerateRandom()
{

  char a = (Rnd & 0x8000)>>15;
  char b = (Rnd & 0x4000)>>14;
  char c = (Rnd & 0x1000)>>12;
  char d = (Rnd & 0x0080)>>3;

  char e = !(a^b);
  char f = !(c^d);
  char g = !(e^f);

  Rnd =( Rnd<<1) + g;
}

unsigned int GetRandom(unsigned int modulo)
{
  GenerateRandom();
  return Rnd % modulo;
}


//------------------------------------------------------------------------------------------------------------
//                                                  TIMER
//------------------------------------------------------------------------------------------------------------
uint32_t Chrono(uint32_t *timer)
{
  uint32_t now = millis();
  uint32_t dif = now-*timer;
  return dif;
}


//------------------------------------------------------------------------------------------------------------
//                                                 KEYBOARD MATRIX
//------------------------------------------------------------------------------------------------------------
void KeyboardMatrixOut(int val)
{
  digitalWrite(BP_MATRICE_ROW_0, HIGH); 
  digitalWrite(BP_MATRICE_ROW_1, HIGH);
  switch(val)
  {
    case 0:break;
    case 1:digitalWrite(BP_MATRICE_ROW_0, LOW);break;
    case 2:digitalWrite(BP_MATRICE_ROW_1, LOW);break;
  }
}

int KeyboardMatrixIn()
{
  if(digitalRead(BP_MATRICE_COL_0)==0)return 1;
  if(digitalRead(BP_MATRICE_COL_1)==0)return 2;
  return 0;
}

int ReadKeyboard()
{
  //Row1
  KeyboardMatrixOut(1);
  switch(KeyboardMatrixIn())
  {
    case 1:KeyboardMatrixOut(0);return 1;break;
    case 2:KeyboardMatrixOut(0);return 2;break;
  }
  //Row1
  KeyboardMatrixOut(2);
  switch(KeyboardMatrixIn())
  {
    case 1:KeyboardMatrixOut(0);return 3;break;
    case 2:KeyboardMatrixOut(0);return 4;break;
  }
  //Else
  KeyboardMatrixOut(0);return 0;
  return 0;
}

void ExecuteKeyboard()
{
  //Aquisition clavier & antirebond
  if (Chrono(&keyTimer)>20)
  {
    keyTimer=millis(); //Lis le chrono
    int key = ReadKeyboard();
    if (key!=lastKey)
    {
      switch (key)
      {
        case 0:break;

        case 1:
          if (paletteIndex>0)
            paletteIndex--;
            break;

        case 2:
          if (paletteIndex<NB_PALETTES-1)
            paletteIndex++;
            break;

        case 3:
          if (effetIndex>0)
            effetIndex--;
            break;

        case 4:
          if (effetIndex<NB_EFFETS-1)
            effetIndex++;
            break;
      }
    }
    lastKey = key;
  }

}

//------------------------------------------------------------------------------------------------------------
//                                                 EFFETS
//------------------------------------------------------------------------------------------------------------

void InitEffets()
{
  Effets[0].nbSteps = 4;
  Effets[1].nbSteps = 4;
  Effets[2].nbSteps = 4;
  Effets[3].nbSteps = 4;
  Effets[4].nbSteps = 0;
  Effets[5].nbSteps = 0;
  Effets[6].nbSteps = 0;
  Effets[7].nbSteps = 4;
  Effets[8].nbSteps = 4;
  Effets[9].nbSteps = 13;
}

void StepEffet(Effet *e)
{
  e->stepIndex++;
  if (e->stepIndex >= e->nbSteps)
    e->stepIndex = 0;
}

//Chaser Right
void ExecuteEffet_0()
{
  DMX_Clear();
  switch (Effets[0].stepIndex)
  {
    case 0:ColorProjectorSetPalette(3, 0);break;
    case 1:ColorProjectorSetPalette(2, 1);break;
    case 2:ColorProjectorSetPalette(1, 2);break;
    case 3:ColorProjectorSetPalette(0, 3);break;
  }
  StepEffet(&Effets[0]);
}

//Chaser Left
void ExecuteEffet_1()
{
  DMX_Clear();
  switch (Effets[1].stepIndex)
  {
    case 0:ColorProjectorSetPalette(0, 0);break;
    case 1:ColorProjectorSetPalette(1, 1);break;
    case 2:ColorProjectorSetPalette(2, 2);break;
    case 3:ColorProjectorSetPalette(3, 3);break;
  }
  StepEffet(&Effets[1]);
}

//Scroll Right Bicolor
void ExecuteEffet_2()
{
  DMX_Clear();
  switch (Effets[2].stepIndex)
  {
    case 0:
      ColorProjectorSetPalette(0, 1);
      ColorProjectorSetPalette(1, 1);
      ColorProjectorSetPalette(2, 0);
      ColorProjectorSetPalette(3, 0);
      break;
      
    case 1:
      ColorProjectorSetPalette(0, 0);
      ColorProjectorSetPalette(1, 1);
      ColorProjectorSetPalette(2, 1);
      ColorProjectorSetPalette(3, 0);
      break;

    case 2:
      ColorProjectorSetPalette(0, 0);
      ColorProjectorSetPalette(1, 0);
      ColorProjectorSetPalette(2, 1);
      ColorProjectorSetPalette(3, 1);
      break;

    case 3:
      ColorProjectorSetPalette(0, 1);
      ColorProjectorSetPalette(1, 0);
      ColorProjectorSetPalette(2, 0);
      ColorProjectorSetPalette(3, 1);
      break;
  }
  StepEffet(&Effets[2]);
}

//Color Weel
void ExecuteEffet_3()
{
  StepEffet(&Effets[3]);
}

//Color Weel
void DimEffet_3()
{
  float dimRatio = (float)dimIndex/DIM_NB_STEPS;
  float stepRatio = (dimRatio + Effets[3].stepIndex) / Effets[3].nbSteps;
  float p1Weel = stepRatio * 255;
  float p2Weel = (stepRatio + 0.25) * 255;
  float p3Weel = (stepRatio + 0.5) * 255;
  float p4Weel = (stepRatio + 0.75) * 255;

  TColor c1 = Wheel(p1Weel);
  TColor c2 = Wheel(p2Weel);
  TColor c3 = Wheel(p3Weel);
  TColor c4 = Wheel(p4Weel);
  

  ColorProjectorSet(0, c1);
  ColorProjectorSet(1, c2);
  ColorProjectorSet(2, c3);
  ColorProjectorSet(3, c4);

  
}

//Random Palette
void ExecuteEffet_4()
{
  int modulo = Palettes[paletteIndex].nbColors;
  int paletteColor = GetRandom(modulo);
  int projectorIndex = GetRandom(NB_COLOR_DMX);
  
  ColorProjectorSetPalette(projectorIndex, paletteColor);
}

//Random Palette Flash
void ExecuteEffet_5()
{
  DMX_Clear();
  ExecuteEffet_6();
}

//Random Color Flash
void ExecuteEffet_6()
{
  int modulo = Palettes[paletteIndex].nbColors;
  int ColorWheel = GetRandom(255);
  int projectorIndex = GetRandom(NB_COLOR_DMX);
  TColor c = Wheel(ColorWheel);
  ColorProjectorSet(projectorIndex, c);
}


//Color Dim
void ExecuteEffet_7()
{
  DMX_Clear();
  StepEffet(&Effets[7]);
}

void DimEffet_7()
{
  float dimRatio = abs(1.0 - (float)dimIndex*2/DIM_NB_STEPS);
  TColor c = GetColorFromCurrentPalette(0);
  c = DimColor(c,dimRatio*255);
  
  switch (Effets[7].stepIndex)
  {
    case 0:ColorProjectorSet(0, c);break;
    case 1:ColorProjectorSet(1, c);break;
    case 2:ColorProjectorSet(2, c);break;
    case 3:ColorProjectorSet(3, c);break;
  }
  
  
}

//White Flash
void ExecuteEffet_8()
{
  StepEffet(&Effets[8]);
}

void DimEffet_8()
{
  float lightRatio = 1.0-abs(1.0 - (float)dimIndex*2/DIM_NB_STEPS);
  TColor c = GetColorFromCurrentPalette(1);
  TColor lighted = LightColor(c,lightRatio*255);
  
  ColorProjectorSetPalette(0, 0);
  ColorProjectorSetPalette(1, 1);
  ColorProjectorSetPalette(2, 2);
  ColorProjectorSetPalette(3, 3);

  switch (Effets[8].stepIndex)
  {
    case 0:ColorProjectorSet(0, lighted);break;
    case 1:ColorProjectorSet(1, lighted);break;
    case 2:ColorProjectorSet(2, lighted);break;
    case 3:ColorProjectorSet(3, lighted);break;
  }
  
  
}


//Execute Patern
int patern_9[] = {0x01,0x02,0x04,0x08,0x04,0x02,0x01,0x06,0x09,0x0A,0x05,0x02,0x08 };
void ExecuteEffet_9()
{
  DMX_Clear();
  int modulo = Effets[9].stepIndex % Effets[9].nbSteps;
  int patern = patern_9[modulo];

  for (int i=0;i<4;i++)
  {
    if (patern & 1)
      ColorProjectorSetPalette(i, Effets[9].stepIndex);
    patern=patern>>1;
  }
  
  StepEffet(&Effets[9]);
}

//------------------------------------------------------------------------------------------------------------
//                                                  Rythme
//------------------------------------------------------------------------------------------------------------

void ExecuteRythme()
{
  int rythmeDelay = map(analogRead(ANALOG_PIN_RYTHME),0,1023,1000,10);
  if (Chrono(&rythmeTimer)>rythmeDelay)
  {
    rythmeTimer=millis(); //Lis le chrono

    switch (effetIndex)
    {
      case 0:ExecuteEffet_0();break;
      case 1:ExecuteEffet_1();break;
      case 2:ExecuteEffet_2();break;
      case 3:ExecuteEffet_3();break;
      case 4:ExecuteEffet_4();break;
      case 5:ExecuteEffet_5();break;
      case 6:ExecuteEffet_6();break;
      case 7:ExecuteEffet_7();break;
      case 8:ExecuteEffet_8();break;
      case 9:ExecuteEffet_9();break;
    }

    dimIndex=0;


    remapCounter++;
    if (remapCounter>8)
    {
      ExecuteRemapNeoPixel();
      remapCounter=0;
    }
  }

  int dimDelay = rythmeDelay/DIM_NB_STEPS;
  if (Chrono(&dimTimer)>dimDelay)
  {
    dimTimer=millis(); //Lis le chrono
    dimIndex++;

    switch (effetIndex)
    {
      case 3:DimEffet_3();break;
      case 7:DimEffet_7();break;
      case 8:DimEffet_8();break;
    }

  }
}

//------------------------------------------------------------------------------------------------------------
//                                             NEO PIXEL CONTROL
//------------------------------------------------------------------------------------------------------------

void RemapNeoPixel(byte nb, byte width, byte DmxstartAddress, byte DmxOffset)
{
  DmxSimple.write(DMX_NEOPIXEL_CONFIG_ADDRESS, 1); //Enter Config
  DmxSimple.write(DMX_NEOPIXEL_CONFIG_ADDRESS+1, nb);
  DmxSimple.write(DMX_NEOPIXEL_CONFIG_ADDRESS+2, width);
  DmxSimple.write(DMX_NEOPIXEL_CONFIG_ADDRESS+3, DmxstartAddress);
  DmxSimple.write(DMX_NEOPIXEL_CONFIG_ADDRESS+4, DmxOffset);
}

void ExecuteRemapNeoPixel()
{
  switch (remapSequence)
  {
    case 0:RemapNeoPixel(4, 30, 2, 4);break;
    case 1:RemapNeoPixel(4, 20, 2, 4);break;
    case 2:RemapNeoPixel(4, 10, 2, 4);break;
    case 3:RemapNeoPixel(4, 5, 2, 4);break;
    case 4:RemapNeoPixel(4, 2, 2, 4);break;
    case 5:RemapNeoPixel(4, 1, 2, 4);break;
  }

  
  remapSequence++;
  if (remapSequence>6)
    remapSequence = 0;
}

//------------------------------------------------------------------------------------------------------------
//                                                  LOOP
//------------------------------------------------------------------------------------------------------------

void loop() 
{
  GenerateRandom();
  ExecuteRythme();
  ExecuteKeyboard();
}


