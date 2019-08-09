
#include <FastLED.h>
#define NUM_LEDS 300
// on cable yellow of led strip
#define DATA_PIN 6

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
}
void loop() {
  //for (int dot = 0; dot < NUM_LEDS; dot++) {
    //leds[dot] = CRGB::White;
    //leds[dot].red = random(255);
    //leds[dot].blue = random(255);
    //leds[dot].green = random(255);
    
    //FastLED.show();
    // clear this led for the next time around the loop
    //leds[dot] = CRGB::Black;
    //delay(30);
  //} 

  //for (int dot = 0; dot < NUM_LEDS; dot++) {
    int dot = random(500);
    //leds[dot] = CRGB::White;
    leds[dot].red = random(255);
    leds[dot].blue = random(255);
    leds[dot].green = random(255);
    
    FastLED.show();
    // clear this led for the next time around the loop
    //leds[dot] = CRGB::Black;
    delay(30);
  //} 

  
}
