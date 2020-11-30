

// SimpleTx - the master or the transmitter

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


//SPI MOSI 51 | MISO 50 | SCK 52
#define CE_PIN 53
#define CSN_PIN 48

const byte slaveAddress[5] = {'R', 'x', 'A', 'A', 'A'};


RF24 radio(CE_PIN, CSN_PIN); // Create a Radio

char dataToSend[10] = "Message 0";
char txNum = '0';


unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1000; // send once per second



void send() {

  bool rslt;
  rslt = radio.write( &dataToSend, sizeof(dataToSend) );
  // Always use sizeof() as it gives the size as the number of bytes.
  // For example if dataToSend was an int sizeof() would correctly return 2

  Serial.print("Data Sent ");
  Serial.print(dataToSend);
  if (rslt) {
    Serial.println("  Acknowledge received");
    updateMessage();
  }
  else {
    Serial.println("  Tx failed");
  }
}

void sendChar(int in) {
  dataToSend[0] = in;
  send();
}

//================

void updateMessage() {
  // so you can see that new data is being sent
  txNum += 1;
  if (txNum > '9') {
    txNum = '0';
  }
  dataToSend[8] = txNum;
}

void setup() {

  Serial.begin(9600);

  Serial.println("SimpleTx Starting");

  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate( RF24_250KBPS );
  radio.setRetries(3, 5); // delay, count
  radio.openWritingPipe(slaveAddress);
}

//====================

void loop() {

  int interval = 2000;
  currentMillis = millis();

  sendChar('q');
  delay(interval);
  sendChar('a');
  delay(interval);
  sendChar('z');
  delay(interval);
  sendChar('s');
  delay(interval);

  //if (currentMillis - prevMillis >= txIntervalMillis) {
  //  send();
  //  prevMillis = millis();
}


//====================
