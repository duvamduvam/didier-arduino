#include <Arduino.h>
#include "ArduinoLog.h"
#include <Adafruit_PWMServoDriver.h>
#include "Commands.h"

#define SERVOMIN  80 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  640 // this is the 'maximum' pulse length count (out of 4096)

class Neck {

  private :
    Command command;
    char action[3] = {};
    int token = 0;
    Adafruit_PWMServoDriver pwm;
    int step = 30;
    int currentPos = 400;
    int targetPos = currentPos;

    int nextStep(int step)
    {
      if (SERVOMIN >= currentPos <= SERVOMAX)
      {
        Log.notice ("position %d next step %d\n", currentPos, step);
        currentPos += step;
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
      //pwm.setPWM(0, 0, 220);
      delay(100);
      pwm.setPWM(0, 0, currentPos);
    }

    void process(char* in) {
      if (in[0] == 'N') {

        strcpy(action, in);

        /*
                   token = 0;
          command.set(in);
          getCommand();
        */
        updateTargetPos();
        Log.notice("######## Neck process input:\"%s\" action[%d]:\"%s\" currentPos:%d targetPos:%d ########\n", in, token, action, currentPos, targetPos);
      }
    }

    boolean next() {
      token++;
      if (token < command.nbToken) {
        getCommand();
        updateTargetPos();
        Log.notice("Neck update token:%d nbToken:%d targetPos:%d currentPos:%d\n",  token, command.nbToken, targetPos, currentPos);
        return true;
      }
      return false;
    }

    void updateTargetPos() {
      if (strcmp(action, "N+")) {
        targetPos += 10;
        Log.notice("Neck +10\n");
      } else if (strcmp(action, "N-")) {
        Log.notice("Neck -10\n");
        targetPos -= 10;
      } else {
        targetPos = ((action[0] - 32) * 100) + (action[1] - 32);
      }
      Log.notice("Neck next %s %i\n", action, targetPos);
    }

    void getCommand() {
      command.getCommand(token, action, 1, 2, 5, 6, 3, 4);
    }


    void execute() {

      //if (command.doFinish()) {
      //  if (!next()) {
      //TODO come back to the middle
      //  }
      //}
      //if (command.doAttack()) {
      moveHead();
      //}


    }

    void moveHead() {

      if (targetPos != currentPos) {
        if (targetPos > currentPos) {
          nextStep(1);
        } else {
          nextStep(-1);
        }
      }
    }


};
