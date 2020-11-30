#include <Arduino.h>
#include <FastLED.h>
#include <string.h>
#include <stdio.h>
#include "ArduinoLog.h"

#define NUM_LEDS 268
// on cable yellow of led strip
#define DATA_PIN 10

//front
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

//Left
#define LEFT_DOWN_START 89
#define LEFT_DOWN_END 115

//Back
#define BACK_START 116
#define BACK_END 145

//Left
#define LEFT_UP_START 146
#define LEFT_UP_END 175

//Up
#define UP_LEFT_START 176
#define UP_LEFT_END 192
#define UP_RIGHT_START 193
#define UP_RIGHT_END 213

//Right
#define RIGHT_UP_START 214
#define RIGHT_UP_END 239
#define RIGHT_DOWN_START 240
#define RIGHT_DOWN_END 268

//#define LedBlock blocks[NUM_LEDS];




class Lights {

  private:

    CRGB leds[NUM_LEDS];
    int i,outputIndex,ledIndex;
    
  public:

    

    //Faces dimmers (float 0 à 1)
    float masterFront;
    float masterUp;
    float masterBack;
    float masterLeft;
    float masterRight;

    //Motif
    int nbOutputs;
    int sizeOutput;
    CRGB motif[NUM_LEDS];

    //stars
    bool starsEnabled;
    long starsTimer;
    long starsDelay;
    int starIndex;
    CRGB starColor;

    //effects
    int chaserIndex;
    long chaserTimer;
    long chaserDelay;
    int effectIndex=1;

    

    
    Lights() 
    {
      FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    }

    void process() 
    {
      switch(effectIndex)
      {
        case 0:
          Effet1();
          break;
      }
      
      
      Generate();
    }

    void Generate() 
    {
      //Motif
      ledIndex=0;
      while(ledIndex<NUM_LEDS)
      {
        for(outputIndex=0;outputIndex<nbOutputs;outputIndex++)
        {
          for(i=0;i<sizeOutput;i++)
          {
            CRGB color = motif[outputIndex]; 
            float intens = FaceMaster(ledIndex);
            leds[ledIndex] = IntensityColor(color, intens);
            ledIndex++;
          }
        }
      }

      //Stars
      if (starsEnabled)
      {
        leds[starIndex] = starColor;
      }

      
      FastLED.show();
    }

    void FacesAllEnabled()
    {
      masterFront=1;
      masterUp=1;
      masterBack=1;
      masterLeft=1;
      masterRight=1;

    }

    void Effet1()
    {
      FacesAllEnabled();

      //Motif
      nbOutputs=5;
      sizeOutput=3;

      //stars
      starsDelay=10;
      starsEnabled = true;
      
      
      //Effect
      chaserDelay=200;


      int i,j;
      
      if (millis() - chaserTimer > chaserDelay) 
      {
        chaserTimer = millis();
        chaserIndex++;
        if (chaserIndex>=nbOutputs)
          chaserIndex=0;

        for (i=0;i<nbOutputs;i++)
        {
          j=(i+chaserIndex)%nbOutputs;
          int tint = map(j,0,nbOutputs,0,255);

          motif[i] = Wheel(tint);
        }
        

        
      }

      if (starsEnabled)
      {
        if (millis() - starsTimer > starsDelay)
        {
          starsTimer = millis();

          starIndex = random(NUM_LEDS);
          starColor = RandomColor();
          starColor = RandomColorTint();
          
        } 
      }
      
      
    }




      

    float FaceMaster(int ledIndex)
    {
      if(ledIndex>=LINE_FRONT_BOTTOM_LEFT_START && ledIndex<=LINE_FRONT_MIDDLE_UP_END)
        return masterFront;
      if(ledIndex>=LEFT_DOWN_START && ledIndex<=LEFT_DOWN_END)
        return masterLeft;
      if(ledIndex>=BACK_START && ledIndex<=BACK_END)
        return masterBack;
      if(ledIndex>=LEFT_UP_START && ledIndex<=LEFT_UP_END)
        return masterLeft;
      if(ledIndex>=UP_LEFT_START && ledIndex<=UP_RIGHT_END)
        return masterUp;
      if(ledIndex>=RIGHT_UP_START && ledIndex<=RIGHT_DOWN_END)
        return masterRight;
    }



  
  
  
  CRGB IntensityColor(CRGB Base, float intens)
  {
    CRGB result;
    if (intens<0)
      intens = 0;
    if (intens>1)
      intens = 1;
      
    result.red = Base.red * intens;
    result.green = Base.green * intens;
    result.blue = Base.blue * intens;
    return result;
  }
  
  CRGB Wheel(byte tint)
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
      result.blue =255 - tint * 3;
    }
    else
    {
      tint -= 170;
      result.red = tint * 3;
      result.green = 255 - tint * 3;
      result.blue =0;
    }
  
    return result;
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
    return Wheel(tint);
  }





    
    void processOLD() 
    {
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
