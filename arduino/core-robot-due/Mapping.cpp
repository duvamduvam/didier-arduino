#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArduinoLog.h"
#include "Fonctions.h"
#include "Commands.h"

#define TABLE_SIZE 29



class Mapping {

  private:
    char* mapping[TABLE_SIZE];

  public :

    Mapping() {

      mapping[1] = (char*)"A42";
      mapping[2] = (char*)"WF";

      mapping[3] = (char*)"A43";
      mapping[4] = (char*)"WL";

      mapping[5] = (char*)"A49";
      mapping[6] = (char*)"WR";

      mapping[7] = (char*)"A48";
      mapping[8] = (char*)"WB";

      mapping[9] = (char*)"A40";
      mapping[10] = (char*)"L5";

      mapping[11] = (char*)"A47";
      mapping[12] = (char*)"F03";

      mapping[13] = (char*)"A40";
      mapping[14] = (char*)"F02";

      mapping[15] = (char*)"A41";
      mapping[16] = (char*)"L8";

      mapping[17] = (char*)"A27";
      mapping[18] = (char*)"L! LE+% |L! LE-% |L! LE-% ";

      mapping[19] = (char*)"A45";
      mapping[20] = (char*)"LE-";

      mapping[21] = (char*)"A47";
      mapping[22] = (char*)"LON";

      mapping[23] = (char*)"A39";
      mapping[24] = (char*)"LE+";

      mapping[25] = (char*)"A46";
      mapping[26] = (char*)"F01 ";

      mapping[27] = (char*)"A41";
      mapping[28] = (char*)"F00 ";

    }

    Commands getCommands(char* key)
    {
      //Log.notice("Mapping.cpp getComand 55 input : %s \n", key);
      Commands commands;
      char* copy = strtrim_safe(key);
      if (strcmp((char*)key, "") != 0) {


        //Joystick
        if (copy[0] == 'J') {

          byte bX = copy[1];
          byte bY = copy[2];
          SpeedVector robot = ToRobot(bX, bY);
          commands.leftSpeed = robot.left;
          commands.rightSpeed = robot.right;
          strcpy(commands.wheel, copy);
        }
        else {
          //Recherche dictionnaire (mapping)
          for (int i = 0; i < TABLE_SIZE - 1; i++) {
            if (strcmp(mapping[i], copy) == 0) {
              switch (mapping[i + 1][0]) {
                case 'W':
                  strcpy(commands.wheel, mapping[i + 1]);
                  break;
                case 'N':
                  strcpy(commands.neck, mapping[i + 1]);
                  //Log.notice("Light mapping command %s : mapping[i + 1]:%s commands.lights:%s\n", key, mapping[i + 1], commands.neck );
                  break;
                case 'L':
                  strcpy(commands.lights, mapping[i + 1]);
                  //Log.notice("Light mapping command %s : mapping[i + 1]:%s commands.lights:%s\n", key, mapping[i + 1], commands.lights );
                  break;
                case 'S':
                  strcpy(commands.sound, mapping[i + 1]);
                  break;
                case 'F':
                  strcpy(commands.face, mapping[i + 1]);
                  break;
              }
            }
          }
        }
      }
      return commands;
    }

    char* getValue(char* key) {
      char* copy = strtrim_safe(key);
      if (strcmp((char*)key, "") != 0) {
        //Log.notice("copy %s\n", copy);
        //Log.notice("copy : %s\n", copy);
        for (int i = 0; i < TABLE_SIZE - 1; i++) {
          Log.notice("mapping %d->%s\n", i, mapping[i]);
          if (strcmp(mapping[i], copy) == 0) {
            return mapping[i + 1];
          }
        }
      }
      return key;
    }

};
