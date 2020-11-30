#include <DMXSerial.h>

//------------------------------------------------------------------------------------------------------------
//                                                  HARDWARE
//------------------------------------------------------------------------------------------------------------
#define PIN_RED         3
#define PIN_GREEN       5
#define PIN_BLUE        6

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
//                                                  SETUP
//------------------------------------------------------------------------------------------------------------

void setup () 
{
  DMXSerial.init(DMXReceiver);
  
  pinMode(PIN_RED,   OUTPUT);
  pinMode(PIN_GREEN, OUTPUT);
  pinMode(PIN_BLUE,  OUTPUT);

  pinMode(PIN_SW0, INPUT_PULLUP);
  pinMode(PIN_SW1, INPUT_PULLUP);
  pinMode(PIN_SW2, INPUT_PULLUP);
  pinMode(PIN_SW3, INPUT_PULLUP);
  pinMode(PIN_SW4, INPUT_PULLUP);
  pinMode(PIN_SW5, INPUT_PULLUP);
  pinMode(PIN_SW6, INPUT_PULLUP);
  pinMode(PIN_SW7, INPUT_PULLUP);
  pinMode(PIN_SW8, INPUT_PULLUP);

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
//                                                 LOOP
//------------------------------------------------------------------------------------------------------------
void loop() 
{
  ReadAddress();
  unsigned long lastPacket = DMXSerial.noDataSince();
  if (lastPacket < 5000) 
  {
    analogWrite(PIN_RED,   255-DMXSerial.read(dmxAddress));
    analogWrite(PIN_GREEN, 255-DMXSerial.read(dmxAddress+1));
    analogWrite(PIN_BLUE,  255-DMXSerial.read(dmxAddress+2));
  } 
}


