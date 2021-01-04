
#include "Fonctions.h"

//######### UTILS ##########
//--------------------------
char* strtrim_safe( char* input ) {

  int size = sizeof(input);

  if (input[size] == ' ') {
    char* copy[size - 1];
    for ( int i = 0; i < size-1; i++ ) {
      copy[i] = input[i];
    }
  }
  return input;
}
