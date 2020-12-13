#include <string.h>
#include "Fonctions.h"
#include "ArduinoLog.h"

#define TABLE_SIZE 48

class Mapping {

  private:
    int mapping[TABLE_SIZE];

  public :
    Mapping() {

      mapping[0] = 53;
      mapping[1] = 1;

      mapping[2] = 47;
      mapping[3] = 2;

      mapping[4] = 37;
      mapping[5] = 3;

      mapping[6] = 31;
      mapping[7] = 4;

      mapping[8] = 999; //bouton 2 ligne 1 colonne
      mapping[9] = 5;

      mapping[10] = 46;
      mapping[11] = 6;

      mapping[12] = 36;
      mapping[13] = 7;

      mapping[14] = 30;
      mapping[15] = 8;

      mapping[16] = 999; //3 eme ligne 1 colone
      mapping[17] = 9;

      mapping[18] = 44;
      mapping[19] = 10;

      mapping[20] = 34;
      mapping[21] = 11;

      mapping[22] = 28;
      mapping[23] = 12;

      mapping[24] = 51; // bouton 4 ligne 1ere colonne -> fait tout bugger
      mapping[25] = 13;

      mapping[26] = 45;
      mapping[27] = 14;

      mapping[28] = 35;
      mapping[29] = 15;

      mapping[30] = 29;
      mapping[31] = 16;

      mapping[32] = 48;
      mapping[33] = 17;

      mapping[34] = 42;
      mapping[35] = 18;

      mapping[36] = 32;
      mapping[37] = 19;

      mapping[38] = 26;
      mapping[39] = 20;

      mapping[40] = 49;
      mapping[41] = 21;

      mapping[42] = 43;
      mapping[43] = 22;

      mapping[44] = 33;
      mapping[45] = 23;

      mapping[46] = 27;
      mapping[47] = 24;

    }

    int getValue(int key) {

      for (int i = 0; i < TABLE_SIZE - 1; i++) {
        if (mapping[i] ==  key) {
          Log.notice("mapping key %d -> %d\n",key,  mapping[i + 1]);
          return mapping[i + 1];
        }
      }
      return 0;
    }

};
