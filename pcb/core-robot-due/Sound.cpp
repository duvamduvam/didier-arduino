#include <Arduino.h>
#include "ArduinoLog.h"
#include "Fonctions.h"
#include "Commands.h"

class Sound {

  private :
    Command command;
    int music;
    int token = 0;
    bool playing = false;

    long lastInput = 0;
    unsigned const int INPUT_INTERVAL = 300;

  public :

    Sound() {
      Log.notice("init sound player\n");
      Serial2.begin(115200);
    }

    void process(char* in) {

      if (!strcmp(in, "")) {
        return;
      }

      extractChar(in, in, 1, 2);
      in[2] = 0;
      music = atoi(in);

      //avoid continous fire button
      //if (in[0] == 'S' && (millis() - lastInput) > INPUT_INTERVAL) {
      //if (arraySize(in) > 2) {

      Log.notice("######## Sound process input:\"%s\" music : \"%d\" ########\n", in,  music);
      Serial2.println(music);
      //}
    }

    //input structure char[0,1] -> attack, char[2,3,4] -> music, char[5,6] ex : "+ 123? "
    /*boolean next() {
      token++;
      if (token < command.nbToken) {
        getCommand();
        return true;
      }
      return false;
      }*/

    void execute() {
      //delay(1000);
      //Serial2.println("test");
    }
};
