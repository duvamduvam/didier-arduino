#include <Arduino.h>
#include "ArduinoLog.h"
//#include "Utils.h"

//to fix no such file <Wire.h>, the wire library was added to pwmservodriver from https://github.com/adafruit/Adafruit_Arduino_Boards
//Library manager -> adafruit pwm servo driver library
#include <Adafruit_PWMServoDriver.h>

#define LOG_LEVEL LOG_LEVEL_VERBOSE

class Head {

    Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  80 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  640 // this is the 'maximum' pulse length count (out of 4096)

    int speed = 200;
    unsigned long lastMoveTime;
    int step = 10;
    int currentPos = 400;

    int moveServo(int step) {

      //printStringLn(debug, "servo " + (String) servo);
      //printStringLn(debug, "pos " + (String) p);
      //printStringLn(debug, (String) currentPos);

      if (currentPos < SERVOMAX) {
        currentPos = currentPos + step;
        pwm.setPWM(0, 0, currentPos);
      }
    }

  public:
    void process(uint8_t in[]) {
      //delay(500);
      if (strstr((char*)in, "B37") != 0)
      {
        moveServo(step);
      } else if (strstr((char*)in, "B31") != 0)
      {
        moveServo(-step);
      }
    }

};
