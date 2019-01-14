//HC-12 Toggle button Send
//Autor Tom Heylen tomtomheylen.com


#include <SoftwareSerial.h>

SoftwareSerial hc12(2,3); //RX, TX

int buttonPin = 8;

void setup() {
  pinMode(buttonPin, INPUT);
  hc12.begin(9600);
  Serial.begin(9600);
}

void loop() {

  hc12.println(4444);//send unique code to the receiver in this case 1234
  delay(20);//delay little for better serial communication
}
