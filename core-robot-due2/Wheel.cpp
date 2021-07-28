#include <Arduino.h>
#include "ArduinoLog.h"
#include "Commands.h"
#include "Fonctions.h"

//#define LOG_NOTICE_WHEEL

#define SPEED_MAX  1
#define SPEED_RATIO  1
#define SPEED_ACCELERATION  10 //2ms
#define INCREMENT_ACCELERATION  2 //2ms

#define FIRST_CHAR_NB 33

//left
#define pwm_left 3
#define dir_left 2

//right
#define pwm_right 4
#define dir_right 5

class Wheel
{
  private :

    Commands command;
    char action[2] = {};
    int speedleft;
    int speedright;

    float cmdLeft, cmdRight;
    float spLeft, spRight;

    int token = 0;

    long lastSpeedTc; //dernier timecode pour tempo acceleration


    // These maintain the current state
    unsigned int previousMillis;
    unsigned int TimeoutTimer;


  public:


    Wheel()
    {
      pinMode(pwm_left, OUTPUT);
      pinMode(dir_left, OUTPUT);
      pinMode(pwm_right, OUTPUT);
      pinMode(dir_right, OUTPUT);
    }

    /*void direct(char* input ) {
      action = input;
      }*/

    void process(Commands cmd) {

    }


    void process2(int left, int right) {

      if (left == 0 && right == 0) {
        return;
      }

      cmdLeft = map(left, FIRST_CHAR_NB, 126, -99, 99);
      cmdRight = map(right, FIRST_CHAR_NB, 126, -99, 99);

      Log.notice("Wheel left:%d right:%d ########\n", cmdLeft, cmdRight);

      updateInstruction();

      // TODO finish multi action move
      //command.set(action);
      //next();

    }

    void updateInstruction() {

      if (action[0] == 'W') {
        if (strcmp(action, "WF") == 0) {
          cmdLeft = SPEED_MAX;
          cmdRight = SPEED_MAX;
        }
        else if (strcmp(action, "WB") == 0) {
          cmdLeft = -SPEED_MAX;
          cmdRight = -SPEED_MAX;
        }
        else if (strcmp(action, "WL") == 0) {
          cmdLeft = -SPEED_MAX;
          cmdRight = SPEED_MAX;
        }
        else if (strcmp(action, "WR") == 0) {
          cmdLeft = SPEED_MAX;
          cmdRight = -SPEED_MAX;
        }
        else if (strcmp(action, "WS") == 0) {
          cmdLeft = 0;
          cmdRight = 0;
        }
      }
    }

    void stop()
    {
      cmdLeft = 0;
      cmdRight = 0;
    }

    void execute() {

      if (millis() - lastSpeedTc > SPEED_ACCELERATION) {
        lastSpeedTc = millis();

        float deltaL = cmdLeft - spLeft;
        float deltaR = cmdRight - spRight;


        int decCmdL = (int)(cmdLeft * 10);
        int decCmdR = (int)(cmdRight * 10);

#ifdef LOG_NOTICE_WHEEL
        Log.notice("JOY - L=%d - R=%d \n", decCmdL, decCmdR);
#endif
        if (abs(deltaL) > 0) {
          if (deltaL > 0) {
            spLeft += INCREMENT_ACCELERATION;
          } else {
            spLeft -= INCREMENT_ACCELERATION;
          }
        }
        if (abs(deltaR) > 0) {
          if (deltaR > 0) {
            spRight += INCREMENT_ACCELERATION;
          } else {
            spRight -= INCREMENT_ACCELERATION;
          }
        }
        int dL = 0;
        int dR = 0;
        if (spLeft > 0)
          dL = 1;
        if (spRight > 0)
          dR = 1;


        int decL = (int)(spLeft * 10);
        int decR = (int)(spRight * 10);

        // Log.notice("JOY - L=%d - R=%d \n", decL,decR);

        speedleft =  abs((int)(spLeft * SPEED_MAX));
        speedright = abs((int)(spRight * SPEED_MAX));

        if (speedleft < 10)
          speedleft = 0;
        if (speedright < 10)
          speedright = 0;

#ifdef LOG_NOTICE_WHEEL
        Log.notice("JOY - L=%d - R=%d \n", speedleft, speedright);
        Log.notice("JOY - CMD L=%d - R=%d | CUR L=%d - R=%d | DIR L=%d - R=%d \n", decCmdL, decCmdR, decL, decR, dL, dR);
#endif

        digitalWrite(dir_left, dL);
        digitalWrite(dir_right, dR);

        analogWrite(pwm_left, speedleft);
        analogWrite(pwm_right, speedright);
      }

      if (millis() - TimeoutTimer > 1000)
      {
        TimeoutTimer = millis();
        stop();
      }
    }

    /*
        void execute() {

          if (millis() - lastSpeedTc > SPEED_ACCELERATION) {
            lastSpeedTc = millis();

            int deltaL = cmdLeft - spLeft;
            int deltaR = cmdRight - spRight;


            int decCmdL = cmdLeft * 10;
            int decCmdR = cmdRight * 10;

            Log.notice("JOY - L=%d - R=%d - decCmdL=%d - decCmdR=%d - deltaL=%d - deltaR=%d \n", cmdLeft, cmdRight, decCmdL, decCmdR, deltaL, deltaR);

            if (deltaL > 0) {
              spLeft += INCREMENT_ACCELERATION;
            } else {
              spLeft -= INCREMENT_ACCELERATION;
            }
            if (deltaR > 0) {
              spRight += INCREMENT_ACCELERATION;
            } else {
              spRight -= INCREMENT_ACCELERATION;
            }
            int dL = 0;
            int dR = 0;
            if (spLeft > 0)
              dL = 1;
            if (spRight > 0)
              dR = 1;


            int decL = spLeft * 1000;
            int decR = spRight * 1000;

            Log.notice("JOY - spLeft=%d - spRight=%d \n", spLeft, spRight);

            speedleft =  spLeft * SPEED_MAX;
            speedright = spRight * SPEED_MAX;

            if (speedleft < 10)
              speedleft = 0;
            if (speedright < 10)
              speedright = 0;

      #ifdef LOG_NOTICE_WHEEL
            Log.notice("JOY - L=%d - R=%d \n", speedleft, speedright);
            Log.notice("JOY - CMD L=%d - R=%d | CUR L=%d - R=%d | DIR L=%d - R=%d \n", decCmdL, decCmdR, decL, decR, dL, dR);
      #endif

            digitalWrite(dir_left, dL);
            digitalWrite(dir_right, dR);

            analogWrite(pwm_left, speedleft);
            analogWrite(pwm_right, speedright);
          }

          if (millis() - TimeoutTimer > 1000)
          {
            TimeoutTimer = millis();
            stop();
          }
        }
    */
};
