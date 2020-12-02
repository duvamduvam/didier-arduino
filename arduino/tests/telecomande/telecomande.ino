// LoRa 9x_TX8
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example LoRa9x_RX

#include <SPI.h>
#include <RH_RF95.h>
#include <Keypad.h>

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 868.0

#include <SD.h>
#include <FTOLED.h>

//screen
const byte pin_cs = 7;
const byte pin_dc = 2;
const byte pin2_reset = 3;

OLED oled(pin_cs, pin_dc, pin_reset);

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

// keypad
const byte rows = 4; //nombre de lignes
const byte cols = 4; //nombre de colonnes
char keys[rows][cols] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[rows] = {41, 43, 45, 47}; //entrées numériques où sont branchées les lignes
byte colPins[cols] = {33, 35, 37, 39}; //entrées numériques où sont branchées les colonnes

long lastModPressed = 0;
char lastPressed;
char mod = 'A';


Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );


void setup()
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  while (!Serial);
  Serial.begin(9600);
  delay(100);

  Serial.println("Arduino LoRa Remote");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  rf95.setTxPower(23, false);
}

int16_t packetnum = 0;  // packet counter, we increment per xmission

void sendMsg(char msg[]) {
  //itoa(packetnum++, msg + 13, 10);
  Serial.print("Sending "); Serial.println(msg);
  //msg[19] = 0;
  Serial.println("Sending..."); delay(10);
  rf95.send((uint8_t *)msg, 2);
}

void loop()
{


  // test monitor
  char debugInput[32];
  int availableBytes = Serial.available();
  for (int i = 0; i < availableBytes; i++)
  {
    debugInput[i] = Serial.read();
  }
  if (availableBytes > 0) {
    Serial.print("monitor input : ");Serial.println(debugInput);
    sendMsg(debugInput);
  }
  // end test monitor

  char key = keypad.getKey();

  if (key != NO_KEY) {
    Serial.println(key);
    char in[2];
    in[0] = mod;
    in[1] = key;
    //for (int i = 2; i < 10; i++) {
    //  in[i] = ' ';
    //}
    sendMsg(in);
  }


  if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
    if (millis() - lastModPressed < 500) {
      mod = key;
    }
    lastModPressed = millis();
  }
}
