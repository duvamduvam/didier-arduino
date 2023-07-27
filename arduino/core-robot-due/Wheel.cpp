#include <Arduino.h>
#include "ArduinoLog.h"
#include "Commands.h"
#include "Fonctions.h"

#define LOG_NOTICE_WHEEL

#define SPEED_MAX  1.2
//#define SPEED_RATIO  1
#define SPEED_ACCELERATION  10 //2ms
#define INCREMENT_ACCELERATION  2 //2ms

#define FIRST_CHAR_NB 50

//left
#define pwm_left 3
#define dir_left 24

//right
#define pwm_right 4
#define dir_right 22

#define DIRECT_CMD 10

#define MAX_FORWARD 35
#define MAX_BACKWARD -5

#define TIMEOUT 2000

class Wheel
{
  private :

    Commands command;
    char action[2] = {};
    int speedleft;
    int speedright;
    bool stopped = true;

    float cmdLeft, cmdRight;
    float spLeft, spRight;

    int token = 0;

    long lastSpeedTc; //dernier timecode pour tempo acceleration


    // These maintain the current state
    unsigned int previousMillis;
    unsigned int timeoutTimer;


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

    void process(char* cmd) {
      
      if (!strcmp(cmd, "")) {
        return;
      }
      updateInstruction(cmd);
    }


    void process2(int left, int right) {

      if (left == 0 && right == 0) {
        return;
      }

      Log.notice("WHEEL LEFT %d RIGHT %d \n", int(left), int(right));

      cmdLeft = map(int(left), FIRST_CHAR_NB, 126, MAX_BACKWARD, MAX_FORWARD);
      cmdRight = map(int(right), FIRST_CHAR_NB, 126, MAX_BACKWARD, MAX_FORWARD);
      //Log.notice("JOY - cmdLeft=%d - cmdRight=%d \n", cmdLeft, cmdRight);
      lastSpeedTc = millis();
      timeoutTimer = millis();
      stopped = false;
      Serial.print("cmdLeft ");Serial.print(cmdLeft);Serial.print(" cmdRight ");Serial.println(cmdRight);
      //Log.notice("WHEEL CMD LEFT %d > CMD RIGHT %d \n", cmdLeft, cmdRight);

      // TODO finish multi action move
      //command.set(action);
      //next();

    }


    void process_pi(char* msg) {

      char left[2] = {};
      extractChar(left, msg, 1, 2);  

      char right[2] = {};
      extractChar(left, msg, 3, 4);  

      Log.notice("WHEEL LEFT %d RIGHT %d \n", int(left), int(right));

      cmdLeft = map(int(left), 0, 99, MAX_BACKWARD, MAX_FORWARD);
      cmdRight = map(int(right), 0, 99, MAX_BACKWARD, MAX_FORWARD);
      //Log.notice("JOY - cmdLeft=%d - cmdRight=%d \n", cmdLeft, cmdRight);
      lastSpeedTc = millis();
      timeoutTimer = millis();
      stopped = false;
      Serial.print("cmdLeft ");Serial.print(cmdLeft);Serial.print(" cmdRight ");Serial.println(cmdRight);
      //Log.notice("WHEEL CMD LEFT %d > CMD RIGHT %d \n", cmdLeft, cmdRight);

      // TODO finish multi action move
      //command.set(action);
      //next();

    }

    void updateInstruction(char* cmd) {

      if (cmd[0] == 'W') {
      Log.notice("Direct button command %s\n", cmd);
        
        if (strcmp(cmd, "WF") == 0) {
          cmdLeft = DIRECT_CMD;
          cmdRight = DIRECT_CMD;
        }
        else if (strcmp(cmd, "WB") == 0) {
          cmdLeft = -DIRECT_CMD;
          cmdRight = -DIRECT_CMD;
        }
        else if (strcmp(cmd, "WL") == 0) {
          cmdLeft = -DIRECT_CMD;
          cmdRight = DIRECT_CMD;
        }
        else if (strcmp(cmd, "WR") == 0) {
          cmdLeft = DIRECT_CMD;
          cmdRight = -DIRECT_CMD;
        }
        else if (strcmp(cmd, "WS") == 0) {
          cmdLeft = 0;
          cmdRight = 0;
        }

        digitalWrite(dir_left, (cmdLeft>0));
        digitalWrite(dir_right, (cmdRight>0));

        analogWrite(pwm_left, abs(cmdLeft));
        analogWrite(pwm_right, abs(cmdRight));

        
      }
    }

    void stop()
    {
      cmdLeft = 0;
      cmdRight = 0;
    }

    void execute() {

      if (millis() - lastSpeedTc > SPEED_ACCELERATION) {

        //Log.notice("millis %d - lastSpeedTc =%d > SPEED_ACCELERATION %d \n", millis(), lastSpeedTc, SPEED_ACCELERATION);

        //Serial.print("cmdLeft ");Serial.print(cmdLeft);Serial.print(" cmdRight ");Serial.println(cmdRight);

        float deltaL = cmdLeft - spLeft;
        float deltaR = cmdRight - spRight;


        int decCmdL = (int)(cmdLeft * 10);
        int decCmdR = (int)(cmdRight * 10);

        //Log.notice("JOY - decCmdL=%d - decCmdR=%d \n", decCmdL, decCmdR);
        
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
  
        //Log.notice("JOY - speedleft=%d - speedright=%d \n", speedleft, speedright);
        //Log.notice("JOY - CMD L=%d - R=%d | CUR L=%d - R=%d | DIR L=%d - R=%d \n", decCmdL, decCmdR, decL, decR, dL, dR);

        digitalWrite(dir_left, dL);
        digitalWrite(dir_right, dR);

        analogWrite(pwm_left, speedleft);
        analogWrite(pwm_right, speedright);
      }

      if (!stopped && (millis() - timeoutTimer > TIMEOUT))
      {
        Log.notice("WHEEL stopped \n");
        stopped = true;
        stop();
      }
    }


};
