#include "Fonctions.h"
#include "ArduinoLog.h"
//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#define LOG_NOTICE_CORE

#include "Wheel.cpp"
#include "Lights.cpp"
#include "Neck.cpp"
#include "Mapping.cpp"
#include "Commands.h"
#include "Sound.cpp"
#include "Face.h"


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

#define INPUT_SIZE 20
char radio  [INPUT_SIZE];
bool newData = false;
byte inputCount = 0;

Wheel wheel;
long lastMove = 0;
bool stopped = true;
#define moveTime 200

Lights lights;
Neck neck;
Sound sound;
Face face;

bool testInputRead = false;

Mapping mapping;

void setup()
{

  Serial.begin(115200);
  Serial2.begin(115200);
  Log.begin   (LOG_LEVEL, &Serial);
  Log.notice("init core robot due");
  neck.init();

  delay(100);

}

boolean serialAvailable(byte nb) {

  switch (nb) {
    case 0:
      return Serial.available();
    case 1:
      return Serial1.available();
    case 2:
      return Serial2.available();
  }

}

char serialRead(byte nb) {

  switch (nb) {
    case 0:
      return Serial.read();
    case 1:
      return Serial1.read();
    case 2:
      return Serial2.read();
  }

}


void recvWithStartEndMarkers(byte serial) {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (serialAvailable(serial) > 0 && newData == false) {
    rc = serialRead(serial);
    if (recvInProgress == true) {
      if (rc != endMarker) {
        radio[ndx] = rc;
        ndx++;
        if (ndx >= INPUT_SIZE) {
          ndx = INPUT_SIZE - 1;
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

  //Log.notice("loop\n");

  //input from keyboard
  recvWithStartEndMarkers(0);

  //input form modem
  recvWithStartEndMarkers(2);

  if (newData)
  {

#ifdef LOG_NOTICE_CORE
    Log.notice("message from modem : %s\n", radio);
#endif

  }

  // }

  if (newData) {
    newData = false;
    //Serial.println(radio);
    // Log.notice("input from lora %s\n", loraInput);

    Commands commands = mapping.getCommands(radio);

    sound.process(commands.sound);
    wheel.process(commands);
    //Log.notice("commands.wheels : \n", commands.wheel);
    neck.process(commands.neck);
    lights.process(commands.lights);
    face.process(commands.face);

    lastMove = millis();
    stopped = false;
  }

  wheel.execute();
  lights.execute();
  //sound.execute();
  neck.execute();
  face.execute();


  if ((millis() - lastMove > moveTime) && !stopped) {
    wheel.stop();
    stopped = true;
  }


}
