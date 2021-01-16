/*
  This is a simple example show the Heltec.LoRa sended data in OLED.

  The onboard OLED display is SSD1306 driver and I2C interface. In order to make the
  OLED correctly operation, you should output a high-low-high(1-0-1) signal by soft-
  ware to OLED's reset pin, the low-level signal at least 5ms.

  OLED pins to ESP32 GPIOs via this connecthin:
  OLED_SDA -- GPIO4
  OLED_SCL -- GPIO15
  OLED_RST -- GPIO16
*/

#include "heltec.h"
#include "images.h"
#include "joystick.cpp"

#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6

unsigned int counter = 0;
String rssi = "RSSI --";
String packSize = "--";
String packet ;

char nanoReceive;
boolean newData = false;

char mod = '1';
boolean newMod = false;

void logo()
{
  Heltec.display->clear();
  Heltec.display->drawXbm(0, 5, logo_width, logo_height, logo_bits);
  Heltec.display->display();
}

void setup()
{
  //WIFI Kit series V1 not support Vext control
  Heltec.begin(true /*DisplayEnable Enable*/, true /*Heltec.Heltec.Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, BAND /*long BAND*/);

  Heltec.display->init();
  Heltec.display->flipScreenVertically();
  Heltec.display->setFont(ArialMT_Plain_10);
  logo();
  delay(1500);
  Heltec.display->clear();

  Heltec.display->drawString(0, 0, "Heltec.LoRa Initial success!");
  Heltec.display->display();
  delay(1000);
}

void oledPrint(char* toPrint) {
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  Heltec.display->display();
  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(90, 0, toPrint);
  Heltec.display->display();
}

void flashLed() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED, HIGH);
    delay(50);
    digitalWrite(LED, LOW);
    delay(50);
  }
}

void sendData() {

  char sendPacket[2];
  sendPacket[0] = mod;
  sendPacket[1] = nanoReceive;

  LoRa.beginPacket();
  LoRa.print(sendPacket);
  LoRa.endPacket();
  Serial.print("send : "); Serial.println(sendPacket);
  oledPrint(sendPacket);
  flashLed();
}

boolean setMod() {
  if (nanoReceive == 'Q') {
    mod = '1';
    return true;
  } else if (nanoReceive == 'R') {
    mod = '2';
    return true;
  } else if (nanoReceive == 'S') {
    mod = '3';
    return true;
  } else if (nanoReceive == 'T') {
    mod = '4';
    return true;
  }
  return false;
}

void loop()
{
  if (Serial.available() && !newData) {
    nanoReceive = Serial.read();
    newData = true;
  }

  if (newData && !setMod()) {
    sendData();
    newData = false;
  }

}
