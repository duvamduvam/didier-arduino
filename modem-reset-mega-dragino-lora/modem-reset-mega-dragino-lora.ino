#include "ArduinoLog.h"
#include "Radio.h"

#define RST_PIN 31
#define BUILD_IN 13
#define RST_READ 40

#define RST "<UH>"
#define PERIOD_TIME 5000

#define INPUT_SIZE 20
char msg  [INPUT_SIZE];
bool newData = false;
byte inputCount = 0;

int current_time;

Radio radio;

void setup()
{

  Serial.begin(115200);
  Serial3.begin(115200);
  Serial.println("init reset board");
  pinMode(RST_PIN, OUTPUT);
  pinMode(RST_READ, INPUT);

  current_time = millis();

  radio.init();
}

void recvWithStartEndMarkers() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (Serial3.available() > 0 && newData == false) {
    rc = Serial3.read();
    if (recvInProgress == true) {
      if (rc != endMarker) {
        msg[ndx] = rc;
        ndx++;
        if (ndx >= INPUT_SIZE) {
          ndx = INPUT_SIZE - 1;
        }
      }
      else {
        msg[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
      }
    }
    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
}

void loop()
{

  radio.read();

  if (!strcmp((char*)radio.msg, RST)) {
    Serial.println("send reset");
    digitalWrite(RST_PIN, HIGH);
    delay(100);
    digitalWrite(RST_PIN, LOW);
  }

  radio.msgset();

  //check arduin
  recvWithStartEndMarkers();
  if(newData){
    Serial.println(msg);
  }
}
