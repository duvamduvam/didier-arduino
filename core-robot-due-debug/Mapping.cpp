#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArduinoLog.h"
#include "Fonctions.h"
#include "Commands.h"

#define TABLE_SIZE 23

class Mapping {

  private:
    char* mapping[TABLE_SIZE];

  public :

    Mapping() {

      mapping[1] = (char*)"XG";
      mapping[2] = (char*)"WF";

      mapping[3] = (char*)"XF";
      mapping[4] = (char*)"WL";

      mapping[5] = (char*)"XH";
      mapping[6] = (char*)"WR";

      mapping[7] = (char*)"XO";
      mapping[8] = (char*)"WB";

      mapping[9] = (char*)"QOO";
      mapping[10] = (char*)"WS";

      mapping[11] = (char*)"A08";
      mapping[12] = (char*)"MRIGHT";

      mapping[13] = (char*)"XBB";
      mapping[14] = (char*)"HLEFT";

      mapping[15] = (char*)"XCC";
      mapping[16] = (char*)"HRIGHT";

      mapping[17] = (char*)"HU";
      mapping[18] = (char*)"L! LE+% |L! LE-% ";
      //mapping[18] = (char*)"S! 123% |S! 321% ";

      mapping[19] = (char*)"HTT";
      mapping[20] = (char*)"LE-";

      mapping[21] = (char*)"HQQ";
      mapping[22] = (char*)"S! 123% ";

      mapping[23] = (char*)"HQQ";
      mapping[24] = (char*)"S! 123% S! 321% ";

    }

    Commands getCommands(char* key)
    {
      Commands commands;

      Log.notice("Mapping.cpp getComand 55 input : %s \n", key);
      if (strcmp((char*)key, "") != 0) {

        //Recherche dictionnaire (mapping)
        for (int i = 0; i < TABLE_SIZE - 1; i++)
        {
          if (strcmp(mapping[i], key) == 0) {
            switch (mapping[i + 1][0])
            {
              case 'W':
                strcpy(commands.wheel, mapping[i + 1]);
                break;
              case 'N':
                strcpy(commands.neck, mapping[i + 1]);
                break;
              case 'L':
                strcpy(commands.lights, mapping[i + 1]);
                Log.notice("Light mapping command %s : mapping[i + 1]:%s commands.lights:%s\n", key, mapping[i + 1], commands.lights );
                break;
              case 'S':
                Log.notice("Mapping load sound process");
                //remove first char
                strcpy(commands.sound, mapping[i + 1]);
                break;
              case 'M':
                strcpy(commands.mouth, mapping[i + 1]);
                break;
              case 'E':
                strcpy(commands.eyes, mapping[i + 1]);
                break;
              case 'H':
                strcpy(commands.heart, mapping[i + 1]);
                break;
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
