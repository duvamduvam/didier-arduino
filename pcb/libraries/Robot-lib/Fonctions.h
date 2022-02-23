	#ifndef FONCTIONS_H
#define FONCTIONS_H

//#include <Arduino.h>
//#include <String.h>
//#include <Servo.h>

//#ifndef FONCTIONS
//#define FONCTIONS

//make the compilation silent
#define FASTLED_INTERNAL
#include <FastLED.h>

//######### PROTOCOL ##########
//--------------------------
void ParseVectorString(char* chain, int* adX, int* adY);
int GetIntAfterString(char* chain, char* separator);
char* SubString20(char* chain, int startIndex, int stopIndex);
char* ShiftCharArray20(char* chain);

//######### BOUND ##########
//--------------------------
int BoundInt(int value, int valMin, int valMax);

//######### CHAIN ##########
//--------------------------

int AsciiIntToInt(char* chain);
char* IntToAsciiInt(int value);
int CharArrayLength(char *s,int sizeMax);
char* Concat16(char* a, char* b);
char* arrayConcat(char* concatArray, int concatArraySize, char* postfix, int postFixSize);


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
void extractRadioChar(char* receiver, char* str);
byte arraySize(char array[]);
bool recvWithStartEndMarkers(char* input, bool newData, byte inputSize);

#endif
