
#include <Arduino.h>
//#include <FastLED.h>
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

    int NbColors;
    CRGB Palette[PALETTE_SIZE];

    
    ColorPalette() 
    {
     
    }

    void Init(int tint,int span,float brightnessMin,float brightnessMax,int nbColors)
    {
      Tint = tint;
      Span = span;
      BrightnessMin = brightnessMin;
      BrightnessMax = brightnessMax;
      NbColors = nbColors;
    }

    void GenerateRandom() 
    {
      float ratioTint;
      float ratioBright;
      int i;
      for(i=0;i<NbColors;i++)
      {
        int rndTint = random(255);
        int rndBright = random(255);
        ratioTint = MapFloat(rndTint,0,255,0,1);
        ratioBright = MapFloat(rndBright,0,255,BrightnessMin,BrightnessMax);
        CRGB color = Gradient(Tint, Tint+Span, ratioTint);
        Palette[i] = Brightness(color,ratioBright);
      }
    }

    void GenerateDegrade(float brightness) 
    {
      int i;
      for(i=0;i<NbColors;i++)
      {
        float ratioTint = MapFloat(i,0,NbColors,0,1);
        CRGB color = Gradient(Tint, Tint+Span, ratioTint);
        Palette[i] = Brightness(color,brightness);
      }
    }

    void GenerateBright(float tintRatio) 
    {
      int i;
      for(i=0;i<NbColors;i++)
      {
        float angle = MapFloat(tintRatio,0,1,Tint, Tint+Span);
        CRGB color = ColorTint(angle);
        float ratioBright = MapFloat(i,0,NbColors,BrightnessMin,BrightnessMax);
        Palette[i] = Brightness(color,ratioBright);
      }
    
    }

};
