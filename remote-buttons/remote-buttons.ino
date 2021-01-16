#include <Keypad.h>

const byte ROWS = 3; 
const byte COLS = 8; 
char keys[ROWS][COLS] = {
  {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'},
  {'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'},
  {'Q', 'R', 'S', 'T', 'U', 'V', 'X', 'Y'},
};
byte rowPins[ROWS] = {12, 11, 10};
byte colPins[COLS] = {9, 8, 7, 6, 5, 4, 3, 2};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(115200);
}

void loop() {
  char key = keypad.getKey();

  if (key != NO_KEY) {
    Serial.println(key);
  }
}
