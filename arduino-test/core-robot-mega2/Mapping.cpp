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

      mapping[1] = (char*)"A03";
      mapping[2] = (char*)"MFORWARD";

      mapping[3] = (char*)"A04";
      mapping[4] = (char*)"MBACKWARD";

      mapping[5] = (char*)"A05";
      mapping[6] = (char*)"HLEFT";

      mapping[7] = (char*)"A06";
      mapping[8] = (char*)"HRIGHT";

      mapping[9] = (char*)"A07";
      mapping[10] = (char*)"MLEFT";

      mapping[11] = (char*)"A08";
      mapping[12] = (char*)"MRIGHT";

      mapping[13] = (char*)"a";
      mapping[14] = (char*)"HLEFT";

      mapping[15] = (char*)"z";
      mapping[16] = (char*)"HRIGHT";

      mapping[17] = (char*)"A01";
      mapping[18] = (char*)"LE+";

      mapping[19] = (char*)"A02";
      mapping[20] = (char*)"LE-";

      mapping[21] = (char*)"A20";
      mapping[22] = (char*)"S! 123% ";

    }

    Commands getCommands(char* key) {
      Commands commands;
      char* copy = strtrim_safe(key);
      if (strcmp((char*)key, "") != 0) {
        //Log.notice("copy %s\n", copy);
        //Log.notice("copy : %s\n", copy);
        for (int i = 0; i < TABLE_SIZE - 1; i++) {
          //Log.notice("mapping %d->%s\n", i, mapping[i]);
          if (strcmp(mapping[i], copy) == 0) {
            Log.notice("mapping : %s\n", mapping[i + 1]);
            switch (mapping[i + 1][0])
            {
              case 'W':
                commands.wheel = mapping[i + 1];
                break;
              case 'N':
                commands.lights = mapping[i + 1];
                break;
              case 'L':
                commands.neck = mapping[i + 1];
                break;
              case 'S':
                Log.notice("Mapping load sound process");
                //remove first char
                strcpy(commands.sound, mapping[i + 1]);
                break;
              case 'M':
                commands.mouth = mapping[i + 1];
                break;
              case 'E':
                commands.eyes = mapping[i + 1];
                break;
              case 'H':
                commands.heart = mapping[i + 1];
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
          //Log.notice("mapping %d->%s\n", i, mapping[i]);
          if (strcmp(mapping[i], copy) == 0) {
            return mapping[i + 1];
          }
        }
      }
      return key;
    }

};