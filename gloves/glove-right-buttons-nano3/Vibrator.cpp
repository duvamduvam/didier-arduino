#include <Arduino.h>

#define VIBRATOR_PIN 3

#define PRESS_TIME 300

class Vibrator {

  private :
    long vibrationTime = 0;

  public :

    Vibrator() {
      pinMode(VIBRATOR_PIN, OUTPUT);
    }

    void init() {
      analogWrite(VIBRATOR_PIN, 0);
    }

    void process() {

    }

    void press() {
      //Serial.println("vibration press set");
      vibrationTime  = millis() +  PRESS_TIME;
    }

    void execute() {
      if (vibrationTime > millis()) {
        //Serial.println("vibration on");
        analogWrite(VIBRATOR_PIN, 255);
      } else {
        //Serial.println("vibration off");
        analogWrite(VIBRATOR_PIN, LOW);
      }


    }
    /*      for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5)
          {
            // sets the value (range from 0 to 255):
            analogWrite(VIBRATOR_PIN, fadeValue);
            // wait for 30 milliseconds to see the dimming effect
            delay(30);
          }

          // fade out from max to min in increments of 5 points:
          for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5)
          {
            // sets the value (range from 0 to 255):
            analogWrite(VIBRATOR_PIN, fadeValue);
            // wait for 30 milliseconds to see the dimming effect
            delay(30);
          }
        }*/

};
