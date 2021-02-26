
#include <Arduino.h>
#include "ArduinoLog.h"

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
#define pwm_left 8
#define dir_left 7

//right
#define pwm_right 4
#define dir_right 3


class Move
{
  private :
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

  public:

    Move()
    {
      pinMode(pwm_left, OUTPUT);
      pinMode(dir_left, OUTPUT);
      pinMode(pwm_right, OUTPUT);
      pinMode(dir_right, OUTPUT);
    }

    void process(char in[])
    {
      if (in[0] == 'M')
      {
        //Serial.print("move : ");
        //Serial.println(in);
        if (strstr((char*)in, "MFORWARD") != 0)
        {
          forward();
        }
        else if (strstr((char*)in, "MBACKWARD") != 0)
        {
          backward();
        }
        else if (strstr((char*)in, "MLEFT") != 0)
        {
          left();
        }
        else if (strstr((char*)in, "MRIGHT") != 0)
        {
          right();
        }
      }
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
        analogWrite(pwm_left, speedleft);
        analogWrite(pwm_right, speedright);
      }
    }

};
