#include <Arduino.h>
#include "ArduinoLog.h"

//bool debug = false;
//Utils utils(debug);

//Modes...
#define STOP     0
#define FORWARD  1
#define BACKWARD 2
#define LEFT     3
#define RIGHT    4

#define SPEED_MAX_LEFT  1000
#define SPEED_MAX_RIGHT  1000
#define SPEED_ACCELERATION  1 //1ms


class Move 
{

    //left
#define pwm_left 0
#define dir_left 1

    //right
#define pwm_right 4
#define dir_right 5





    int speedleft = 1000;
    int speedright = 1000;

    int runningMode;
    int consigneMode;
    long lastSpeedTc; //dernier timecode pour tempo acceleration
    bool mouvement;   
    bool accelerationEnCours;
    bool stoped;

  public:

    // Class Member Variables
    // These are initialized at startup
    int ledPin;      // the number of the LED pin
    long OnTime;     // milliseconds of on-time
    long OffTime;    // milliseconds of off-time

    // These maintain the current state
    unsigned long previousMillis;  	// will store last time LED was updated

    Move() 
    {

      //utils = utils(d);
      pinMode(pwm_left, OUTPUT);
      pinMode(dir_left, OUTPUT);
      pinMode(pwm_right, OUTPUT);
      pinMode(dir_right, OUTPUT);
    }

    void process(char in[]) 
    {
      //Serial.print("move : ");
      //Serial.println(in);
      if (strstr((char*)in, "A37") != 0) 
      {
        forward();
      } 
      else if (strstr((char*)in, "A31") != 0) 
      {
        backward();
      } 
      else if (strstr((char*)in, "A36") != 0) 
      {
        left();
      } 
      else if (strstr((char*)in, "A30") != 0) 
      {
        right();
      }
    }

    void forward() {
      Log.notice("forward\n");
      consigneMode = FORWARD;
      
    }
    void backward() {
      Log.notice("backward\n");
      consigneMode = BACKWARD;
      
    }
    void left() {
      Log.notice("left\n");
      consigneMode = LEFT;

    }
    void right() {
      Log.notice("right\n");
      consigneMode = RIGHT;

      
    }
    void stop() 
    {
      consigneMode = STOP;
      accelerationEnCours=true;

    }
    
    void execute() 
    {
      
      
      if (millis() - lastSpeedTc > SPEED_ACCELERATION) 
      {
        lastSpeedTc = millis();
        
        mouvement=false;
        if (runningMode == STOP)
        {
          if (speedleft>0)
          {
            speedleft--;
            mouvement=true;
          }
          if (speedright>0)
          {
            speedright--;
            mouvement=true;
          }
        }
        else
        {
          if (speedleft<SPEED_MAX_LEFT)
          {
            speedleft++;
            mouvement=true;
          }
          if (speedright<SPEED_MAX_RIGHT)
          {
            speedright++;
            mouvement=true;
          }
        }
      }

    //Attends la fin de l'acceleration pour la nouvelle consigne
    if (mouvement == false);
      runningMode = consigneMode;

      
        switch(runningMode)
        {
          case FORWARD:
            digitalWrite(dir_left, 1);
            digitalWrite(dir_right, 1);
            break;
            
          case BACKWARD:
            digitalWrite(dir_left, 0);
            digitalWrite(dir_right, 0);
            break;
            
          case LEFT:
            digitalWrite(dir_left, 1);
            digitalWrite(dir_right, 0);
            break;
            
          case RIGHT:
            digitalWrite(dir_left, 0);
            digitalWrite(dir_right, 1);
            break;

        }
        analogWrite(pwm_left, speedleft);
        analogWrite(pwm_right, speedright);


    }
    
};
