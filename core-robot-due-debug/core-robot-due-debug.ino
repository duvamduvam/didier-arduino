#include "Fonctions.h"
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
//#include "Face.h"


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

//RADIO
unsigned int recv_size = 0;
unsigned long prev, interval = 5000;


const int inputSize = 5;
char radio  [5];
bool newData = false;
byte inputCount = 0;

Move wheel;
long lastMove = 0;
bool stopped = true;
#define moveTime 50

Lights lights;
Head head;
Sound sound;
Commands commands;
//Face face;

bool testInputRead = false;

Mapping mapping;

void setup()
{

  Serial.begin(115200);
  Serial2.begin(9600);
  Log.begin   (LOG_LEVEL, &Serial);

  //face.loadSequence(0);

  delay(100);

}


void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();
    if (recvInProgress == true) {
      if (rc != endMarker) {
        radio[ndx] = rc;
        ndx++;
        if (ndx >= inputSize) {
          ndx = inputSize - 1;
        }
      }
      else {
        radio[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }
    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

void recvWithStartEndMarkers2() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial2.available() > 0 && newData == false) {
    rc = Serial2.read();
    if (recvInProgress == true) {
      if (rc != endMarker) {
        radio[ndx] = rc;
        ndx++;
        if (ndx >= inputSize) {
          ndx = inputSize - 1;
        }
      }
      else {
        radio[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }
    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

void loop()
{

  //debug
  // if (Serial.available()) {

  //keyboard debug
  String keyboardInput =  Serial.readStringUntil('\0');
  char debugInput[3];
  keyboardInput.toCharArray(debugInput, 3);
  newData = true;
  Log.notice("Serial keyboard available : \"%s\"\n", debugInput);

  commands = mapping.getCommands("HU");

  //sound.process(commands.sound);
  //wheel.process(commands);
  //Log.notice("commands.wheels : \n", commands.wheel);
  //head.process(commands.neck);
    Log.notice("Loop before   lights.process(): \"%s\"\n", commands.lights);
  lights.process(commands.lights);


  lastMove = millis();
  stopped = false;


  //wheel.execute();
  lights.execute();
  //head.execute();
  //sound.execute();
  //face.execute();

  if ((millis() - lastMove > moveTime) && !stopped) {
    wheel.stop();
    stopped = true;
  }

  delay(50000);
}
