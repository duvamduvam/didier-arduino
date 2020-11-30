#include <Arduino.h>
#include <FastLED.h>
#include <string.h>
#include <stdio.h>
#include "ArduinoLog.h"

#define NUM_LEDS 268
// on cable yellow of led strip
#define DATA_PIN 6

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

//#define LedBlock blocks[NUM_LEDS];

class Lights {

  private:

    CRGB leds[NUM_LEDS];

  public:

    Lights() {
      FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    }

    void process() {
      ledRandom(10);
    }

    void process2(String msg) {
      //100~130:200~300|0xFF44DD

      //printDebug(msg);
      int nb = msg.indexOf("|");

      String ledPosString = msg.substring(0, nb);
      //printDebug(ledPosString);

      msg.substring(nb + 1);
      uint32_t color = strtoul(msg.substring(nb + 1).c_str(), nullptr, HEX);
      //Serial.println(color);

      initilizeLeds();

      //280|0xFF44DD
      if (ledPosString.indexOf(':') == -1) {
        //Serial.println(color);
        setupLeds(ledPosString, color);

        //100~130:200~300|0xFF44DD 100~130:200~300:110~150:210~280|0xFF44DD
      } else {
        String ledPosList[10];
        while (ledPosString.indexOf(':') != -1) {
          int ledPos = ledPosString.indexOf(":");
          //printDebug(ledPosString.substring(0, ledPos));
          setupLeds(ledPosString.substring(0, ledPos), color);

          ledPosString = ledPosString.substring(ledPos + 1);
          if (ledPosString.indexOf(':') == -1) {
            //printDebug(ledPosString);
            setupLeds(ledPosString, color);
          }
        }
      }
      //initilizeLeds();
      FastLED.show();

    }

    void initilizeLeds() {
      FastLED.clear();
    }

    void setupLeds(String nbLeds, uint32_t color) {

      //Log.notice (F("color %s"CR ), color);

      int nb = nbLeds.indexOf('~');
      if (nb == -1) {
        leds[nbLeds.toInt()] = color;
      } else {

        int start = nbLeds.substring(0, nb).toInt();
        int end = nbLeds.substring(nb + 1).toInt();
        for (int led = start; led <= end; led++) {
          leds[led] = color;
        }
      }
    }

    void ledRoundFront() {

      for (int led = LINE_FRONT_BOTTOM_LEFT_START;
           led <= LINE_FRONT_BOTTOM_LEFT_END; led++) {
        ledRandomColor(led, 30);
      }
      for (int led = LINE_FRONT_UP_LEFT_START; led <= LINE_FRONT_UP_LEFT_END;
           led++) {
        ledRandomColor(led, 30);
      }
      for (int led = LINE_FRONT_UP_RIGHT_START;
           led <= LINE_FRONT_UP_RIGHT_END; led++) {
        ledRandomColor(led, 30);
      }
      for (int led = LINE_FRONT_BOTTOM_RIGHT_START;
           led <= LINE_FRONT_BOTTOM_RIGHT_END; led++) {
        ledRandomColor(led, 30);
      }
    }

    void ledRoundFull() {

      for (int led = 0; led <= NUM_LEDS; led++) {
        ledRandomColor(led, 30);
      }
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
