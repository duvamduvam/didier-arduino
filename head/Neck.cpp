#include <Arduino.h>
#include "ArduinoLog.h"
#include <Servo.h>
#include "Commands.h"
#include "Fonctions.h"

#define SERVOMIN  0 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  180 // this is the 'maximum' pulse length count (out of 4096)
#define SERVO_PIN 6
#define MARGIN 5

class Neck {

  private :
    Servo servo;
    Command command;
    //char action[3] = {};
    int token = 0;
    int step = 3;
    int currentPos = 95;
    int targetPos = currentPos;

    void nextStep(int step) {
      if ((SERVOMIN <= currentPos) && (currentPos <= SERVOMAX)) {
        Log.notice ("position %d next step %d\n", currentPos, step);
        currentPos += step;
        servo.write(currentPos);
      }
    }

  public :
    //porgram don't work when initialized in constructor
    void init() {
      //Log.notice("init head servo\n");
      servo.attach(SERVO_PIN);
      //pwm.setPWM(0, 0, 220);
      delay(100);
      servo.write(currentPos);
    }

    void process(char in) {

      //int slider = GetIntAfterString(action, (char*)"N");
      targetPos = map(in, 33, 126, SERVOMIN, SERVOMAX);
      Log.notice("Neck next %i from : %s\n", targetPos, in);
    }

    // void updateTargetPos() {

    // }

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
      int diff = abs(targetPos - currentPos);
      if (diff > MARGIN && targetPos != currentPos) {
        if (targetPos > currentPos) {
          nextStep(1);
        } else {
          nextStep(-1);
        }
      }
    }


};
