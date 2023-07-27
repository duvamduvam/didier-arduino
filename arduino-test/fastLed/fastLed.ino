#include <FastLED.h>

#define NUM_LEDS 30
#define PIN 6
CRGB leds[NUM_LEDS];

void setup() {

  Serial.begin(115200);
  Serial.println("test leds");
  FastLED.addLeds<NEOPIXEL, PIN>(leds, NUM_LEDS);
  FastLED.setBrightness(  100 );
}

void ledRandomColor(int nbLed, int time) {

  leds[nbLed].red = 255; //random(255);
  //leds[nbLed].blue = random(255);
  //leds[nbLed].green = random(255);

  FastLED.show();
  // clear this led for the next time around the loop
  delay(time);
  leds[nbLed] = CRGB::Black;
}

void loop() {
  for (int i = 0; i < NUM_LEDS; i++) {
    ledRandomColor(i, 50);

  }

}
