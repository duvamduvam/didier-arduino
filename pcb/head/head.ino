
#include "ArduinoLog.h"
#include "FonctionsMega.h"
//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#include "Neck.cpp"
Neck neck;

#define EFFECT_PIN 3

#define INPUT_SIZE 10
char input[INPUT_SIZE];
bool newData, effect = false;

long effectTime;
int effectDuration = 200;

void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  Log.begin   (LOG_LEVEL, &Serial);
  Log.notice("start head servo\n");
  pinMode(EFFECT_PIN, OUTPUT);
  delay(200);
  digitalWrite(EFFECT_PIN, HIGH);
  neck.init();
}

void loop() {
  newData = recvWithStartEndMarkersMega(input, newData, INPUT_SIZE, 2);
  if (newData) {
    Serial.print("received : "); Serial.println(input);
    if (!strcmp(input, "VE")) {
      digitalWrite(EFFECT_PIN, LOW);
      effectTime = millis();
      effect = true;
      Log.notice("voice effect on\n");
    } else {
      neck.process(input[0]);
    }
    newData = false;
  }
  neck.execute();

  if (effect && ((effectTime + effectDuration) < millis())) {
    digitalWrite(EFFECT_PIN, HIGH);
    Log.notice("voice effect off\n");
    effect = false;
  }

  delay(10);

}
