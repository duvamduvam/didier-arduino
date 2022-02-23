#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ArduinoLog.h"
#include "Fonctions.h"


#include "Mapping.h"

    Mapping::Mapping() {

      mapping[1] = (char*)"A0";
      mapping[2] = (char*)"";

      mapping[3] = (char*)"A1";
      mapping[4] = (char*)"F1";

      mapping[5] = (char*)"A2";
      mapping[6] = (char*)"F2";

      mapping[7] = (char*)"A3";
      mapping[8] = (char*)"F3";

      mapping[9] = (char*)"A4";
      mapping[10] = (char*)"F4";

      mapping[11] = (char*)"A5";
      mapping[12] = (char*)"F5";

      mapping[13] = (char*)"A6";   
      mapping[14] = (char*)"F6";

      mapping[15] = (char*)"A7";
      mapping[16] = (char*)"F7";

      mapping[17] = (char*)"A8";
      mapping[18] = (char*)"F8";
      //mapping[18] = (char*)"L! LE+% |L! LE-% |L! LE-% ";

      mapping[19] = (char*)"A9";
      mapping[20] = (char*)"F9";

      mapping[21] = (char*)"A*";
      mapping[22] = (char*)"NN+";

      mapping[23] = (char*)"A#";
      mapping[24] = (char*)"NN-";

      mapping[25] = (char*)"B0";
      mapping[26] = (char*)"";

      mapping[27] = (char*)"B1";
      mapping[28] = (char*)"L1";

      mapping[29] = (char*)"B2";
      mapping[30] = (char*)"L2";

      mapping[31] = (char*)"B3";
      mapping[32] = (char*)"L3";
	  
	  mapping[33] = (char*)"B4";
      mapping[34] = (char*)"L4";

      mapping[35] = (char*)"B5";
      mapping[36] = (char*)"L5";

      mapping[37] = (char*)"B6";
      mapping[38] = (char*)"L6";

      mapping[39] = (char*)"B7";
      mapping[40] = (char*)"L7";

      mapping[41] = (char*)"B8";
      mapping[42] = (char*)"L8";

      mapping[43] = (char*)"B9";
      mapping[44] = (char*)"L9";

      mapping[45] = (char*)"B*";
      mapping[46] = (char*)"LON";

      mapping[47] = (char*)"B#";
      mapping[48] = (char*)"F06";

      mapping[49] = (char*)"C0";
      mapping[50] = (char*)"S24";

      mapping[51] = (char*)"C1";
      mapping[52] = (char*)"S10";

      mapping[53] = (char*)"C2";
      mapping[54] = (char*)"S13";

      mapping[55] = (char*)"C3";
      mapping[56] = (char*)"S12";

      mapping[57] = (char*)"C4";
      mapping[58] = (char*)"S17";

      mapping[59] = (char*)"C5";
      mapping[60] = (char*)"S18";

      mapping[61] = (char*)"C6";
      mapping[62] = (char*)"S25";
	  
	  mapping[63] = (char*)"C7";
      mapping[64] = (char*)"S26";
	  
      mapping[65] = (char*)"C8";
      mapping[66] = (char*)"S27";

      mapping[67] = (char*)"C9";
      mapping[68] = (char*)"S28";

      mapping[69] = (char*)"C*";
      mapping[70] = (char*)"S00";

      mapping[71] = (char*)"C#";
      mapping[72] = (char*)"S23";

      mapping[73] = (char*)"D0";
      mapping[74] = (char*)"F23";

      mapping[75] = (char*)"D1";
      mapping[76] = (char*)"S19";

      mapping[77] = (char*)"D2";
      mapping[78] = (char*)"S20";
	  
	  mapping[79] = (char*)"D3";
      mapping[80] = (char*)"S21";

      mapping[81] = (char*)"D4";
      mapping[82] = (char*)"S22";

      mapping[83] = (char*)"D5";
      mapping[84] = (char*)"S23";

      mapping[85] = (char*)"D6";
      mapping[86] = (char*)"S26";

      mapping[87] = (char*)"D7";
      mapping[88] = (char*)"S27";

      mapping[89] = (char*)"D8";
      mapping[90] = (char*)"L05";

      mapping[91] = (char*)"D9";
      mapping[92] = (char*)"L06";

      mapping[93] = (char*)"D*";   
      mapping[94] = (char*)"LON";

      mapping[95] = (char*)"D#";
      mapping[96] = (char*)"L07";

      mapping[97] = (char*)"a ";
      mapping[98] = (char*)"S10F04";
      //mapping[18] = (char*)"L! LE+% |L! LE-% |L! LE-% ";

      mapping[99] = (char*)"b ";
      mapping[100] = (char*)"S12F12";

      mapping[101] = (char*)"c ";
      mapping[102] = (char*)"S29F13";

      mapping[103] = (char*)"d ";
      mapping[104] = (char*)"S30F13";

      mapping[105] = (char*)"e ";
      mapping[106] = (char*)"S31F13";

      mapping[107] = (char*)"f ";
      mapping[108] = (char*)"S32F13";

      mapping[109] = (char*)"g ";
      mapping[110] = (char*)"S12";

      mapping[111] = (char*)"i ";
      mapping[112] = (char*)"F19";
	  
      mapping[113] = (char*)"j ";
      mapping[114] = (char*)"F20";

      mapping[115] = (char*)"k ";
      mapping[116] = (char*)"F21.";

      mapping[117] = (char*)"l ";
      mapping[118] = (char*)"F22";

      mapping[119] = (char*)"m ";
      mapping[120] = (char*)"F23";

      mapping[121] = (char*)"n ";
      mapping[122] = (char*)"S16";

      mapping[123] = (char*)" A";
      mapping[124] = (char*)"S02";

      mapping[125] = (char*)" B";
      mapping[126] = (char*)"S17";

      mapping[127] = (char*)" C";
      mapping[128] = (char*)"S00";

      mapping[129] = (char*)" D";
      mapping[130] = (char*)"";

      mapping[131] = (char*)" E";
      mapping[132] = (char*)"";

      mapping[133] = (char*)" G";
      mapping[134] = (char*)"";

      mapping[135] = (char*)" H";
      mapping[136] = (char*)"";

      mapping[137] = (char*)" I";
      mapping[138] = (char*)"";

      mapping[139] = (char*)" J";
      mapping[140] = (char*)"";

      mapping[141] = (char*)" K";
      mapping[142] = (char*)"";
	  
	  mapping[143] = (char*)" L";
      mapping[144] = (char*)"";
	  
      mapping[145] = (char*)" M";
      mapping[146] = (char*)"";

      mapping[147] = (char*)" N";
      mapping[148] = (char*)"";

      mapping[149] = (char*)"C1";
      mapping[150] = (char*)"F4";

      mapping[151] = (char*)"C2";
      mapping[152] = (char*)"F5";

      mapping[153] = (char*)"C3";
      mapping[154] = (char*)"F4";

      mapping[155] = (char*)"";
      mapping[156] = (char*)"S12";

      mapping[157] = (char*)"";
      mapping[158] = (char*)"S11";

      mapping[159] = (char*)"";
      mapping[160] = (char*)"S14";

      mapping[161] = (char*)"";
      mapping[162] = (char*)"S15";
	  
	  mapping[163] = (char*)"";
      mapping[164] = (char*)"S0s";
	  
	  mapping[165] = (char*)"";
      mapping[166] = (char*)"S17";

      mapping[167] = (char*)"";
      mapping[168] = (char*)"S18";

      mapping[169] = (char*)"";
      mapping[170] = (char*)"S19";

      mapping[171] = (char*)"";
      mapping[172] = (char*)"S20";

      mapping[173] = (char*)"";
      mapping[174] = (char*)"S21";

      mapping[175] = (char*)"";
      mapping[176] = (char*)"S22";

      mapping[177] = (char*)"";
      mapping[178] = (char*)"F23";

      mapping[179] = (char*)"";
      mapping[180] = (char*)"F04";

      mapping[181] = (char*)"";
      mapping[182] = (char*)"F04";
	  
	  mapping[183] = (char*)"";
      mapping[184] = (char*)"F04";

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
              String s = String(mapping[i + 1]);
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

                    if(s.length()>3 && s.charAt(3)=='F'){
                    	strcpy(commands.face, s.substring(3,6).c_str());
                    	strcpy(commands.sound, s.substring(0,3).c_str());
                    	Log.notice("Sound mapping sound : %s face : %s\n", commands.sound, commands.face);
                    }else{
                    	strcpy(commands.sound, mapping[i + 1]);
                    }            
		    Log.notice("Sound mapping command %s : mapping[i + 1]:%s commands.sound:%s\n", key, mapping[i + 1], commands.sound );
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
