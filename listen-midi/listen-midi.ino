//local arduino_midi_library-master.zip


//#include "MIDIUSB.h"
#include <SPI.h>
//local RadioHead-master.zip
//used pin 9 2 maybe used 13 12 11 10 8 7 6
#include <RH_RF95.h>
#include "ArduinoLog.h"
//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

/*PINMAP
  0 COM
  1 COM
  2 RFM95_INT
  3
  4
  5
  6
  7
  8
  9 RFM95_RST
  10 RFM95_CS
  11
  12
  13 LED

*/
#define RFM95_INT 2
#define RFM95_RST 9
#define RFM95_CS 10
#define LED 13


/// start radio def /////

#define RF95_FREQ 868.0
RH_RF95 rf95(RFM95_CS, RFM95_INT);


uint8_t radioMsg[12];
uint8_t len = sizeof(radioMsg);

//Mapping mapping;

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);
  // eratics problem with lora radio when changing baud rate
  Serial.begin(115200);
  Log.begin   (LOG_LEVEL, &Serial);
  //midi debug
  Serial1.begin(115200);

  delay(100);

  //// init radio
  //printStringLn(debug, "Arduino LoRa RX Test!");
  // manual reset
  digitalWrite(RFM95_RST, LOW);
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
  rf95.setTxPower(23, false);
}

void radioRead() {
  if (rf95.available())
  {
    if (rf95.recv(radioMsg, &len))
    {
      digitalWrite(LED, HIGH);
      Log.notice("radio received %s\n", radioMsg);
      digitalWrite(LED, LOW);
    }
    else
    {
      Log.error("Receive failed");
    }
  }
}

/*void sendNote(char mod, int note) {

  midiEventPacket_t noteOn = {0x09, 0x90 | 0, note, 64};
  //MidiUSB.sendMIDI(noteOn);

  midiEventPacket_t noteOff = {0x08, 0x80 | 0, note, 64};
  //MidiUSB.sendMIDI(noteOff);

  Serial1.println("send");
  debugMidiEventPacket(noteOn);

}*/

/*void debugMidiEventPacket(midiEventPacket_t in) {
  if (in.header != 0 && in.byte1 != 0 && in.byte2 != 0 && in.byte3 != 0) {
    Serial1.print("Received: ");
    Serial1.print(in.header, HEX);
    Serial1.print("-");
    Serial1.print(in.byte1, HEX);
    Serial1.print("-");
    Serial1.print(in.byte2, HEX);
    Serial1.print("-");
    Serial1.println(in.byte3, HEX);
  }
}*/

void loop()
{
/*  midiEventPacket_t rx;
  do {
    rx = MidiUSB.read();
    debugMidiEventPacket(rx);
  } while (rx.header != 0);
*/
  radioRead();

  // char* input = mapping.getValue((char*)radioMsg);
  char* input = (char*)radioMsg;
  // test monitor
  if (Serial.available()) {
    String in = Serial.readStringUntil('\n');
    //Log.notice("You typed: %s \n", in );
  }

  if (strcmp((char*)radioMsg, "") != 0) {
    Log.notice("radio: %s \n", radioMsg );
    Log.notice("input: %s \n", input );
    char note[2];
    note[0] = input[2];
    note[1] = input[3];
    //sendNote(input[1], atoi(note));
    //arduino mega reboot after sending note without delay
    delay(30);

  }


  memset(radioMsg, 0, sizeof(radioMsg));
}
