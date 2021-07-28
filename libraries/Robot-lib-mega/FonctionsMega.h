#include <Arduino.h>

char serialRead(byte nb);
boolean serialAvailable(byte nb);
bool recvWithStartEndMarkersMega(char* input, bool newData,  byte inputSize, byte serial);