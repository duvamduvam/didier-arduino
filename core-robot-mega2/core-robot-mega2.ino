
//local arduino_midi_library-master.zip / conflit with servo lib #define RH_ASK_ARDUINO_USE_TIMER2 in RH_ASK.h

#include <SPI.h>
//local RadioHead-master.zip
//used pin 9 2 maybe used 13 12 11 10 8 7 6
#include <RH_RF95.h>
#include "ArduinoLog.h"
//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#include "Move.cpp"
#include "Lights.cpp"
#include "Head.cpp"
#include "Mapping.cpp"
#include "Commands.h"
#include "Sound.cpp"


/*PINMAP
  0 COM
  1 COM
  2 RFM95_INT
  3 DIR_RIGHT
  4 PWM_RIGHT
  5
  6 LIGHT
  7
  8
  9 RFM95_RST
  10 RFM95_CS
  11 DIR_LEFT
  12 PWM_LEFT
  13 LED
  SDA MEGA Adafruit_PWMServoDriver
  SCL MEGA Adafruit_PWMServoDriver

*/
#define RFM95_INT 2
#define RFM95_RST 9
#define RFM95_CS 10
#define LED 13


Move wheel;
long lastMove = 0;
bool stopped = true;
#define moveTime 300

/// start radio def /////

#define RF95_FREQ 868.0
RH_RF95 rf95(RFM95_CS, RFM95_INT);


uint8_t radioMsg[12];
uint8_t len = sizeof(radioMsg);

Lights lights;
Head head;
Sound sound;

bool testInputRead = false;

Mapping mapping;

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);
  // eratics problem with lora radio when changing baud rate
  Serial.begin(115200);
  Log.begin   (LOG_LEVEL, &Serial);

  delay(100);

  head.init();

  // init radio manual reset
  /*digitalWrite(RFM95_RST, LOW);
    delay(10);
    digitalWrite(RFM95_RST, HIGH);
    delay(10);
    while (!rf95.init()) {
    Log.error("LoRa radio init failed");
    while (1);
    }
    Log.notice("LoRa radio init OK!\n");
    if (!rf95.setFrequency(RF95_FREQ)) {
    Log.error("setFrequency failed");
    while (1);
    }
    //printString(debug, "Set Freq to: "); printDoubleLn(debug, RF95_FREQ);
    rf95.setTxPower(23, false);*/
}

void radioRead() {
  if (rf95.available())
  {
    if (rf95.recv(radioMsg, &len))
    {
      digitalWrite(LED, HIGH);
      //Log.notice(F("radio received %s")CR, radioMsg);
      digitalWrite(LED, LOW);
    }
    else
    {
      Log.notice("Receive failed");
    }
  }
}

void loop()
{

  if (!testInputRead) {
    //sound.process("! 111% |+ 222% |+ 333% |+ 444% |+ 555% ");
    //sound.process("! 123% ");
    //sound.process("! 111% |+ 222% |+ 333% |+ 444% |+ 555% ");

    //head.process("! \"\"% ");
    //head.process("! \"\"% |! )Z% ");

    //lights.process("! LE- ");
    lights.process("! LE-% |! LE-% ");

    testInputRead = true;
  }
  //sound.execute();
  //head.execute();
  lights.execute();
  /*radioRead();

    Commands commands = mapping.getCommands((char*)radioMsg);

    char* input = mapping.getValue((char*)radioMsg);
    // test monitor
    if (Serial.available()) {
    //String in = Serial.readStringUntil('\n');
    //Log.notice("You typed: %s \n", (char*)in );
    }

    if (strcmp((char*)radioMsg, "") != 0) {
    Log.notice("radio: " );
    Serial.println((char*)radioMsg);
    Log.notice("input: %s \n", input );

    head.process(commands.neck);
    lights.process(commands.lights);
    wheel.process(commands.wheel);

    sound.process(commands.sound);

    lastMove = millis();
    stopped = false;
    }

    wheel.execute();
    lights.execute();

    if ((millis() - lastMove > moveTime) && !stopped) {
    wheel.stop();
    stopped = true;
    }

    memset(radioMsg, 0, sizeof(radioMsg));*/
}
