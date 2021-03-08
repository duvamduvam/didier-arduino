#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArduinoLog.h"
#include "Fonctions.h"
#include "Commands.h"

#define TABLE_SIZE 33

#define LOG_NOTICE_MAPPING

class Mapping {

  private:
    char* mapping[TABLE_SIZE];

  public :

    Mapping() {

      mapping[1] = (char*)"WG";
      mapping[2] = (char*)"WF";

      mapping[3] = (char*)"WH";
      mapping[4] = (char*)"WL";

      mapping[5] = (char*)"WO";
      mapping[6] = (char*)"WR";

      mapping[7] = (char*)"WP";
      mapping[8] = (char*)"WB";

      mapping[9] = (char*)"XQ";
      mapping[10] = (char*)"LE+";

      mapping[11] = (char*)"XR";
      mapping[12] = (char*)"LE-";

      mapping[13] = (char*)"XS";
      mapping[14] = (char*)"LON";

      mapping[15] = (char*)"XA";
      mapping[16] = (char*)"F01";

      mapping[17] = (char*)"XB";
      mapping[18] = (char*)"F00";
      //mapping[18] = (char*)"L! LE+% |L! LE-% |L! LE-% ";

      mapping[19] = (char*)"XC";
      mapping[20] = (char*)"F02";

      mapping[21] = (char*)"";
      mapping[22] = (char*)"";

      mapping[23] = (char*)"";
      mapping[24] = (char*)"";

      mapping[25] = (char*)"XI";
      mapping[26] = (char*)"N+";

      mapping[27] = (char*)"XJ";
      mapping[28] = (char*)"N-";

      mapping[29] = (char*)"AAA";
      mapping[30] = (char*)"N+";

      mapping[31] = (char*)"BBB";
      mapping[32] = (char*)"N-";

    }


    Commands getCommands(char* key)
    {

      //Log.notice("Mapping.cpp getComand 55 input : %s \n", key);
      Commands commands;
      char* copy = strtrim_safe(key);
      if (strcmp((char*)key, "") != 0) {

        char prefixe = copy[0];

        switch (prefixe)
        {
          case 'W':  int nL, nR;
            ParseVectorString(copy, &nL, &nR);
            commands.leftSpeed = (float)nL / 100;
            commands.rightSpeed = (float)nR / 100;
            strcpy(commands.wheel, copy);
            Log.notice("Wheels - L=%d - R=%d \n", nL, nR);
            break;
            
          case 'N':

            int val;
            ParseIntString(copy, &val);
            Log.notice("Neck - V=%d \n", val);
            break;

          default :
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
            break;

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

#ifdef LOG_NOTICE_MAPPING
          Log.notice("mapping %d->%s\n", i, mapping[i]);
#endif

          if (strcmp(mapping[i], copy) == 0) {
            return mapping[i + 1];
          }
        }
      }
      return key;
    }

};
