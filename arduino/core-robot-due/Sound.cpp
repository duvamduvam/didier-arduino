#include <Arduino.h>
#include "ArduinoLog.h"
#include "Fonctions.h"
#include "Commands.h"

class Sound {

  private :
    Command command;
    char music[3];
    int token = 0;
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
      if (in[0] == 'S' && (millis() - lastInput) > INPUT_INTERVAL) {
        if (arraySize(in) > 2) {
          token = 0;
          lastInput = millis();
          command.set(in);
          getCommand();
          Log.notice("######## Sound process input:\"%s\" action[%d] : \"%s\" ########\n", in, token, music);
        } else {
          Log.notice("sound process already typed%s\n", in);
        }
      }
    }

    //input structure char[0,1] -> attack, char[2,3,4] -> music, char[5,6] ex : "+ 123? "
    boolean next() {
      token++;
      if (token < command.nbToken) {
        getCommand();
        return true;
      }
      return false;
    }

    void getCommand() {
      command.getCommand(token, music, 1, 2, 6, 7, 3, 5);
    }

    void execute() {
      if (strcmp(music, "")) {
        //Log.notice("Sound music : %s\n", music);
        if (playing) {
          if (command.doFinish()) {
            Serial3.println('S');
            Log.notice("stop playing sound %s\n", music);
            playing = false;
            if (!next()) {
              strcpy(music, "");
            }
          }
        } else {
          if (command.doAttack()) {
            Serial3.println(music);
            Log.notice("start playing sound %s\n", music);
            playing = true;
          }
        }
      }
    }
};
