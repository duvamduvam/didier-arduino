#include "Arduino.h"
#include "heltec.h"
#include "WiFi.h"
#include "images.h"

#define BAND    868E6  //you can set band here directly,e.g. 868E6,915E6

String rssi = "RSSI --";
String packSize = "--";
char packet[3];

unsigned int counter = 0;

bool receiveflag = false; // software flag for LoRa receiver, received data makes it true.

long lastSendTime = 0;        // last send time
int interval = 1000;          // interval between sends
uint64_t chipid;

bool newData = false;

void logo() {
  Heltec.display -> clear();
  Heltec.display -> drawXbm(0, 5, logo_width, logo_height, (const unsigned char *)logo_bits);
  Heltec.display -> display();
}


bool resendflag = false;
bool deepsleepflag = false;
void interrupt_GPIO0()
{
  delay(10);
  if (digitalRead(0) == 0)
  {
    if (digitalRead(LED) == LOW)
    {
      resendflag = true;
    }
    else
    {
      deepsleepflag = true;
    }
  }
}

void setup()
{

  Serial.begin(115200);

  Heltec.begin(true /*DisplayEnable Enable*/, true /*LoRa Enable*/, true /*Serial Enable*/, true /*LoRa use PABOOST*/, BAND /*LoRa RF working band*/);

  logo();
  delay(300);
  Heltec.display -> clear();


  chipid = ESP.getEfuseMac(); //The chip ID is essentially its MAC address(length: 6 bytes).
  //Serial.printf("ESP32ChipID=%04X", (uint16_t)(chipid >> 32)); //print High 2 bytes
  //Serial.printf("%08X\n", (uint32_t)chipid); //print Low 4bytes.

  attachInterrupt(0, interrupt_GPIO0, FALLING);
  LoRa.onReceive(onReceive);
  send();
  LoRa.receive();
  displaySendReceive();
}


void loop()
{
  LoRa.receive();
  displaySendReceive();
  if (newData) {
    Serial.println(packet);
    //flashLed();
    newData = false;
  }
}

void send()
{
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter++);
  LoRa.endPacket();
}
void displaySendReceive()
{
  Heltec.display -> drawString(0, 50, "Packet " + (String)(counter - 1) + " sent done");
  Heltec.display -> drawString(0, 0, "Received Size  " + packSize + " packages:");
  Heltec.display -> drawString(0, 10, packet);
  Heltec.display -> drawString(0, 20, "With " + rssi + "db");
  Heltec.display -> display();
  delay(100);
  Heltec.display -> clear();
}

void flashLed() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(50);                       // wait for a second
    digitalWrite(LED, LOW);    // turn the LED off by making the voltage LOW
    delay(50);
  }
}

void onReceive(int packetSize)//LoRa receiver interrupt service
{
  //if (packetSize == 0) return;

  packSize = String(packetSize, DEC);
  int i = 0;
  while (LoRa.available())
  {
    packet[i] = (char) LoRa.read();
    i++;
  }

  if (i > 2) {
    newData = true;
  }

  rssi = "RSSI: " + String(LoRa.packetRssi(), DEC);
  receiveflag = true;

}
