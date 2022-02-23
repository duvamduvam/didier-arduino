//TODO -> attack ?

// move wheel prefix W, char 2 roue gauche, char 3 roue droite, char 4 temps |
// nake prefix H char 2 position, char 3 et 4 temps |
// lumière prefix L char 2 mod char 3 couleur char 4 et 5 temps |
// mouth prefix S char 2 mod char 4 et 5 temps
// eyes prefix E char 2 mod char 4 et 5
// heart prefix H char 2 mod char 4 et 5 temps

#include "ArduinoLog.h"
#include <Arduino.h>
#include "Fonctions.h"

#include "Commands.h"


    void Command::set(char* command) {
      if (arraySize(command) >= 5) {
        getInputList(command);
      } else {
        nbToken = 1;
        strcpy(commands[0], command);
      }
    }

    void Command::getCommand(int token, char* action, byte a1, byte a2, byte f1, byte f2, byte c1, byte c2) {
      //Log.notice("getCommand:%s token:%d nbToken:%d attack start:%d end:%d release start:%d end:%d command begin :%d end:%d\n", commands[token], token, nbToken, a1, a2, f1, f2, c1, c2);
      char command[10];
      strcpy(command, commands[token]);
      attack = millis() + extractTime(command, a1, a2);
      finish = millis() + extractTime(command, f1, f2);
      //char action[c2 - c1];
      extractChar(action, command, c1, c2);
      //Log.notice("command[%d]:\"%s\" millis %d attack %d finish %d action %s\n", token, command, millis(), attack, finish, action);
    }

    bool Command::doAttack() {
      return millis() >= attack;
    }

    bool Command::doFinish() {
      return finish != 0 && millis() >= finish;
    }


    void Command::getInputList(char* str)
    {
      nbToken = 0;
      char delim = '|'; //124
      int i = 0;
      int s = 0;
      int e = 0;
      bool hasDelim = false;

      //Log.notice("getInputList in : %s\n", str);
      while (str[i] != 0) {
        if (str[i] == 124) { // | delim
          hasDelim = true;
          e = i - 1;
          extractChar(commands[nbToken], str, s, e);
          Log.notice("getInputList token %i : %s\n", nbToken,  commands[nbToken]);
          nbToken++;
          s = i + 1;
        }
        i++;
      }
      extractChar(commands[nbToken], str, s, arraySize(str));
      Log.notice("getInputList token %i : %s\n", nbToken,  commands[nbToken]);
      nbToken++;

    }

