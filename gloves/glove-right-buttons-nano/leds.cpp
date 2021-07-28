#include <Arduino.h>
//make the compilation silent
#define FASTLED_INTERNAL
#include <FastLED.h>
#include "ArduinoLog.h"

#define NUM_LEDS 3
// on cable yellow of led strip
#define DATA_PIN 6

class Lights {

  private:

    CRGB leds[NUM_LEDS];

  public:

    Lights()
    {
      FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    }


    void initilizeLeds() {
      FastLED.clear();
    }

    void ledRandom(int time) {

      int led = random(NUM_LEDS);
      ledRandomColor(led, time);

    }

    void ledRandomColor(int nbLed, int time) {

      leds[nbLed].red = random(255);
      leds[nbLed].blue = random(255);
      leds[nbLed].green = random(255);

      FastLED.show();
      // clear this led for the next time around the loop
      delay(time);
      leds[nbLed] = CRGB::Black;
    }

};
