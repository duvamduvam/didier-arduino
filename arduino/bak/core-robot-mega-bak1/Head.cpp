#include <Arduino.h>
#include "ArduinoLog.h"
//#include "Utils.h"

//to fix no such file <Wire.h>, the wire library was added to pwmservodriver from https://github.com/adafruit/Adafruit_Arduino_Boards
//Library manager -> adafruit pwm servo driver library
#include <Adafruit_PWMServoDriver.h>

#define LOG_LEVEL LOG_LEVEL_VERBOSE

class Head {

    Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

    //SDA 4 SCL 5
    uint8_t servo0 = 0;
    uint8_t servo1 = 1;

#define SERVOMIN0  200 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX0  320 // this is the 'maximum' pulse length count (out of 4096)

#define SERVOMIN1  0 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX1  500 // this is the 'maximum' pulse length count (out of 4096)

    int curServPos0 = SERVOMIN0;
    int curServPos1 = SERVOMIN1;

    int speed = 200;
    unsigned long lastMoveTime;
    int headStep = 10;

    int moveServo(int p, int servo, int currentPos, int MIN, int MAX) {

      //printStringLn(debug, "servo " + (String) servo);
      //printStringLn(debug, "pos " + (String) p);
      //printStringLn(debug, (String) currentPos);

      if (p > 7 && currentPos < MAX) {
        currentPos = currentPos + headStep;
        pwm.setPWM(servo, 0, currentPos);
      } else if (p < 3 && currentPos > MIN) {
        currentPos = currentPos - headStep;
        pwm.setPWM(servo, 0, currentPos);
      }

      return currentPos;
    }

  public:
    void process(int input) {
      //delay(500);


      //move head up and dow
      //curServPos0 = moveServo(p0, servo0, curServPos0, SERVOMIN0, SERVOMAX0);
      //curServPos1 = moveServo(p1, servo1, curServPos1, SERVOMIN1, SERVOMAX1);



    }

};
