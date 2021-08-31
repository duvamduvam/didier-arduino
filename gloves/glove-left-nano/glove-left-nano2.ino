#include <Keypad.h>
#include "ArduinoLog.h"
#include <Button.h>
//#include "Vibrator.h"

//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#define VIBRATION_PIN 3

//Vibrator vibrator(VIBRATION_PIN);
//Mapping mapping;

//index bleu
const byte rows = 3; //four rows
const byte cols = 4; //three columns
char keys[rows][cols] = {
  {'c', 'f', 'g', 'j'},
  {'b', 'd', 'i', 'l'},
  {'a', 'e', 'h', 'k'},
};
byte rowPins[rows] = {6, 7, 8}; //connect to the row pinouts of the keypad
byte colPins[cols] = {11, 12, 4, 13}; //connect to the column pinouts of the keypad

// 11 12 4  13

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

//dead 9 / 3 / A3 / A6 / A7

char msg[4];
char key, holdKey;
unsigned long t_hold;

void setup()
{
  Serial.begin(115200);
  Log.begin   (LOG_LEVEL, &Serial);
  Log.notice("start glove left");

  //pinMode(VIBRATION_PIN, OUTPUT);
  //analogWrite(VIBRATION, 0);

  msg[0] = '<';
  msg[1] = '0';
  msg[2] = '>';
  msg[3] = '\0';

}


void loop()
{
  char key = keypad.getKey();

  if (key != NO_KEY) {
    Serial.print("<"); Serial.print(key); Serial.println(">");
    holdKey = key;
  }

  if (keypad.getState() == HOLD) {
    if ((millis() - t_hold) > 100 ) {
      Serial.print("<"); Serial.print(holdKey); Serial.println(">");
      t_hold = millis();
    }
  }

  //vibrator.process();

  delay(20);
}
