#include <Arduino.h>
#include "ArduinoLog.h"
#include "Commands.h"

//Modes
#define STOP     0
#define FORWARD  1
#define BACKWARD 2
#define LEFT     3
#define RIGHT    4

#define SPEED_MAX_LEFT  150
#define SPEED_MAX_RIGHT  150
#define SPEED_ACCELERATION  10 //1ms

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

    int runningMode;
    int consigneMode;
    long lastSpeedTc; //dernier timecode pour tempo acceleration
    bool mouvement;

    int ledPin;      // the number of the LED pin
    long OnTime;     // milliseconds of on-time
    long OffTime;    // milliseconds of off-time

    // These maintain the current state
    unsigned long previousMillis;    // will store last time LED was updated

    char action[2];

  public:

    Move()
    {
      pinMode(pwm_left, OUTPUT);
      pinMode(dir_left, OUTPUT);
      pinMode(pwm_right, OUTPUT);
      pinMode(dir_right, OUTPUT);
    }

    void process(char* action) {
      Log.notice("Move.h 57 move process %s\n", action);
      if (strcmp(action, "WF") == 0)
      {
        forward();
      }
      else if (strcmp(action, "WB") == 0)
      {
        backward();
      }
      else if (strcmp(action, "WL") == 0)
      {
        left();
      }
      else if (strcmp(action, "WR") == 0)
      {
        right();
      } else if (strcmp(action, "WS") == 0)
      {
        stop();
      } else {
        //command joystick
      }

      // TODO finish multi action move
      //command.set(action);
      //next();
    }
    //input structure char[0,1] -> attack, char[2,3,4] -> music, char[5,6] ex : "+ 123? "
    void next() {
      strcpy(action, command.nextCommand(0, 1, 4, 5, 2, 3));
    }

    void forward()
    {
      Log.notice("forward\n");
      if (runningMode != FORWARD)
        runningMode = STOP;
      consigneMode = FORWARD;
    }

    void backward()
    {
      Log.notice("backward\n");
      if (runningMode != BACKWARD)
        runningMode = STOP;
      consigneMode = BACKWARD;
    }

    void left()
    {
      Log.notice("left\n");
      if (runningMode != LEFT)
        runningMode = STOP;
      consigneMode = LEFT;
    }

    void right()
    {
      Log.notice("right\n");
      if (runningMode != RIGHT)
        runningMode = STOP;
      consigneMode = RIGHT;
    }

    void stop()
    {
      if (speedleft > 0 || speedright > 0)
      {
        Log.notice("stop\n");
        runningMode = STOP;
        consigneMode = STOP;
      }
    }

    void execute()
    {
      if (millis() - lastSpeedTc > SPEED_ACCELERATION)
      {
        lastSpeedTc = millis();

        if (runningMode == STOP)
        {
          if (speedleft > 0)
            speedleft -= 10;
          if (speedright > 0)
            speedright -= 10;
        }
        else
        {
          if (speedleft < SPEED_MAX_LEFT)
            speedleft += 10;
          if (speedright < SPEED_MAX_RIGHT)
            speedright += 10;
        }

        if (speedleft < 0)
          speedleft = 0;
        if (speedright < 0)
          speedright = 0;
        if (speedleft > SPEED_MAX_LEFT)
          speedleft = SPEED_MAX_LEFT;
        if (speedright > SPEED_MAX_RIGHT)
          speedright = SPEED_MAX_LEFT;

        //Attends la fin de l'acceleration pour la nouvelle consigne
        if (speedleft == 0 && speedright == 0)
        {
          runningMode = consigneMode;

          switch (runningMode)
          {
            case FORWARD:
              digitalWrite(dir_left, 1);
              digitalWrite(dir_right, 1);
              Log.notice("change forward\n");
              break;

            case BACKWARD:
              digitalWrite(dir_left, 0);
              digitalWrite(dir_right, 0);
              Log.notice("change backward\n");
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
        }
        Log.notice("Move.cpp 188 pwm_left %i speed %i\n", pwm_left, speedleft);
        Log.notice("Move.cpp 189 pwm_right %i speed %i\n", pwm_left, speedleft);
        analogWrite(pwm_left, speedleft);
        analogWrite(pwm_right, speedright);
      }
    }

};
