#include <Arduino.h>
#include "ArduinoLog.h"
#include <Adafruit_PWMServoDriver.h>

#define SERVOMIN  80 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  640 // this is the 'maximum' pulse length count (out of 4096)

class Head {

  private :
    Adafruit_PWMServoDriver pwm;
    int step = 30;
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
      Log.notice("init head servo\n");
      pwm = Adafruit_PWMServoDriver();
      pwm.begin();
      pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
      pwm.setPWM(0, 0, 220);
      delay(100);
      pwm.setPWM(0, 0, currentPos);
    }

    void process(char in[]) {

      if (in[0] == 'H')
      {
        if (strstr((char*)in, "HLEFT") != 0)
        {
          moveServo(step);
        } else if (strstr((char*)in, "HRIGHT") != 0)
        {
          moveServo(-step);
        }
      }
    }

};
