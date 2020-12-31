#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArduinoLog.h"
#include "Fonctions.h"

#define TABLE_SIZE 21

class Mapping {

  private:
    char* mapping[TABLE_SIZE];

  public :
    Mapping() {

      mapping[1] = (char*)"$A3";
      mapping[2] = (char*)"MFORWARD";

      mapping[3] = (char*)"$A4";
      mapping[4] = (char*)"MBACKWARD";

      mapping[5] = (char*)"$A5";
      mapping[6] = (char*)"HLEFT";

      mapping[7] = (char*)"$A6";
      mapping[8] = (char*)"HRIGHT";

      mapping[9] = (char*)"$A7";
      mapping[10] = (char*)"MLEFT";

      mapping[11] = (char*)"$A8";
      mapping[12] = (char*)"MRIGHT";

      mapping[13] = (char*)"a";
      mapping[14] = (char*)"HLEFT";

      mapping[15] = (char*)"z";
      mapping[16] = (char*)"HRIGHT";

      mapping[17] = (char*)"$A1";
      mapping[18] = (char*)"LE+";

      mapping[19] = (char*)"$A2";
      mapping[20] = (char*)"LE-";

    }

    char* getValue(char* key) {


      char* copy = strtrim_safe(key);
      if (strcmp((char*)key, "") != 0) {
        //Log.notice("copy %s\n", copy);
        //Log.notice("copy : %s\n", copy);
        for (int i = 0; i < TABLE_SIZE - 1; i++) {
          //Log.notice("mapping %d->%s\n", i, mapping[i]);
          if (strcmp(mapping[i], copy) == 0) {
            //Serial.println(mapping[i + 1]);
            return mapping[i + 1];
          }
        }
      }

      return key;
    }
};
