#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArduinoLog.h"
#include "Fonctions.h"

#define TABLE_SIZE 104

class Mapping {

  private:
    char* mapping[TABLE_SIZE];

  public :
    Mapping() {

      mapping[0] = (char*)"$A1";
      mapping[1] = (char*)"A1";

      mapping[2] = (char*)"$A2";
      mapping[3] = (char*)"A2";

      mapping[4] = (char*)"$A3";
      mapping[5] = (char*)"MFORWARD";

      mapping[6] = (char*)"$A4";
      mapping[7] = (char*)"MBACKWARD";

      mapping[8] = (char*)"$A5";
      mapping[9] = (char*)"N 5";

      mapping[10] = (char*)"$A6";
      mapping[11] = (char*)"N 6";

      mapping[12] = (char*)"$A7";
      mapping[13] = (char*)"MLEFT";

      mapping[14] = (char*)"$A8";
      mapping[15] = (char*)"MRIGHT";

      mapping[16] = (char*)"$A9";
      mapping[17] = (char*)"N 9";

      mapping[18] = (char*)"$A10";
      mapping[19] = (char*)"N 10";

      mapping[20] = (char*)"$A11";
      mapping[21] = (char*)"N 11";

      mapping[22] = (char*)"$A12";
      mapping[23] = (char*)"N 12";

      mapping[24] = (char*)"$A13";
      mapping[25] = (char*)"N 13";

      mapping[26] = (char*)"$A14";
      mapping[27] = (char*)"N 14";

      mapping[28] = (char*)"$A15";
      mapping[29] = (char*)"N 15";

      mapping[30] = (char*)"$A16";
      mapping[31] = (char*)"N 16";

      mapping[32] = (char*)"$A17";
      mapping[33] = (char*)"HRIGHT";

      mapping[34] = (char*)"$A18";
      mapping[35] = (char*)"N 21";

      mapping[36] = (char*)"A19";
      mapping[37] = (char*)"HLEFT";

      mapping[38] = (char*)"$A20";
      mapping[39] = (char*)"N 23";

      mapping[40] = (char*)"$A21";
      mapping[41] = (char*)"N 24";

      mapping[42] = (char*)"$A22";
      mapping[43] = (char*)"N 25";

      mapping[44] = (char*)"$A23";
      mapping[45] = (char*)"N 26";

      mapping[46] = (char*)"$A24";
      mapping[47] = (char*)"N 27";



      mapping[48] = (char*)"$B1";
      mapping[49] = (char*)"N 28";

      mapping[50] = (char*)"$B2";
      mapping[51] = (char*)"N 29";

      mapping[52] = (char*)"$B3";
      mapping[53] = (char*)"HLEFT";

      mapping[54] = (char*)"$B4";
      mapping[55] = (char*)"HRIGHT";

      mapping[56] = (char*)"$B5";
      mapping[57] = (char*)"N 32";

      mapping[58] = (char*)"$B6";
      mapping[59] = (char*)"N 33";

      mapping[60] = (char*)"$B7";
      mapping[61] = (char*)"N 34";

      mapping[62] = (char*)"$B8";
      mapping[63] = (char*)"N 35";

      mapping[64] = (char*)"$B9";
      mapping[65] = (char*)"LE+";

      mapping[66] = (char*)"$B10";
      mapping[67] = (char*)"N 37";

      mapping[68] = (char*)"$B11";
      mapping[69] = (char*)"LE-";

      mapping[70] = (char*)"$B12";
      mapping[71] = (char*)"N 39";

      mapping[72] = (char*)"$B13";
      mapping[73] = (char*)"N 40";

      mapping[74] = (char*)"$B14";
      mapping[75] = (char*)"N 41";

      mapping[76] = (char*)"$B15";
      mapping[77] = (char*)"N 42";

      mapping[78] = (char*)"$B16";
      mapping[79] = (char*)"N 43";

      mapping[80] = (char*)"$B17";
      mapping[81] = (char*)"N 44";

      mapping[82] = (char*)"$B18";
      mapping[83] = (char*)"N 45";

      mapping[84] = (char*)"$B19";
      mapping[85] = (char*)"N 46";

      mapping[86] = (char*)"$B20";
      mapping[87] = (char*)"N 47";

      mapping[88] = (char*)"$B21";
      mapping[89] = (char*)"N 48";

      mapping[90] = (char*)"$B22";
      mapping[91] = (char*)"N 49";

      mapping[92] = (char*)"$B23";
      mapping[93] = (char*)"N 50";

      mapping[94] = (char*)"$B24";
      mapping[95] = (char*)"N 51";


      mapping[96] = (char*)"$C1";
      mapping[97] = (char*)"$D37";

      mapping[98] = (char*)"$C2";
      mapping[99] = (char*)"$D31";

      mapping[100] = (char*)"$C3";
      mapping[101] = (char*)"LE+";

      mapping[102] = (char*)"$C4";
      mapping[103] = (char*)"LE-";

      mapping[104] = (char*)"$C5";
      mapping[105] = (char*)"$D37";

      mapping[106] = (char*)"$C6";
      mapping[107] = (char*)"$D31";

      mapping[108] = (char*)"$C7";
      mapping[109] = (char*)"$D36";

      mapping[110] = (char*)"$C8";
      mapping[111] = (char*)"$D30";

      mapping[112] = (char*)"$C9";
      mapping[113] = (char*)"$D37";

      mapping[114] = (char*)"$C10";
      mapping[115] = (char*)"$D31";

      mapping[116] = (char*)"$C11";
      mapping[117] = (char*)"$D36";

      mapping[118] = (char*)"$C12";
      mapping[119] = (char*)"$D30";

      mapping[120] = (char*)"$C13";
      mapping[121] = (char*)"$D37";

      mapping[122] = (char*)"$C14";
      mapping[123] = (char*)"$D31";

      mapping[124] = (char*)"$C15";
      mapping[125] = (char*)"$D36";

      mapping[126] = (char*)"$C16";
      mapping[127] = (char*)"$D30";

      mapping[128] = (char*)"$C17";
      mapping[129] = (char*)"$D37";

      mapping[130] = (char*)"$C18";
      mapping[131] = (char*)"$D31";

      mapping[132] = (char*)"$C19";
      mapping[133] = (char*)"$D36";

      mapping[134] = (char*)"$C20";
      mapping[135] = (char*)"$D30";

      mapping[136] = (char*)"$C21";
      mapping[137] = (char*)"$D37";

      mapping[138] = (char*)"$C22";
      mapping[139] = (char*)"$D31";

      mapping[140] = (char*)"$C23";
      mapping[141] = (char*)"$D36";

      mapping[142] = (char*)"$C24";
      mapping[143] = (char*)"$D30";


      mapping[144] = (char*)"$D1";
      mapping[145] = (char*)"$D37";

      mapping[146] = (char*)"$D2";
      mapping[147] = (char*)"$D31";

      mapping[148] = (char*)"$D3";
      mapping[149] = (char*)"$D36";

      mapping[150] = (char*)"$D4";
      mapping[151] = (char*)"$D30";

      mapping[152] = (char*)"$D5";
      mapping[153] = (char*)"$D37";

      mapping[154] = (char*)"$D6";
      mapping[155] = (char*)"$D31";

      mapping[156] = (char*)"$D7";
      mapping[157] = (char*)"$D36";

      mapping[158] = (char*)"$D8";
      mapping[159] = (char*)"$D30";

      mapping[160] = (char*)"$D9";
      mapping[161] = (char*)"$D37";

      mapping[162] = (char*)"$D10";
      mapping[163] = (char*)"$D31";

      mapping[164] = (char*)"$D11";
      mapping[165] = (char*)"$D36";

      mapping[166] = (char*)"$D12";
      mapping[167] = (char*)"$D30";

      mapping[168] = (char*)"$D13";
      mapping[169] = (char*)"$D37";

      mapping[170] = (char*)"$D14";
      mapping[171] = (char*)"$D31";

      mapping[172] = (char*)"$D15";
      mapping[173] = (char*)"$D36";

      mapping[174] = (char*)"$D16";
      mapping[175] = (char*)"$D30";

      mapping[176] = (char*)"$D17";
      mapping[177] = (char*)"$D37";

      mapping[178] = (char*)"$D18";
      mapping[179] = (char*)"$D31";

      mapping[180] = (char*)"$D19";
      mapping[181] = (char*)"$D36";

      mapping[182] = (char*)"$D20";
      mapping[183] = (char*)"$D30";

      mapping[184] = (char*)"$D21";
      mapping[185] = (char*)"$D37";

      mapping[186] = (char*)"$D22";
      mapping[187] = (char*)"$D31";

      mapping[188] = (char*)"$D23";
      mapping[189] = (char*)"$D36";

      mapping[190] = (char*)"$D24";
      mapping[191] = (char*)"$D30";

    }

    char* getValue(char* key) {


      char* copy = strtrim_safe(key);
      if (strcmp((char*)key, "") != 0) {
        //Log.notice("copy %s\n", copy);
        //Log.notice("copy : %s\n", copy);
        for (int i = 0; i < TABLE_SIZE - 1; i++) {
          //Log.notice("mapping %d->%s\n", i, mapping[i]);
          if (strcmp(mapping[i], copy) == 0) {
            Serial.println(mapping[i + 1]);
            return mapping[i + 1];
          }
        }
      }
      return NULL;
    }
};
