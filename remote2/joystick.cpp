#include <Arduino.h>

#define xPin 6
#define yPin 7

class JoyStick
{
  private :
    int x;
    int y;

  public:
    JoyStick() {
      pinMode(xPin, INPUT);
      pinMode(yPin, INPUT);
    }

    char* getValues() {
      x = analogRead(xPin);
      char intx = x / 103;
      y = analogRead(yPin);
      char inty = y / 103;
      char values[2];
      values[0] = intx;
      values[1] = inty;
      Serial.print("joystick values : "); Serial.println(values);
      return values;
    }
};
