
#include <Arduino.h>
#include <FastLED.h>
#include <string.h>
#include <stdio.h>
#include "ArduinoLog.h"

#include "Fonctions.h"


#define PALETTE_SIZE 32

class ColorPalette 
{

  private:

    
  public:
    //Axe X
    int Tint; //0-360
    int Span; //0-90

    //Axe Y
    float BrightnessMin;
    float BrightnessMax;

    int nb_Colors;
    CRGB Palette[PALETTE_SIZE];

    
    ColorPalette() 
    {
      nb_Colors = 5;
    }

    void Init(int tint,int span,float brightnessMin,float brightnessMax)
    {
      Tint = tint;
      Span = span;
      BrightnessMin = brightnessMin;
      BrightnessMax = brightnessMax;
    }

    void GenerateRandom() 
    {
      float ratioTint;
      float ratioBright;
      int i;
      for(i=0;i<nb_Colors;i++)
      {
        int rndTint = random(255);
        int rndBright = random(255);
        ratioTint = MapFloat(rndTint,0,255,0,1);
        ratioBright = MapFloat(i,0,255,BrightnessMin,BrightnessMax);
        CRGB color = Gradient(Tint, Tint+Span, ratioTint);
        Palette[i] = Brightness(color,ratioBright);
      }
    }

    void GenerateDegrade(float brightness) 
    {
      int i;
      for(i=0;i<nb_Colors;i++)
      {
        float ratioTint = MapFloat(i,0,nb_Colors,0,1);
        CRGB color = Gradient(Tint, Tint+Span, ratioTint);
        Palette[i] = Brightness(color,brightness);
      }
    }

    void GenerateBright(int tint) 
    {
      int i;
      for(i=0;i<nb_Colors;i++)
      {
        CRGB color = ColorTint(tint);
        float ratioBright = MapFloat(i,0,nb_Colors,BrightnessMin,BrightnessMax);
        Palette[i] = Brightness(color,ratioBright);
      }
    
    }

};
