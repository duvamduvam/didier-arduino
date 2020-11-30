
#include <FastLED.h>
#define NUM_LEDS 268
// on cable yellow of led strip
#define DATA_PIN 9

#define LINE_FRONT_BOTTOM_LEFT_START 0
#define LINE_FRONT_BOTTOM_LEFT_END 11
#define LINE_FRONT_UP_LEFT_START 12
#define LINE_FRONT_UP_LEFT_END 36
#define LINE_FRONT_UP_RIGHT_START 37
#define LINE_FRONT_UP_RIGHT_END 61
#define LINE_FRONT_BOTTOM_RIGHT_START 62
#define LINE_FRONT_BOTTOM_RIGHT_END 73
#define LINE_FRONT_MIDDLE_DOWN_START 74
#define LINE_FRONT_MIDDLE_DOWN_END 78
#define LINE_FRONT_MIDDLE_UP_START 79
#define LINE_FRONT_MIDDLE_UP_END 88
#define LEFT_DOWN_START 89
#define LEFT_DOWN_END 115
#define BACK_START 116
#define BACK_END 145
#define LEFT_UP_START 146
#define LEFT_UP_END 175
#define UP_LEFT_START 176
#define UP_LEFT_END 192
#define UP_RIGHT_START 193
#define UP_RIGHT_END 213
#define RIGHT_UP_START 214
#define RIGHT_UP_END 239
#define RIGHT_DOWN_START 240
#define RIGHT_DOWN_END 268

#define start 240
#define endd 268

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  Serial.begin(9600);
}

bool Contains(String s, String search) {
    int max = s.length() - search.length();

    for (int i = 0; i <= max; i++) {
        if (s.substring(i) == search) return true; // or i
    }

    return false; //or -1
} 

void loop() {

  for (int dot = 0; dot <= endd; dot++) {
    //int dot = random(500);
    //leds[dot] = CRGB::White;
    //leds[dot] = 0x6daf21;
    leds[dot].red = random(255);
    leds[dot].blue = random(255);
    leds[dot].green = random(255);
    
    FastLED.show();
    // clear this led for the next time around the loop
    delay(30);
    leds[dot] = CRGB::Black;
}

  
  FastLED.show();
 
}
