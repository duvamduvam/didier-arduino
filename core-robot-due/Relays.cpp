#include <Arduino.h>
#include "ArduinoLog.h"
#include "Fonctions.h"
#include "Commands.h"

#define AMPLI_ON "R1"
#define RH_EFFECT_ON "R2"
#define AMPLI_OFF "R3"
#define RH_EFFECT_OFF"R4"

class Relays {

  private :
    const int RELAY_1 = 47; // eratic problems ...
    const int RELAY_2 = 45;
    const int RELAY_3 = 43;
    const int RELAY_4 = 41;
    const int RELAY_5 = 39;
    const int RELAY_6 = 37;
    const int RELAY_7 = 35;
    const int RELAY_8 = 33;

  public :

    Relays() {
      Log.notice("init relays\n");
      pinMode(RELAY_1, OUTPUT);
      pinMode(RELAY_2, OUTPUT);
      pinMode(RELAY_3, OUTPUT);
      pinMode(RELAY_4, OUTPUT);
      pinMode(RELAY_5, OUTPUT);
      pinMode(RELAY_6, OUTPUT);
      pinMode(RELAY_7, OUTPUT);
      pinMode(RELAY_8, OUTPUT);
    }

    void process(char* in) {
      if (in[0] == 'R') {
        if (!strcmp(in, AMPLI_ON)) {
          Serial.print("change ampli on ");
          digitalWrite(RELAY_4, HIGH);
        } else if (!strcmp(in, AMPLI_OFF)) {
          Serial.print("change ampli off ");
          digitalWrite(RELAY_4, LOW);
        } else if (!strcmp(in, RH_EFFECT_ON)) {
          Serial.print("change hf and ampli on ");
          digitalWrite(RELAY_2, HIGH);
          digitalWrite(RELAY_4, HIGH);
        } else if (!strcmp(in, RH_EFFECT_OFF)) {
          Serial.print("change hf off ");
          digitalWrite(RELAY_2, LOW);
        }
      }
    }

    void execute() {

    }

};
