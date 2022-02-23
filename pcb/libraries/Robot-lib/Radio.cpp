//#include "ArduinoLog.h"
#include "Arduino.h"
#include "Radio.h"
#include <RH_RF95.h>

RH_RF95 rf95(RFM95_CS, RFM95_INT);


//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

/*Radio::Radio(){
  
	Serial.println("init radio");

  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);

  delay(100);

  digitalWrite(RFM95_RST, LOW);
  delay(100);
  digitalWrite(RFM95_RST, HIGH);
  delay(100);
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!\n");
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  //printString(debug, "Set Freq to: "); printDoubleLn(debug, RF95_FREQ);
  rf95.setTxPower(23, false);
}*/

void Radio::init() {
	Serial.println("init radio");

  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  while (!Serial);

  delay(100);

  digitalWrite(RFM95_RST, LOW);
  delay(100);
  digitalWrite(RFM95_RST, HIGH);
  delay(100);
  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!\n");
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  //printString(debug, "Set Freq to: "); printDoubleLn(debug, RF95_FREQ);
  rf95.setTxPower(23, false);
}

void Radio::read() {
  if (rf95.available())
  {
    uint8_t len = sizeof(msg);
    if (rf95.recv((uint8_t *)msg, &len))
    {
      digitalWrite(LED, HIGH);
      Serial.print("radio received (");Serial.print(len);Serial.print(") : ");Serial.println((char*)msg);
      digitalWrite(LED, LOW);

    }
    else
    {
      Serial.println("Receive failed");
    }
  }
}

void Radio::msgset(){
  memset(msg, 0, sizeof(msg));
}

void Radio::RfOut(){
  //Output
  int len = strlen(msg);
  rf95.send((uint8_t *)msg, len);
  //Serial.print("Sending (");Serial.print(len); Serial.print(") : ");Serial.println(msg);
}

void Radio::sendCharMsg(char m, char input) {

  msg[0] = '<';
  msg[1] = m;
  msg[2] = input;
  msg[3] = '>';
  msg[4] = 0;

  RfOut();
}

void Radio::sendArrayMsg(char m, char* input) {

  msg[0] = '<';
  msg[1] = m;
  msg[2] = input[0];
  msg[3] = input[1];
  msg[4] = '>';
  msg[5] = 0;

  RfOut();
}


void Radio::sendMsg(char* input, int size) {
	
  msg[0] = '<';
  Serial.print(" sendMsg : ");Serial.print(input);Serial.print(" size ");Serial.println(size);
  for(int i=1;i <= size;i++){
	  msg[i] = input[i-1];
	  //Serial.print("input ");Serial.print(i);Serial.print(" size ");Serial.print(size);Serial.print(" ");Serial.println(input[i-1]);
	  if(i==size){
		msg[i+1] = '>';
		msg[i+2] = 0;
	  }
  }
  RfOut();
}

void Radio::sendDecMsg(char m, int value) {

  char charNumber[2] = "";
  itoa ( value, charNumber, 10);

  sendArrayMsg(m, charNumber);

}

void Radio::sendVectorMsg(char m, int valueX, int valueY) {

  char sX[7], sY[7];
  char prefixe[2] = " ";
  prefixe[0] = m;
  char header[2] = "<";

  msg[0] = 0;
  strcpy(msg, header);
  strcat(msg, prefixe);
  itoa (valueX, sX, 10);
  strcat(msg, sX);
  strcat(msg, ",");
  itoa (valueY, sY, 10);
  strcat(msg, sY);
  strcat(msg, ">");

  RfOut();
}



void Radio::sendIntMsg(char m, int value) {

  char sX[7];
  char prefixe[2] = " ";
  prefixe[0] = m;
  char header[2] = "<";

  msg[0] = 0;
  strcpy(msg, header);
  strcat(msg, prefixe);
  itoa (value, sX, 10);
  strcat(msg, sX);
  strcat(msg, ">");

  RfOut();
}
