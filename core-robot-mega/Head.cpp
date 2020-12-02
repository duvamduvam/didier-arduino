#include <Arduino.h>
#include "ArduinoLog.h"
#include <Adafruit_PWMServoDriver.h>

#define LOG_LEVEL LOG_LEVEL_VERBOSE

#define SERVOMIN  80 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  640 // this is the 'maximum' pulse length count (out of 4096)

class Head {

  private :
    Adafruit_PWMServoDriver pwm;
    int speed = 200;
    unsigned long lastMoveTime;
    int step = 10;
    int currentPos = 400;


    int moveServo(int step)
    {
      if (SERVOMIN >= currentPos <= SERVOMAX)
      {
        Log.notice ("position %d next step %d\n", currentPos, step);
        currentPos = currentPos + step;
        pwm.setPWM(0, 0, currentPos);
      }
    }

  public :
    //porgram don't work when initialized in constructor
    void init()
    {
      pwm = Adafruit_PWMServoDriver(0x40, Wire);
      pwm.begin();
      pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
    }

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
