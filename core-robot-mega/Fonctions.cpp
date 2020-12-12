
#include "Fonctions.h"



//######### MATHS ##########
//--------------------------


float MapFloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  if ((in_max - in_min + out_min) == 0)
    return 0;
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}



//######### COLORS ##########
//--------------------------
CRGB Brightness(CRGB Base, float intens)
{
  CRGB result;
  if (intens < 0)
    intens = 0;
  if (intens > 2)
    intens = 2;

  if (intens <= 1)
  { //Dimmer
    result.red = Base.red * intens;
    result.green = Base.green * intens;
    result.blue = Base.blue * intens;
  }
  else
  { //brightnass
    int gain = (intens - 1) * 255;
    int r = Base.red + gain;
    int g = Base.green + gain;
    int b = Base.blue + gain;
    if (r > 255)
      r = 255;
    if (g > 255)
      g = 255;
    if (b > 255)
      b = 255;

    result.red = r;
    result.green = g;
    result.blue = b;
  }
  return result;
}

CRGB ColorTintByte(byte tint)
{
  CRGB result;

  tint = 255 - tint;
  if (tint < 85)
  {
    result.red = 255 - tint * 3;
    result.green = 0;
    result.blue = tint * 3;
  }
  else if (tint < 170)
  {
    tint -= 85;
    result.red = 0;
    result.green = tint * 3;
    result.blue = 255 - tint * 3;
  }
  else
  {
    tint -= 170;
    result.red = tint * 3;
    result.green = 255 - tint * 3;
    result.blue = 0;
  }

  return result;
}

CRGB ColorTint(int angle)
{
  return ColorTintByte(map(angle, 0, 360, 0, 255));
}

CRGB RandomColor()
{
  CRGB result;
  result.red = random(255);
  result.blue = random(255);
  result.green = random(255);
  return result;
}

CRGB RandomColorTint()
{
  byte tint = random(255);
  return ColorTint(tint);
}


CRGB Gradient(int startAngle, int stopAngle, float ratio)
{
  float angle = MapFloat(ratio, 0, 1, startAngle, stopAngle);
  return ColorTint((int)angle);
}


//######### UTILS ##########
//--------------------------
char* strtrim_safe( char* input ) {

  int size = sizeof(input);

  if (input[size] == ' ') {
    char* copy[size - 1];
    for ( int i = 0; i < size-1; i++ ) {
      copy[i] = input[i];
    }
  }
  return input;
}
