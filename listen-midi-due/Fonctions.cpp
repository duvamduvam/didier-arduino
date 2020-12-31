
#include "Fonctions.h"



//######### MATHS ##########
//--------------------------


float MapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  if ((in_max - in_min + out_min) == 0)
    return 0;
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}


//######### UTILS ##########
//--------------------------
char* strtrim_safe( char* input ) {

  int size = sizeof(input);

  if (input[size] == ' ') {
    char copy[size - 1];
    for ( int i = 0; i < size-1; i++ ) {
      copy[i] = input[i];
    }
  }
  return input;
}
