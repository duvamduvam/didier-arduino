#include <DmxSimple.h>
//------------------------------------------------------------------------------------------------------------
//                                                  DMX_TEST
//------------------------------------------------------------------------------------------------------------
/*
 * Testeur de cannaux DMX, les 4bp permettent de choisir la couleur du premier projecteur
 * ->Exemple clavier matricé
 * ->Exemple structures
 * ->Patch DMX
 * ->Palette de couleurs
 * 
 * Le Par36 LED utilisé pour les tests comporte 4 cannaux, le premier ne sert à rien(mode auto), puis cannaux R,G,B
 * 
 */
 
#define DMX_NB_CHANNELS_TEST   12
int channelIndex;


//------------------------------------------------------------------------------------------------------------
//                                                  PROGRAM
//------------------------------------------------------------------------------------------------------------
#define NB_PALETTES 3
#define NB_PROGRAMS 3



//------------------------------------------------------------------------------------------------------------
//                                                  HARWARE
//------------------------------------------------------------------------------------------------------------
#define DMX_NB_CHANNELS     20
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


//------------------------------------------------------------------------------------------------------------
//                                                  GLOBAL VARS
//------------------------------------------------------------------------------------------------------------

//RYTHME
uint32_t rythmeTimer;

//KEYBOARD
uint32_t keyTimer;
int lastKey;

//PAINT
int paletteIndex;
int programIndex;

//PROJECTORS
DmxColorProjector ColorProjectors[10];
byte SingleProjectors[10];



//------------------------------------------------------------------------------------------------------------
//                                                  PATCH DMX
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

void ColorProjectorOff(int projectorIndex)
{
  ColorProjectorSet(projectorIndex, NewColor(0, 0, 0));
}

//------------------------------------------------------------------------------------------------------------
//                                              COLOR PALETTE
//------------------------------------------------------------------------------------------------------------


TColor Palette[8][8];

TColor NewColor(byte r, byte g, byte b)
{

  TColor result;
  result.r = r;
  result.g = g;
  result.b = b;
  return result;
}

#define PAL0_COLOR_BLACK    0
#define PAL0_COLOR_RED      1
#define PAL0_COLOR_YELLOW   2
#define PAL0_COLOR_GREEN    3
#define PAL0_COLOR_CYAN     4
#define PAL0_COLOR_BLUE     5
#define PAL0_COLOR_MAGENTA  6
#define PAL0_COLOR_WHITE    7

#define PAL1_CHAKRA_BLACK   0
#define PAL1_CHAKRA_RED     1
#define PAL1_CHAKRA_ORANGE  2
#define PAL1_CHAKRA_YELLOW  3
#define PAL1_CHAKRA_GREEN   4
#define PAL1_CHAKRA_BLUE    5
#define PAL1_CHAKRA_INDIGO  6
#define PAL1_CHAKRA_PURPLE  7
#define PAL1_CHAKRA_WHITE   8

void InitPalettes()
{
  //STD
  Palette[0][PAL0_COLOR_BLACK]  = NewColor(0, 0, 0);
  Palette[0][PAL0_COLOR_RED]    = NewColor(255, 0, 0);
  Palette[0][PAL0_COLOR_YELLOW] = NewColor(255, 255, 0);
  Palette[0][PAL0_COLOR_GREEN]  = NewColor(0, 255, 0);
  Palette[0][PAL0_COLOR_CYAN]   = NewColor(0, 255, 255);
  Palette[0][PAL0_COLOR_BLUE]   = NewColor(0, 0, 255);
  Palette[0][PAL0_COLOR_MAGENTA]= NewColor(255, 0, 255);
  Palette[0][PAL0_COLOR_WHITE]  = NewColor(255, 255, 255);

  //CHAKRAS
  Palette[1][PAL1_CHAKRA_BLACK]  = NewColor(0, 0, 0);
  Palette[1][PAL1_CHAKRA_RED]    = NewColor(0xFF, 0,0);
  Palette[1][PAL1_CHAKRA_ORANGE] = NewColor(0x61, 0x1E, 0);
  Palette[1][PAL1_CHAKRA_YELLOW] = NewColor(0x8C, 0xC8, 0);
  Palette[1][PAL1_CHAKRA_GREEN]  = NewColor(0, 0xFF, 0);
  Palette[1][PAL1_CHAKRA_BLUE]   = NewColor(0, 0x0A, 0xDC);
  Palette[1][PAL1_CHAKRA_INDIGO] = NewColor(0x14, 0, 0x64);
  Palette[1][PAL1_CHAKRA_PURPLE] = NewColor(0xF0, 0, 0xB4);
  Palette[1][PAL1_CHAKRA_WHITE]  = NewColor(0xFF, 0xFF, 0xFF);
 
}


//------------------------------------------------------------------------------------------------------------
//                                              COLOR TRANSFORM
//------------------------------------------------------------------------------------------------------------

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
}



//------------------------------------------------------------------------------------------------------------
//                                                  LOOP
//------------------------------------------------------------------------------------------------------------
void ExecuteChaser()
{
  DMX_Clear();
    DmxSimple.write(channelIndex+1, 255);
    channelIndex++;
    if (channelIndex>DMX_NB_CHANNELS_TEST)
      channelIndex=0;
}

void ExecuteRythme()
{
  int rythmeDelay = map(analogRead(ANALOG_PIN_RYTHME),0,1023,1000,100);
  if (Chrono(&rythmeTimer)>rythmeDelay)
  {
    rythmeTimer=millis(); //Lis le chrono

    ExecuteChaser();
  }
}


void loop() 
{

  //ExecuteRythme();

  //Aquisition clavier & antirebond
  if (Chrono(&keyTimer)>20)
  {
    keyTimer=millis(); //Lis le chrono
    int key = ReadKeyboard();
    if (key!=lastKey)
    {
      switch (key)
      {
        case 0:DMX_Clear();break;
        case 1:ColorProjectorSet(0, Palette[0][PAL0_COLOR_RED]);break;
        case 2:ColorProjectorSet(0, Palette[0][PAL0_COLOR_YELLOW]);break;
        case 3:ColorProjectorSet(0, Palette[0][PAL0_COLOR_GREEN]);break;
        case 4:ColorProjectorSet(0, Palette[0][PAL0_COLOR_BLUE]);break;
      }
    }
    lastKey = key;
  }

}


