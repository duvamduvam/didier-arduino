#include <Arduino.h>
//#include "MIDIUSB.h"
#include "ArduinoLog.h"
#include "Mapping.h"
#include "Lights.cpp"
#include "Commands.h"
#include "Radio.h"

//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

//#define delayMidi 150

long lastSend = 0;

char speakMsg[3] = "CD";
int speakNote = 60;
boolean isSpeaking = false;
long speakTime = 0;
int speakTimeOut = 150;

Mapping mapping;
Lights lights;
Radio radio;

void setup() {

  Serial.begin(115200);
  Serial2.begin(115200);
  Serial.println("start due modem");
  Log.begin   (LOG_LEVEL, &Serial);
  radio.init();
}

void loop()
{
  radio.read();

  if (isSpeaking && (millis() - speakTime > speakTimeOut)) {
    //sendNote(0, speakNote);
    isSpeaking = false;
  }

  if (strcmp((char*)radio.msg, "")) {
    //Serial.println("send msg on due");
    Serial2.print((char*)radio.msg);
    Serial.print((char*)radio.msg);

    if (speakMsg == radio.msg && isSpeaking) {
      speakTime = millis();
    } else if (speakMsg == radio.msg && !isSpeaking) {
      isSpeaking = true;
      //sendMidi();
    } /*else {
      sendMidi();
    }*/

    //TODO improve msg management
    char extracMsg[20] = {};
    extractRadioChar(extracMsg, radio.msg);

    char buttons[3] = {};
    extractChar(buttons, extracMsg, 3, 4);
    Commands commands = mapping.getCommands(buttons);
    Log.notice(" extracted buttons : %s lights : %s\n", buttons, commands.lights);
    lights.process(commands.lights);
    //TODO extract prefix and postfix from here
  }

  lights.execute();

  //Serial2.println("test");
  
  radio.msgset();
}
