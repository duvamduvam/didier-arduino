#include <Keypad.h>

const int MIC_pin = A0;
const int X_pin = A1; // analog pin connected to X output
const int Y_pin = A2; // analog pin connected to Y output

const byte ROWS = 3;
const byte COLS = 8;
char keys[ROWS][COLS] = {
  {'K', 'R', 'S', 'T', 'U', 'V', 'W', 'X'},
  {'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'},
  {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'},
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
    Serial.print(key);
  }

  /*byte x = analogRead(X_pin) / 4;
  byte y = analogRead(Y_pin) / 4;
  byte mic = analogRead(MIC_pin) / 4; 

  Serial.print("X-axis: ");
  Serial.print(x);
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.println(y);
  Serial.print("MIC: ");
  Serial.println(mic);
  Serial.print("\n\n");
  delay(10);*/
}
