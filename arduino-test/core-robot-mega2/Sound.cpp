#include <Arduino.h>
#include "ArduinoLog.h"
#include "Fonctions.h"
#include "Commands.h"

class Sound {

  private :
    Command command;
    char music[3];
    bool playing = false;

  public :

    Sound() {
      Log.notice("init sound player\n");
      Serial3.begin(115200);
    }


    void process(char* in) {
      Log.notice("sound process %s\n", in);
      if (arraySize(in) > 2) {
        command.set(in);
        next();
      }
    }

    //input structure char[0,1] -> attack, char[2,3,4] -> music, char[5,6] ex : "+ 123? "
    void next() {
      strcpy(music, command.nextCommand(0, 1, 5, 6, 2, 4));
    }

    void execute() {
      if (Serial3.available()) {
        char in = Serial.read();
        if (in == 'S') {
          playing = false;
          next();
        }
      }

      if (strcmp(music, "")) {
        if (playing) {
          if (command.doFinish()) {
            Serial3.println('S');
            Log.notice("stop playing %s\n", music);
            playing = false;
            if (command.hasNext()) {
              next();
            } else {
              strcpy(music, "");
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
