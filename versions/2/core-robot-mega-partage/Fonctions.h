#pragma once
//#include <Arduino.h>
//#include <String.h>


//#ifndef FONCTIONS
//#define FONCTIONS

#include <FastLED.h>


//######### MATHS ##########
//--------------------------
float MapFloat(float x, float in_min, float in_max, float out_min, float out_max);


//######### COLORS ##########
//--------------------------
CRGB Brightness(CRGB Base, float intens);
CRGB ColorTintByte(byte tint);
CRGB ColorTint(int angle);
CRGB RandomColor();
CRGB RandomColorTint();
CRGB Gradient(int startAngle, int stopAngle, float ratio);

//#endif
