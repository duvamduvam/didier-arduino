#include <Arduino.h>
//#include <analogWrite.h>

#include "heltec.h"
#include "images.h"

#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6

const byte inputSize = 5;
char nanoReceive;
char charSend[2];

const int xpin = 36;                  // x-axis of the accelerometer
const int ypin = 37;                  // y-axis
const int zpin = 38;                  // z-axis (only on 3-axis models)
const int vibration_pin = 25;

boolean newData = false;

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

  charSend[0] = 'X';

  pinMode(vibration_pin, OUTPUT);

}


/*void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial.available() > 0 && newData == false) {
    rc = Serial.read();
    if (recvInProgress == true) {
      if (rc != endMarker) {
        arduinoReceive[ndx] = rc;
        ndx++;
        if (ndx >= inputSize) {
          ndx = inputSize - 1;
        }
      }
      else {
        arduinoReceive[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }
    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
  }*/

void oledPrint(char* toPrint) {
  Heltec.display->clear();
  Heltec.display->setTextAlignment(TEXT_ALIGN_LEFT);
  Heltec.display->setFont(ArialMT_Plain_10);
  //Heltec.display->display();
  Heltec.display->drawString(0, 0, "Sending packet: ");
  Heltec.display->drawString(90, 0, toPrint);
  Heltec.display->display();
}

//TODO -> remove delay
void flashLed() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(50);                       // wait for a second
    digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
    delay(50);
  }
}

void sendData() {
  charSend[1] = nanoReceive;
  LoRa.beginPacket();
  LoRa.print(charSend);
  LoRa.endPacket();
  Serial.print("send : "); Serial.println(charSend);
  oledPrint(charSend);
  //flashLed();
}

void updateMod() {
  if (nanoReceive >= 85) {
    charSend[0] = nanoReceive;
  }
}

void loop()
{

  //recvWithStartEndMarkers();
  //sendData();

  if (Serial.available()) {
    nanoReceive = Serial.read();
    if (nanoReceive >= 65 && nanoReceive <= 88) {
      updateMod();
      sendData();
      Serial.print((int)nanoReceive); Serial.print(" : "); Serial.println(nanoReceive);
    };
  }

  /* 3 axis

     Serial.print(analogRead(xpin));

    // print a tab between values:

    Serial.print("\t");

    Serial.print(analogRead(ypin));

    // print a tab between values:

    Serial.print("\t");

    Serial.print(analogRead(zpin));

    Serial.println();

    // delay before next reading:

    delay(100);
  */



  /* vibrator motor


    for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5)
    {
    // sets the value (range from 0 to 255):
    analogWrite(vibration_pin, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
    }

    // fade out from max to min in increments of 5 points:
    for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5)
    {
    // sets the value (range from 0 to 255):
    analogWrite(vibration_pin, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
    }*/

  // send packet

  /*char* packetToSend;
         LoRa.beginPacket();
    if (Serial.available()) {
    Serial.print("serial available");
    Serial.readBytesUntil('\n', packetToSend, 2);
    Serial.print("send : "); Serial.println(Serial.read());
    send(packetToSend);  LoRa.setTxPower(14,RF_PACONFIG_PASELECT_PABOOST);
    LoRa.print("hello ");
    LoRa.print(packetToSend);
    LoRa.endPacket();

    Heltec.display->drawString(0, 0, "Sending packet: ");
    Heltec.display->drawString(90, 0, packetToSend);
    }*/

}
