#include <Arduino.h>
#include <analogWrite.h>
// Arduino pin numbers
const int pin = 25; // analog pin connected to Y output

void setup() {


  
  pinMode(pin, OUTPUT);
  Serial.begin(115200);
}

void loop() {

  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += 5) 
  {
    // sets the value (range from 0 to 255):
    analogWrite(pin, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }
 
  // fade out from max to min in increments of 5 points:
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= 5) 
  {
    // sets the value (range from 0 to 255):
    analogWrite(pin, fadeValue);
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }

}
