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
  char* wheel;
  char* lights;
  char* neck;
  char* mouth;
  char* eyes;
  char* heart;
  char* sound;
};

typedef struct Commands;

class Command {
  private:

    char commands[10][10];
    int token;
    int nbToken;

  public:

    long attack;
    long finish;

    void set(char* command) {

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
    char nextCommand(int a1, int a2, int f1, int f2, int c) {
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
    }

    //extract attack time a1, a2 finish time f1, f2 command c1,c2
    char* nextCommand(byte a1, byte a2, byte f1, byte f2, byte c1, byte c2) {
      Log.notice("nextCommand token %i nbToken %i a1 %i a2 %i f1 %i f2 %i c1 %i c2 %i\n", token, nbToken, a1, a2, f1, f2, c1, c2);
      if ((token < nbToken) && commands[token] != 0) {
        char command[f2];
        strcpy(command, commands[token]);
        attack = millis() + extractTime(command, a1, a2);
        finish = millis() + extractTime(command, f1, f2);
        char action[c2 - c1];
        strcpy(action, extractChar(command, c1, c2));
        Log.notice("nextCommand[%i]:%s, millis %l attack %l finish %l action %s\n", token, command, millis(), attack, finish, action);
        token++;
        return action;
      }
      return 0;
    }

    bool doAttack() {
      return millis() >= attack;
    }

    bool doFinish() {
      return finish != 0 && millis() >= finish;
    }

    void getInputList(char* str)
    {
      //char* results[10];
      nbToken = 0;
      token = 0;
      char delim = '|'; //124
      int i = 0;
      int s = 0;
      int e = 0;
      bool hasDelim = false;

      Log.notice("getInputList in : %s\n", str);
      while (str[i] != 0) {
        if (str[i] == 124) {
          hasDelim = true;
          e = i - 1;
          strcpy( commands[nbToken], extractChar(str, s, e));
          Log.notice("getInputList token %i : %s\n", nbToken,  commands[nbToken]);
          nbToken++;
          s = i + 1;
        }
        i++;
      }
      strcpy( commands[nbToken], extractChar(str, s, arraySize(str)));
      nbToken++;
      Log.notice("getInputList token %i : %s\n", token,  commands[token]);
      //return results;
    }

};
