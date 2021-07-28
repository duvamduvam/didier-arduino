#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArduinoLog.h"
#include "Fonctions.h"


#include "Mapping.h"

    Mapping::Mapping() {

      mapping[1] = (char*)"B4";
      mapping[2] = (char*)"L01";

      mapping[3] = (char*)"R5";
      mapping[4] = (char*)"N100";

      mapping[5] = (char*)"B6";
      mapping[6] = (char*)"L03";

      mapping[7] = (char*)"B7";
      mapping[8] = (char*)"L04";

      mapping[9] = (char*)"B8";
      mapping[10] = (char*)"L05";

      mapping[11] = (char*)"B9";
      mapping[12] = (char*)"L06";

      mapping[13] = (char*)"B*";
      mapping[14] = (char*)"LON";

      mapping[15] = (char*)"B0";
      mapping[16] = (char*)"L07";

      mapping[17] = (char*)"A1";
      mapping[18] = (char*)"F01";
      //mapping[18] = (char*)"L! LE+% |L! LE-% |L! LE-% ";

      mapping[19] = (char*)"A2";
      mapping[20] = (char*)"F02";

      mapping[21] = (char*)"A3";
      mapping[22] = (char*)"NN+";

      mapping[23] = (char*)"RC";
      mapping[24] = (char*)"NN-";

      mapping[25] = (char*)"A6";
      mapping[26] = (char*)"F08";

      mapping[27] = (char*)"d ";
      mapping[28] = (char*)"L1";

      mapping[29] = (char*)"e ";
      mapping[30] = (char*)"L7";

      mapping[31] = (char*)"f ";
      mapping[32] = (char*)"L6";
	  
	  mapping[33] = (char*)"A7";
      mapping[34] = (char*)"F09";

      mapping[35] = (char*)"";
      mapping[36] = (char*)"F02";

      mapping[37] = (char*)"A9";
      mapping[38] = (char*)"F11";

      mapping[39] = (char*)"";
      mapping[40] = (char*)"F01";

      mapping[41] = (char*)"C1";
      mapping[42] = (char*)"S01";

      mapping[43] = (char*)"C2";
      mapping[44] = (char*)"S02";

      mapping[45] = (char*)" D";
      mapping[46] = (char*)"S07";

      mapping[47] = (char*)"";
      mapping[48] = (char*)"F07";

      mapping[49] = (char*)"";
      mapping[50] = (char*)"F12";

      mapping[51] = (char*)"";
      mapping[52] = (char*)"F01";

      mapping[53] = (char*)"";
      mapping[54] = (char*)"LON";

      mapping[55] = (char*)"B1";
      mapping[56] = (char*)"L01";

      mapping[57] = (char*)"B2";
      mapping[58] = (char*)"L07";

      mapping[59] = (char*)"B3";
      mapping[60] = (char*)"L06";

      mapping[61] = (char*)"B4";
      mapping[62] = (char*)"L05";
	  
	  mapping[63] = (char*)"B5";
      mapping[64] = (char*)"L02";
	  
      mapping[65] = (char*)"a ";
      mapping[66] = (char*)"F01";

      mapping[67] = (char*)"b ";
      mapping[68] = (char*)"F02";

      mapping[69] = (char*)"C3";
      mapping[70] = (char*)"S03";

      mapping[71] = (char*)"C4";
      mapping[72] = (char*)"S04";

      mapping[73] = (char*)"C5";
      mapping[74] = (char*)"S05";

      mapping[75] = (char*)"C6";
      mapping[76] = (char*)"S06";

      mapping[77] = (char*)"C7";
      mapping[78] = (char*)"S07";

      mapping[79] = (char*)"C8";
      mapping[80] = (char*)"S08";

      mapping[81] = (char*)"C9";
      mapping[82] = (char*)"S09";
	  
	  mapping[83] = (char*)"C*";
      mapping[84] = (char*)"S0s";
	  
	  mapping[85] = (char*)" D";
      mapping[86] = (char*)"S00";

      mapping[87] = (char*)" E";
      mapping[88] = (char*)"S04";

      mapping[89] = (char*)" F";
      mapping[90] = (char*)"S08";

      mapping[91] = (char*)" G";
      mapping[92] = (char*)"S05";

      mapping[93] = (char*)" H";
      mapping[94] = (char*)"S07";

      mapping[95] = (char*)" I";
      mapping[96] = (char*)"S03";

      mapping[97] = (char*)" I";
      mapping[78] = (char*)"";

      mapping[99] = (char*)" J";
      mapping[100] = (char*)"";

      mapping[101] = (char*)" K";
      mapping[102] = (char*)"A";
	  
	  mapping[103] = (char*)" L";
      mapping[104] = (char*)"";

    }


    Commands Mapping::getCommands(char* key)
    {

      //Log.notice("Mapping.cpp getComand 55 input : [%s] \n", key);
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
			char n[3];
			n[0] = copy[1];
			n[1] = copy[2];
			n[2] = 0;
           // Log.notice("Neck - V=%d \n", val);
            strcpy(commands.neck, n);
            break;

          case 'P':
            int piezzo;
            piezzo = GetIntAfterString(copy, "P");
            if (piezzo > 50)
              strcpy(commands.face, "F06");
            else if (piezzo > 35)
              strcpy(commands.face, "F05");
            else
              strcpy(commands.face, "F04");

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
                    Log.notice("Light mapping command %s : mapping[i + 1]:%s commands.lights:%s\n", key, mapping[i + 1], commands.lights );
                    break;
                  case 'S':
                    strcpy(commands.sound, mapping[i + 1]);
                    break;
                  case 'F':
                    strcpy(commands.face, mapping[i + 1]);
					//Log.notice("Light mapping command %s : mapping[i + 1]:%s commands.face:%s\n", key, mapping[i + 1], commands.face );
                    break;
				  case 'R':
                    strcpy(commands.relays, mapping[i + 1]);
                    break;
                }
              }
            }
            break;

        }
      }
      return commands;
    }


    char* Mapping::getValue(char* key) {
      char* copy = strtrim_safe(key);
      if (strcmp(key, "") != 0) {
        //Log.notice("copy %s\n", copy);
        //Log.notice("copy : %s\n", copy);
        for (int i = 0; i < TABLE_SIZE - 1; i++) {

#ifdef LOG_NOTICE_MAPPING
          Log.notice("mapping %s->%s\n", key, mapping[i]);
#endif

          if (strcmp(mapping[i], copy) == 0) {
			  	  Log.notice("found %s for key %s\n", mapping[i + 1], copy);
            return mapping[i + 1];
          }
        }
      }
	  Log.notice("no key found for %s\n", copy);
      return key;
    }
