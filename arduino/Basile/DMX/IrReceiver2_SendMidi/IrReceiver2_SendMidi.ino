#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();
//------------------------------------------------------------------------------------------------------------
//                                                  HARDWARE
//------------------------------------------------------------------------------------------------------------
#define PIN_RED         3
#define PIN_GREEN       5
#define PIN_BLUE        6

#define PIN_IR          8

#define PIN_SW0         9
#define PIN_SW1         10

#define PIN_TEST        13

//------------------------------------------------------------------------------------------------------------
//                                               STRUCTURES
//------------------------------------------------------------------------------------------------------------
struct TColor
{
  byte r;
  byte g;
  byte b;
};


//------------------------------------------------------------------------------------------------------------
//                                                  GLOBAL VARS
//------------------------------------------------------------------------------------------------------------

//Debug
uint32_t testTimer = 0; //Off



//------------------------------------------------------------------------------------------------------------
//                                                  SETUP
//------------------------------------------------------------------------------------------------------------

void setup () 
{
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);
  pinMode(PIN_TEST,  OUTPUT);
  pinMode(PIN_IR,  INPUT);
  pinMode(PIN_SW0,  INPUT);
  pinMode(PIN_SW1,  INPUT);

  testTimer=1;
  MIDI.begin(MIDI_CHANNEL_OMNI);
//  MIDI.setHandleControlChange(handleControlChange); 
//  MIDI.sendControlChange();
  
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
//                                                LED RGB
//------------------------------------------------------------------------------------------------------------
void OutputRGB(TColor color)
{
  analogWrite(PIN_RED,   255-color.r);
  analogWrite(PIN_GREEN, 255-color.g);
  analogWrite(PIN_BLUE,  255-color.b);
}

//------------------------------------------------------------------------------------------------------------
//                                                  TIMER
//------------------------------------------------------------------------------------------------------------
uint32_t Chrono(uint32_t *timer)
{
  if (*timer == 0) return 0;
  uint32_t now = millis();
  uint32_t dif = now-*timer;
  return dif;
}

extern volatile unsigned long timer0_overflow_count;
unsigned long ticks (void) //4us / tick
{
 return (timer0_overflow_count << 8) + TCNT0;
}


uint32_t ChronoTicks(uint32_t *timer)
{
  if (*timer == 0) return 0;
  uint32_t now = ticks();
  uint32_t dif = now-*timer;
  return dif;
}

//------------------------------------------------------------------------------------------------------------
//                                                  IR
//------------------------------------------------------------------------------------------------------------
void TelecommandeIR_SendCode(char car)
{
/*
  byte r = car & 3;
  byte g = (car>>2) & 3;
  byte b = (car>>4) & 3;
  OutputRGB(NewColor(r<<6,g<<6,b<<6));
*/
  MIDI.sendControlChange(1,car,1);
}

/********************************************************************
                        PARSE MODELS
                                  .-._     
                                 {_}^ )o  
                        {\________//~`     
                         (         )
                         /||~~~~~||\
                         |_\\_    \\_\_   
                         "' ""'    ""'"'
********************************************************************/
#define SEQ_WAIT    0 
#define SEQ_DLC_START 1
#define SEQ_WAIT_DLC  2 
#define SEQ_DLC_READY 3
#define SEQ_DLC_REPEAT  4
#define SEQ_BIT_START 5
#define SEQ_WAIT_STATE  6

/*
#define MODE_READ   0
#define MODE_PARSE    1
*/

char sendMode;
char seq;
char state;
char lastState; 
unsigned char lastSendedCode;

unsigned char bitIndex;
unsigned char byteIndex;
unsigned char currentByte;
unsigned char values[8];


uint32_t bitTimer = 0; //Off
uint32_t timeoutTimer = 0; //Off
uint32_t oscillatorTimer = 1;//On

unsigned char car, page, cmd;
unsigned char nbBits;


void Parse_ModelA(byte page, byte cmd)
{
  switch (cmd)
  {
    case 4: //Spécial
      switch (page)
      {
        case 0: TelecommandeIR_SendCode('B');break;//Down
        case 1: TelecommandeIR_SendCode('C');break;//Off
        case 2: TelecommandeIR_SendCode('A');break;//Up
        case 3: TelecommandeIR_SendCode('D');break;//On
      }
      break;

    case 1: //80%
      switch (page)
      {
        case 0: TelecommandeIR_SendCode('V');break;//G 80%
        case 1: TelecommandeIR_SendCode('W');break;//B 80%
        case 2: TelecommandeIR_SendCode('U');break;//R 80%

        //Special
        case 3: TelecommandeIR_SendCode('X');break;//Smooth
      }
      break;

    case 2: //100%
      switch (page)
      {
        case 0: TelecommandeIR_SendCode('F');break;//G 100%
        case 1: TelecommandeIR_SendCode('G');break;//B 100%
        case 2: TelecommandeIR_SendCode('E');break;//R 100%

        //Special
        case 3: TelecommandeIR_SendCode('H');break;//White
      }
      break;

    case 3: //60%
      switch (page)
      {
        case 0: TelecommandeIR_SendCode('R');break;//G 60%
        case 1: TelecommandeIR_SendCode('S');break;//B 60%
        case 2: TelecommandeIR_SendCode('Q');break;//R 60%

        //Special
        case 3: TelecommandeIR_SendCode('T');break;//Fade
      }
      break;

    case 5: //40%
      switch (page)
      {
        case 0: TelecommandeIR_SendCode('N');break;//G 40%
        case 1: TelecommandeIR_SendCode('O');break;//B 40%
        case 2: TelecommandeIR_SendCode('M');break;//R 40%

        //Special
        case 3: TelecommandeIR_SendCode('P');break;//Strobe
      }
      break;

    case 6: //20%
      switch (page)
      {
        case 0: TelecommandeIR_SendCode('J');break;//G 20%
        case 1: TelecommandeIR_SendCode('K');break;//B 20%
        case 2: TelecommandeIR_SendCode('I');break;//R 20%

        //Special
        case 3: TelecommandeIR_SendCode('L');break;//Flash
      }
      break;

  }
  
}


void Parse_ModelB(unsigned char page, unsigned char cmd)
{
  switch (cmd)
  {
    case 0: //Spécial
      switch (page)
      {
        case 0: TelecommandeIR_SendCode('A');break;//Up
        case 1: TelecommandeIR_SendCode('C');break;//Off
        case 2: TelecommandeIR_SendCode('B');break;//Down
        case 3: TelecommandeIR_SendCode('D');break;//On
      }
      break;

    case 1: //60%
      switch (page)
      {
        case 0: TelecommandeIR_SendCode('Q');break;//R 60%
        case 1: TelecommandeIR_SendCode('S');break;//B 60%
        case 2: TelecommandeIR_SendCode('R');break;//G 60%

        //Special
        case 3: TelecommandeIR_SendCode('T');break;//Fade
      }
      break;

    case 2: //20%
      switch (page)
      {
        case 0: TelecommandeIR_SendCode('I');break;//R 20%
        case 1: TelecommandeIR_SendCode('K');break;//B 20%
        case 2: TelecommandeIR_SendCode('J');break;//G 20%

        //Special
        case 3: TelecommandeIR_SendCode('L');break;//Flash
      }
      break;


  case 4: //100%
      switch (page)
      {
        case 0: TelecommandeIR_SendCode('E');break;//R 100%
        case 1: TelecommandeIR_SendCode('G');break;//B 100%
        case 2: TelecommandeIR_SendCode('F');break;//G 100%

        //Special
        case 3: TelecommandeIR_SendCode('H');break;//White
      }
      break;



    case 5: //80%
      switch (page)
      {
        case 0: TelecommandeIR_SendCode('U');break;//R 80%
        case 1: TelecommandeIR_SendCode('W');break;//B 80%
        case 2: TelecommandeIR_SendCode('V');break;//G 80%

        //Special
        case 3: TelecommandeIR_SendCode('X');break;//Smooth
      }
      break;


    case 6: //40%
      switch (page)
      {
        case 0: TelecommandeIR_SendCode('M');break;//R 40%
        case 1: TelecommandeIR_SendCode('O');break;//B 40%
        case 2: TelecommandeIR_SendCode('N');break;//G 40%

        //Special
        case 3: TelecommandeIR_SendCode('P');break;//Strobe
      }
      break;


  }
  
}



/********************************************************************
                  TRAITER
                    .-^-.
                   /_/_\_\
                  ' ' | ` `
                      j 
********************************************************************/


void Parse()
{
  car = values[2];
  page = car>>6;
  cmd = (car>>3)&7;

  switch (values[1])
  {
    case 0xFF: Parse_ModelA(page, cmd); break;
    case 0xF7: Parse_ModelB(page, cmd); break;
  }

}


/********************************************************************
                    BITS
                  _____     
               .-(_|||_)-. 
               "         " 
********************************************************************/

void ReinitBit()
{
  int i;
  for(i=0;i<8;i++)
    values[i]=0;
  bitIndex=0;
  byteIndex=0;
  currentByte =0;
  nbBits=0;

  timeoutTimer = 0; //Off
  bitTimer = 0; //Off

}

void EnterBit(char state)
{
  digitalWrite(PIN_TEST,state);

  nbBits++;

  if (bitIndex >= 8)
  {
    if (byteIndex <8)//secu
      values[byteIndex] = currentByte;

    currentByte=0;
    bitIndex=0;
    byteIndex++;
  }
  currentByte= currentByte<<1;
  currentByte += state;
  bitIndex++;
}







void ExecuteTelecommandeIR()
{
  state = digitalRead(PIN_IR);

  
  if (state!=lastState) //Front
  {

      timeoutTimer = millis();

      switch (seq)
      {
        case SEQ_WAIT:
          if (state == 0) //Start DLC Front descendant
            seq = SEQ_DLC_START;
          break;

        case SEQ_DLC_START:
//          digitalWrite(PIN_TEST,0);
          if (state == 1) //Front montant DLC
          {
            seq = SEQ_WAIT_DLC;
            bitTimer = ticks();
          }
          break;

        case SEQ_WAIT_DLC:
          if (state == 0) //Front descendant Repeat
          {
            seq = SEQ_DLC_REPEAT;
            
            //Repeat
          }
          break;

        case SEQ_DLC_REPEAT:
          if (state == 1) //Front montant fin
          {
            seq = SEQ_WAIT;
//            digitalWrite(PIN_TEST,1);
          }
          break;

        case SEQ_DLC_READY:
          if (state == 0) //Start Front descendant
          {
            seq = SEQ_BIT_START;
          }
          break;

        case SEQ_BIT_START:
          if (state == 1) //Front montant début bit
          {
         // digitalWrite(PIN_TEST,1);
            seq = SEQ_WAIT_STATE;
            bitTimer = ticks();
            timeoutTimer = millis();
          }
          break;

        case SEQ_WAIT_STATE:
          if (state == 0) //Front descendant Bit = 0
          {
            seq = SEQ_BIT_START;
          //digitalWrite(PIN_TEST,0);
            bitTimer = 0;
            EnterBit(0);
          }
          break;
      }
  

    }
    lastState = state;

    
    switch (seq)
    {
      case SEQ_WAIT_DLC:
        if (ChronoTicks(&bitTimer) > 750)
        {
          seq = SEQ_DLC_READY;
        }
        break;


      case SEQ_WAIT_STATE:
//        if (ChronoTicks(&bitTimer) > 250 )//1ms
        if (ChronoTicks(&bitTimer) > 350 )//1.4ms
        {
          EnterBit(1);
          //digitalWrite(PIN_TEST,1);
          seq = SEQ_BIT_START;
          bitTimer = 0; //Off
        }
        break;

      case SEQ_BIT_START:
        if (Chrono(&timeoutTimer) > 15)//15
        {
          Parse();
          ReinitBit();
          seq = SEQ_WAIT;
  //        digitalWrite(PIN_TEST,1);
        }
        break;
  }
}



//------------------------------------------------------------------------------------------------------------
//                                                 LOOP
//------------------------------------------------------------------------------------------------------------
int toggle;
void loop() 
{
  ExecuteTelecommandeIR();
}


