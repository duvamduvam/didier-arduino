#pragma once
//#include <Arduino.h>
//#include <String.h>
//#include <Servo.h>

//#ifndef FONCTIONS
//#define FONCTIONS

//make the compilation silent
#define FASTLED_INTERNAL
#include <FastLED.h>


//######### BOUND ##########
//--------------------------
int BoundInt(int value, int valMin, int valMax);




//######### MATHS ##########
//--------------------------
float MapFloat(float x, float in_min, float in_max, float out_min, float out_max);


//######### TO ROBOT ##########
//--------------------------

struct SpeedVector
{
  float left;
  float right;
};

SpeedVector ToRobot(int bX, int bY);



//######### COLORS ##########
//--------------------------
CRGB Brightness(CRGB Base, float intens);
CRGB ColorTintByte(byte tint);
CRGB ColorTint(int angle);
CRGB RandomColor();
CRGB RandomColorTint();
CRGB Gradient(int startAngle, int stopAngle, float ratio);

//######### HORLOGE RTC ##########
//--------------------------
void ExecuteRtc();

//######### UTILS ##########
//--------------------------
char* strtrim_safe(char* in);
//void moveServo(Servo servo, int pos);
char* nextToken(char* str);

long extractTime(char* input, int seconds, int minutes);

//char** getInputList(char* str);
void extractChar(char* receiver, char* str, int s, int e);
byte arraySize(char array[]);
char* arrayConcat(char* concatArray, int concatArraySize, char* postfix, int postFixSize);
