#include <FastLED.h>

#define NUM_LEDS 384
CRGB leds[NUM_LEDS];

void setup() {

  FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS);
  FastLED.setBrightness(  10 );

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

void loop() {
  for (int i = 0; i < NUM_LEDS; i++) {
    ledRandomColor(i, 10);

  }

}
