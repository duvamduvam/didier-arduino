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
*/

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


/*void sendMidi() {

  radio.read();

  char note[2];
  note[0] = radio.msg[2];
  note[1] = radio.msg[3];

  //sendNote(input[1], atoi(note));
  if (radio.msg[1] == 'C') {
    int atoiNote = atoi(note);
    if (atoiNote == 0) {
      sendNote(radio.msg[1], (int)(note[0]));
    } else {
      sendNote(radio.msg[1], atoi(note) + 60);
    }
  }
  }

  void sendNote(char mod, uint8_t note) {

  midiEventPacket_t noteOn = {0x09, 0x90 | 0, note, 64};
  MidiUSB.sendMIDI(noteOn);

  midiEventPacket_t noteOff = {0x08, 0x80 | 0, note, 64};
  MidiUSB.sendMIDI(noteOff);

  lastSend = millis();
  Log.notice("send note %i\n", note);
  //}
  }*/

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
    Commands commands = mapping.getCommands(extracMsg);
    Log.notice(" commands ligths : %s", commands.lights);
    lights.process(commands.lights);
    //TODO extract prefix and postfix from here

  }

  lights.execute();

  radio.msgset();
}
