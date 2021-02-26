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

char loraInput[3];
bool newData = false;
byte inputCount = 0;

Move wheel;
long lastMove = 0;
bool stopped = true;
#define moveTime 300


Lights lights;
Head head;
Sound sound;

bool testInputRead = false;

Mapping mapping;

void setup()
{

  Serial.begin(115200);
  Serial2.begin(115200);
  Log.begin   (LOG_LEVEL, &Serial);

  delay(100);

}

void loop()
{

  if (Serial2.available()) {
    String input = Serial2.readStringUntil('\n');
    input.toCharArray(loraInput, 4);
    //Serial.print("lora input : "); Serial.println(loraInput);
    newData = true;
  }

  if (newData) {
    newData = false;
    Log.notice("input from lora %s\n", loraInput);
    Commands commands = mapping.getCommands(loraInput);

    sound.process(commands.sound);
    //head.process(commands.neck);
    //lights.process(commands.lights);
    //sound.process(commands.sound);
  }

  /*if (!testInputRead) {
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
    lights.execute();*/
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
