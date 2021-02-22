#include <Arduino.h>
#include "ArduinoLog.h"
#include "Fonctions.h"
#include "Commands.h"

class Sound {

  private :
    Command command;
    char music[3];
    bool playing = false;

    long lastInput = 0;
    const int INPUT_INTERVAL = 300;

  public :

    Sound() {
      Log.notice("init sound player\n");
      Serial3.begin(115200);
    }


    void process(char* in) {

      //avoid continous fire button
      if ((millis() - lastInput) > INPUT_INTERVAL) {
        Log.notice("sound process %s\n", in);

        if (arraySize(in) > 2) {
          lastInput = millis();
          command.set(in);
          next();
        } else {
          Log.notice("sound process already typed%s\n", in);
        }
      }
    }

    //input structure char[0,1] -> attack, char[2,3,4] -> music, char[5,6] ex : "+ 123? "
    void next() {
      command.nextCommand(music, 1, 2, 6, 7, 3, 5);
    }

    void execute() {

      if (strcmp(music, "")) {
        Log.notice("Sound music : %s\n", music);

        if (playing) {
          if (command.doFinish()) {
            Serial3.println('S');
            Log.notice("stop playing %s\n", music);
            playing = false;
            if (command.hasNext()) {
              next();
            } else {
              strcpy(music, "");
              playing = false;
            }
          }
        } else {
          if (command.doAttack()) {
            Serial3.println(music);
            Log.notice("start playing %s\n", music);
            playing = true;
          }
        }
      }
    }
};
