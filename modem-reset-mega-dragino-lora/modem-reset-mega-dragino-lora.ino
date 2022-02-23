#include "ArduinoLog.h"
#include "Radio.h"
#include "FonctionsMega.h"

//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#define RST_PIN 31
#define BUILD_IN 13
#define RST_READ 40

#define RST "<D#>"
#define PERIOD_TIME 5000

#define INPUT_SIZE 20
char msg  [INPUT_SIZE];
bool newData = false;
byte inputCount = 0;

char dueMsg[10] = {};

int current_time;

Radio radio;

void setup()
{

  Serial.begin(9600);
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial3.begin(115200);
  Serial.println("init reset board");
  pinMode(RST_PIN, OUTPUT);
  pinMode(RST_READ, INPUT);

  Log.begin   (LOG_LEVEL, &Serial);

  current_time = millis();
  radio.init();
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
  newData = recvWithStartEndMarkersMega(dueMsg, newData, 10, 3);

  if (newData) {
    Serial.println(dueMsg);
    newData = false;
  }
}
