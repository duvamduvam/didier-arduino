
#include "Fonctions.h"
#include "FonctionsMega.h"
#include "ArduinoLog.h"
//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#define LOG_NOTICE_CORE

#include "Wheel.cpp"
#include "Mapping.h"
#include "Commands.h"
#include "Sound.cpp"
//#include "Lights.cpp"
#include "Face.h"
#include "Relays.cpp"

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

#define INPUT_SIZE 1
char radio  [INPUT_SIZE];
bool newData = false;
byte inputCount = 0;

Wheel wheel;
long lastMove = 0;
bool stopped = true;
#define moveTime 200

Sound sound;
Face face;
//Lights lights;
Relays relays;

bool testInputRead = false;

Mapping mapping;

void setup()
{
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);

  Serial.println("start due core");
  Log.begin   (LOG_LEVEL, &Serial);
  //neck.init();
  delay(100);
}

void loop()
{
  //input from keyboard
  // recvWithStartEndMarkersMega(radio, newData, 20, 0);

  //input form modem
  newData = recvWithStartEndMarkersMega(radio, newData, 20, 1);

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
    //neck.process(commands.neck);
    face.process(commands.face);
    //lights.process(commands.lights);
    relays.process(commands.relays);

    if (commands.neck[0] == 'N') {
      //char neck[20];
      //strcpy(neck, commands.neck);
      commands.neck[0] = '<';
      Serial3.print(commands.neck); Serial3.println(">");
      Log.notice("send %s> to serial3\n", commands.neck);
    }

    if (commands.face[0]=='F') {
      relays.process("R2");
      Serial3.print("<VE>");
      Log.notice("send voice effect\n");
    }

    lastMove = millis();
    stopped = false;
  }

  wheel.execute();
  //sound.execute();
  //neck.execute();
  face.execute();
  //lights.execute();


  if ((millis() - lastMove > moveTime) && !stopped) {
    wheel.stop();
    stopped = true;
  }

  Serial2.println("<alive>");

}
