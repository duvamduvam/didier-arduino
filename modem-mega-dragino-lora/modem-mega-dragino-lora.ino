#include "ArduinoLog.h"
#include <RH_RF95.h>
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
  Serial2.begin(115200);

  Log.begin   (LOG_LEVEL, &Serial);
  //midi debug

  delay(100);

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
void loop()
{
  radioRead();

  if (strcmp(radioMsg, "")) {
    Serial2.print((char*)radioMsg);
  }

  /*if (Serial.available() > 0) {
    String s0 = Serial.readStringUntil('\0');
    Serial.print("serial zero received:"); Serial.println(s0);
    }*/

  memset(radioMsg, 0, sizeof(radioMsg));
}
