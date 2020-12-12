#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArduinoLog.h"
#include "Fonctions.h"

#define TABLE_SIZE 96

class Mapping {

  private:
    char* mapping[TABLE_SIZE];

  public :
    Mapping() {

      mapping[0] = (char*)"$A31";
      mapping[1] = (char*)"MBACKWARD";

      mapping[2] = (char*)"$A37";
      mapping[3] = (char*)"MFORWARD";

      mapping[4] = (char*)"$A30";
      mapping[5] = (char*)"MRIGHT";

      mapping[6] = (char*)"$A36";
      mapping[7] = (char*)"MLEFT";


      mapping[8] = (char*)"$A4";
      mapping[9] = (char*)"N 5";

      mapping[10] = (char*)"$A5";
      mapping[11] = (char*)"N 6";

      mapping[12] = (char*)"$A6";
      mapping[13] = (char*)"N 7";

      mapping[14] = (char*)"$A7";
      mapping[15] = (char*)"N 8";

      mapping[16] = (char*)"$A8";
      mapping[17] = (char*)"N 9";

      mapping[18] = (char*)"$A9";
      mapping[19] = (char*)"N 10";

      mapping[20] = (char*)"$AA";
      mapping[21] = (char*)"N 11";

      mapping[22] = (char*)"$AB";
      mapping[23] = (char*)"N 12";

      mapping[24] = (char*)"$AC";
      mapping[25] = (char*)"N 13";

      mapping[26] = (char*)"$AD";
      mapping[27] = (char*)"N 14";

      mapping[28] = (char*)"$A*";
      mapping[29] = (char*)"N 15";

      mapping[30] = (char*)"$A#";
      mapping[31] = (char*)"N 16";

      mapping[32] = (char*)"$B37";
      mapping[33] = (char*)"HRIGHT";

      mapping[34] = (char*)"$B34";
      mapping[35] = (char*)"N 21";

      mapping[36] = (char*)"$B31";
      mapping[37] = (char*)"HLEFT";

      mapping[38] = (char*)"$B26";
      mapping[39] = (char*)"N 23";

      mapping[40] = (char*)"$B36";
      mapping[41] = (char*)"N 24";

      mapping[42] = (char*)"$B32";
      mapping[43] = (char*)"N 25";

      mapping[44] = (char*)"$B6";
      mapping[45] = (char*)"N 26";

      mapping[46] = (char*)"$B7";
      mapping[47] = (char*)"N 27";

      mapping[48] = (char*)"$B8";
      mapping[49] = (char*)"N 28";

      mapping[50] = (char*)"$B9";
      mapping[51] = (char*)"N 29";

      mapping[52] = (char*)"$BA";
      mapping[53] = (char*)"N 30";

      mapping[54] = (char*)"$BB";
      mapping[55] = (char*)"N 31";

      mapping[56] = (char*)"$BC";
      mapping[57] = (char*)"N 32";

      mapping[58] = (char*)"$BD";
      mapping[59] = (char*)"N 33";

      mapping[60] = (char*)"$B*";
      mapping[61] = (char*)"N 34";

      mapping[62] = (char*)"$B#";
      mapping[63] = (char*)"N 35";

      mapping[64] = (char*)"$C31";
      mapping[65] = (char*)"LE+";

      mapping[66] = (char*)"$C34";
      mapping[67] = (char*)"N 37";

      mapping[68] = (char*)"$C37";
      mapping[69] = (char*)"LE-";

      mapping[70] = (char*)"$C3";
      mapping[71] = (char*)"N 39";

      mapping[72] = (char*)"$C4";
      mapping[73] = (char*)"N 40";

      mapping[74] = (char*)"$C5";
      mapping[75] = (char*)"N 41";

      mapping[76] = (char*)"$C6";
      mapping[77] = (char*)"N 42";

      mapping[78] = (char*)"$C7";
      mapping[79] = (char*)"N 43";

      mapping[80] = (char*)"$C8";
      mapping[81] = (char*)"N 44";

      mapping[82] = (char*)"$C9";
      mapping[83] = (char*)"N 45";

      mapping[84] = (char*)"$CA";
      mapping[85] = (char*)"N 46";

      mapping[86] = (char*)"$CB";
      mapping[87] = (char*)"N 47";

      mapping[88] = (char*)"$CC";
      mapping[89] = (char*)"N 48";

      mapping[90] = (char*)"$CD";
      mapping[91] = (char*)"N 49";

      mapping[92] = (char*)"$C*";
      mapping[93] = (char*)"N 50";

      mapping[94] = (char*)"$SC#";
      mapping[95] = (char*)"N 51";

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
      return NULL;
    }
};
