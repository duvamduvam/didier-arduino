#include "Fonctions.h"
#include "FonctionsMega.h"
#include "ArduinoLog.h"
#include "LedStatus.cpp"
//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE
//#define LOG_NOTICE_CORE

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

#define INPUT_SIZE 8
char radio  [INPUT_SIZE];
bool newData = false;
bool newData3 = false;
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
LedStatus ledStatus;

//wheel test
unsigned int TestTimer = 0;
bool testChange = false;

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
  relays.init();
  //relays.process("R5");
  
}
 
void loop()
{

  ledStatus.process();
  //input form modem
  newData = recvWithStartEndMarkersMega(radio, newData, 20, 1);
  if (!newData){
    newData = recvWithStartEndMarkersMega(radio, newData, 20, 0);    
  }
  /*newData = true;

  radio[2] = '|';

  if (millis() - TestTimer > 3000)
  {
    if(testChange){
      radio[0] = '|';
      radio[1] = '|';
    }else{
      radio[0] = '1';
      radio[1] = '1';
    }
    TestTimer = millis();
    testChange = !testChange;
  }*/
  

  if (newData) {

    #ifdef LOG_NOTICE_CORE
        Log.notice("message from modem : [%s]\n", radio);
    #endif
    
    newData = false;
    //Serial.println(radio);
    Log.notice("input from lora =>%s<=\n", radio);
    char buttons[3] = {};
    extractChar(buttons, radio, 3, 4);  
    Commands commands = mapping.getCommands(buttons);

    sound.process(commands.sound);
    wheel.process(commands.wheel);
    if (radio[0] != ' ' && radio[1] != ' ') {
      wheel.process2(radio[0], radio[1]);
    }
    //Log.notice("commands.wheels : \n", commands.wheel);
    //neck.process(commands.neck);
    //Log.notice(" buttons : %s | commands.face %s: \n", buttons, commands.face);
    face.process(commands.face);
    //lights.process(radio[3]);
    relays.process(commands.relays);

    if (radio[2] != ' ') {
      //char neck[20];
      //strcpy(neck, commands.neck);
      //commands.neck[0] = '<';
      Serial3.print("<"); Serial3.print(radio[2]); Serial3.println(">");
      Log.notice("send <%s> to serial3\n", radio[2]);
    }

    if (radio[0] == 'N') {
      Log.notice("move head from button %s\n", commands.neck);
      Serial3.print("<"); Serial3.print(commands.neck); Serial3.println(">");      
    }


    if (commands.neck[0] == 'W') {
      Log.notice("move wheels  %s\n", radio);
      wheel.process_pi(radio);        
    }

    // Log.notice(" before send effect commands.face %s: \n", commands.face);
    if (commands.face[0] != 0) {
      //relays.process("R2");
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
  relays.execute();


  if ((millis() - lastMove > moveTime) && !stopped) {
    wheel.stop();
    stopped = true;
  }

  //delay(100);
  //Serial1.println("<alive>");

}
