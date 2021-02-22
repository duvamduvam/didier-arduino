#pragma once

//TODO -> attack ?

// move wheel prefix W, char 2 roue gauche, char 3 roue droite, char 4 temps |
// nake prefix H char 2 position, char 3 et 4 temps |
// lumière prefix L char 2 mod char 3 couleur char 4 et 5 temps |
// mouth prefix S char 2 mod char 4 et 5 temps
// eyes prefix E char 2 mod char 4 et 5
// heart prefix H char 2 mod char 4 et 5 temps

#include <Arduino.h>
#include "Fonctions.h"

struct Commands
{
  char wheel[20] = {};
  char lights[20] = {};
  char neck[20] = {};
  char mouth[20] = {};
  char eyes[20] = {};
  char heart[20] = {};
  char sound[20] = {};

  float leftSpeed;
  float rightSpeed;
};

typedef struct Commands;

class Command {
  private:

    char commands[10][10];
    int nbToken;

  public:

    unsigned  long attack;
    long finish;
    int token;

    /* Setup commands list

    */
    void set(char* command) {
      Log.notice("Commands set \"%s\" sizeof: %d\n", command, arraySize(command));
      if (arraySize(command) >= 5) {
        getInputList(command);
      } else {
        token = 0;
        nbToken = 1;
        strcpy(commands[0], command);
      }
    }

    boolean hasNext() {
      /*Log.notice("hasNext current token %i < nbToken %i ", token, nbToken);
        if(token < nbToken){
        Log.notice("true\n");
        }else{
        Log.notice("false\n");
        }*/
      return (token < nbToken);
    }

    //a1 attack second a2 attack minutes f1 finish second f2 finish minutes c command
    /*char nextCommand(int a1, int a2, int f1, int f2, int c) {
      Log.notice("nextCommand a1 %i a2 %i f1 %i f2 %i c %i\n", a1, a2, f1, f2, c);
      if ((token < nbToken) && commands[token] != 0) {
        char *command = commands[token];
        attack = millis() + extractTime(command, a1, a2);
        finish = millis() + extractTime(command, f1, f2);
        Log.notice("nextCommand %i %s, millis %l attack %l finish %l command %c\n", token, command, millis(), attack, finish, command[c]);
        token++;
        return command[c];
      }
      return 0;
      }*/

    //extract attack time a1, a2 finish time f1, f2 command c1,c2
   void nextCommand(char* action, byte a1, byte a2, byte f1, byte f2, byte c1, byte c2) {
      Log.notice("nextCommand:%s token:%d nbToken:%d attack start:%d attack end:%d release start:%d release end:%d command begin :%d command end:%d\n", commands[token], token, nbToken, a1, a2, f1, f2, c1, c2);
      if ((token < nbToken)) {
        char command[10];
        strcpy(command, commands[token]);
        attack = millis() + extractTime(command, a1, a2);
        finish = millis() + extractTime(command, f1, f2);
        //char action[c2 - c1];
        extractChar(action, command, c1, c2);
        Log.notice("nextCommand[%d]:%s, millis %d attack %d finish %d action %s\n", token, command, millis(), attack, finish, action);
        token++;
      }
    }

    //extract attack time a1, a2 finish time f1, f2 command c1,c2
    /*String* nextCommand2(byte a1, byte a2, byte f1, byte f2, byte c1, byte c2) {
      Log.notice("nextCommand:%s token:%d nbToken:%d attack start:%d attack end:%d release start:%d release end:%d command begin :%d command end:%d\n", commands[token], token, nbToken, a1, a2, f1, f2, c1, c2);
      if ((token < nbToken)) {
        char command[token];
        strcpy(command, commands[token]);
        attack = millis() + extractTime(command, a1, a2);
        finish = millis() + extractTime(command, f1, f2);
        String action =  command.substring(C1, C2);
        Log.notice("nextCommand[%d]:%s, millis %d attack %d finish %d action %s\n", token, command, millis(), attack, finish, action);
        token++;
        return action;
      }
      return 0;
      }*/

    bool doAttack() {
      return millis() >= attack;
    }

    bool doFinish() {
      return finish != 0 && millis() >= finish;
    }

    void getInputList(char* str)
    {
      nbToken = 0;
      char delim = '|'; //124
      int i = 0;
      int s = 0;
      int e = 0;
      bool hasDelim = false;

      Log.notice("getInputList in : %s\n", str);
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
      nbToken++;
      Log.notice("getInputList token %i : %s\n", token,  commands[token]);
      //return results;
    }

};
