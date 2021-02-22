#include <Arduino.h>
#include "ArduinoLog.h"
#include "Commands.h"


#define SPEED_MAX  255
#define SPEED_RATIO  1
#define SPEED_ACCELERATION  200 //2ms
#define INCREMENT_ACCELERATION  2 //2ms

//left
#define pwm_left 4
#define dir_left 5

//right
#define pwm_right 3
#define dir_right 2


class Move
{
  private :

    Command command;
    int speedleft;
    int speedright;
    
    float cmdLeft,cmdRight;
    float spLeft,spRight;

    long lastSpeedTc; //dernier timecode pour tempo acceleration

//    int ledPin;      // the number of the LED pin
//    long OnTime;     // milliseconds of on-time
//    long OffTime;    // milliseconds of off-time

    // These maintain the current state
    unsigned long previousMillis;    // will store last time LED was updated

    char* action;

  public:

    Move()
    {
      pinMode(pwm_left, OUTPUT);
      pinMode(dir_left, OUTPUT);
      pinMode(pwm_right, OUTPUT);
      pinMode(dir_right, OUTPUT);
    }

    void direct(char* input ) {
      action = input;
    }

    void process(Commands cmd) {

      char* action = cmd.wheel;
      if (strcmp((char*)action, "") != 0){
        
        Log.notice("Move.h 57 move process %s\n", action);
  
        if (strcmp(action, "WF") == 0){
          cmdLeft = SPEED_MAX;
          cmdRight = SPEED_MAX;
        }
        else if (strcmp(action, "WB") == 0){
          cmdLeft = -SPEED_MAX;
          cmdRight = -SPEED_MAX;
        }
        else if (strcmp(action, "WL") == 0){
          cmdLeft = -SPEED_MAX;
          cmdRight = SPEED_MAX;
        }
        else if (strcmp(action, "WR") == 0){
          cmdLeft = SPEED_MAX;
          cmdRight = -SPEED_MAX;
        }
        else if (strcmp(action, "WS") == 0){
          cmdLeft = 0;
          cmdRight = 0;
        } 
      }
      else {
        cmdLeft = cmd.leftSpeed;
        cmdRight = cmd.rightSpeed;
      }


      
      // TODO finish multi action move
      //command.set(action);
      //next();
    }

    void stop()
    {
      cmdLeft = 0;
      cmdRight = 0;
    }

    
    //input structure char[0,1] -> attack, char[2,3,4] -> music, char[5,6] ex : "+ 123? "
    void next() {
      command.nextCommand(action, 0, 1, 4, 5, 2, 3);
    }


    void execute()
    {
      if (millis() - lastSpeedTc > SPEED_ACCELERATION)
      {
        lastSpeedTc = millis();


        float deltaL = cmdLeft - spLeft;
        float deltaR = cmdRight - spRight;

        if (abs(deltaL)>0)
          if (deltaL>0)
            spLeft+=INCREMENT_ACCELERATION;
          else
            spLeft-=INCREMENT_ACCELERATION;
            
        if (abs(deltaR)>0)
          if (deltaR>0)
            spRight+=INCREMENT_ACCELERATION;
          else
            spRight-=INCREMENT_ACCELERATION;

            bool dL,dR;
        if (spLeft>0)
          dL=1;
         if (spRight>0)
          dR=1;
          
        speedleft =  (int)abs(spLeft * SPEED_MAX);
        speedright = (int)abs(spRight * SPEED_MAX);

        Log.notice("JOY - L=%d - R=%d \n", speedleft,speedright);

       

        digitalWrite(dir_left, dL);
        digitalWrite(dir_right, dR);

   //     analogWrite(pwm_left, speedleft);
     //   analogWrite(pwm_right, speedright);
      }
    }

};
