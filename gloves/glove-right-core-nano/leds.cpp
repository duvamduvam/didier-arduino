#include <Arduino.h>
//make the compilation silent
#define FASTLED_INTERNAL
#include <FastLED.h>
#include "ArduinoLog.h"

#define NUM_LEDS 3
// on cable yellow of led strip
#define DATA_PIN 6
#define LED_DELAY 10

class Lights {

  private:

    CRGB leds[NUM_LEDS];
    int brightness = 100;
    int brightnessStep = 20;
    bool speak = false;
    long ledTime = 0;

  public:

    Lights()
    {
      FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    }


    void initilizeLeds() {
      FastLED.clear();
    }

    void setSpeak(bool b) {
      speak = b;
    }

    void ledRandom(int time) {

      int led = random(NUM_LEDS);
      ledRandomColor(led, time);

    }

    void fade() {

      if (ledTime < millis()) {


        for (int i = 0; i < NUM_LEDS; i++)
        {
          leds[i] = CRGB::Red;
        }

        if (brightness > 240) {
          brightnessStep = -brightnessStep;
        } else if (brightness <30) {
          brightnessStep = -brightnessStep;
        }
        brightness += brightnessStep;
        FastLED.setBrightness(brightness);
        FastLED.show();
        Log.notice("led brightness %d\n",brightness);
        ledTime = millis + LED_DELAY;
      }
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

    void execute() {
      if (speak) {

      }
    }

};
