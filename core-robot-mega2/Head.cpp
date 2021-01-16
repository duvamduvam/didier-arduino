#include <Arduino.h>
#include "ArduinoLog.h"
#include <Adafruit_PWMServoDriver.h>
#include "Commands.h"

#define SERVOMIN  80 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  640 // this is the 'maximum' pulse length count (out of 4096)

class Head {

  private :
    Command command;
    Adafruit_PWMServoDriver pwm;
    int step = 30;
    int currentPos = 400;
    int pos = 0;

    int nextStep(int step)
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

    void process(char* in) {
      command.set(in);
      next();
    }

    void next() {
      char action[2];
      strcpy(action, command.nextCommand(0, 1, 4, 5, 2, 3));
      pos = ((action[0] - 32) * 100) + (action[1] - 32);
      Log.notice("Head next %s %i", action, pos);
    }

    void execute() {

      if (pos != 0) {
        //manuel mouvement
        if (pos == 1) {
          nextStep(step);
        } else if (pos == -1) {
          nextStep(-step);
        } else {
          //automatic mouvement
          if (pos == currentPos) {
            if (command.hasNext()) {
              next();
            } else {
              pos = 0;
            }
          } else if (pos > currentPos) {
            nextStep(1);
          } else {
            nextStep(-1);
          }
        }
      }
    }


};
