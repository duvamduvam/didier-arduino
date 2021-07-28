#include "FonctionsMega.h"

char serialRead(byte nb) {

  switch (nb) {
    case 0:
      return Serial.read();
    case 1:
      return Serial1.read();
    case 2:
      return Serial2.read();
	case 3:
      return Serial3.read();
  }
  return 0;
}

boolean serialAvailable(byte nb) {

  switch (nb) {
    case 0:
      return Serial.available();
    case 1:
      return Serial1.available();
    case 2:
      return Serial2.available();
	case 3:
      return Serial3.available();
  }
  return false;
}

bool recvWithStartEndMarkersMega(char* input, bool newData,  byte inputSize, byte serial) {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';
  char rc;

  while (serialAvailable(serial) > 0 && newData == false) {
    rc = serialRead(serial);
    if (recvInProgress == true) {
      if (rc != endMarker) {
        input[ndx] = rc;
        ndx++;
        if (ndx >= inputSize) {
          ndx = inputSize - 1;
        }
      }
      else {
        input[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        return true;
      }
    }
    else if (rc == startMarker) {
      recvInProgress = true;
    }
  }
  return false;
}

