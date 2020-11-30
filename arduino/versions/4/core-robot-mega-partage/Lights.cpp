//TODO 
// leds a change
// 1 -> 0
// 4 -> 2
// 5 -> 7 / 8
// 


#include <Arduino.h>
#include <FastLED.h>
#include <string.h>
#include <stdio.h>
#include "ArduinoLog.h"

#include "Fonctions.h"

#include "ColorPalette.cpp"


#define NUM_LEDS 265
// on cable yellow of led strip
#define DATA_PIN 6

//front
#define LINE_FRONT_BOTTOM_LEFT_START 0 //
#define LINE_FRONT_BOTTOM_LEFT_END 11 
#define LINE_FRONT_UP_LEFT_START 12 //1
#define LINE_FRONT_UP_LEFT_END 36 
#define LINE_FRONT_UP_RIGHT_START 37 //2
#define LINE_FRONT_UP_RIGHT_END 61 
#define LINE_FRONT_BOTTOM_RIGHT_START 62 //3
#define LINE_FRONT_BOTTOM_RIGHT_END 73 
#define LINE_FRONT_MIDDLE_DOWN_START 74 //4
#define LINE_FRONT_MIDDLE_DOWN_END 78 
#define LINE_FRONT_MIDDLE_UP_START 79 //5
#define LINE_FRONT_MIDDLE_UP_END 87 

//Left
#define LEFT_DOWN_START 88 //6
#define LEFT_DOWN_END 113 

//Back
#define BACK_START 114 //7
#define BACK_END 143

//Left
#define LEFT_UP_START 144 //8
#define LEFT_UP_END 169 

//Up
#define UP_LEFT_START 170 //9
#define UP_LEFT_END 190 
#define UP_RIGHT_START 191 //10
#define UP_RIGHT_END 211 

//Right
#define RIGHT_UP_START 212 //11
#define RIGHT_UP_END 237 
#define RIGHT_DOWN_START 238 //12
#define RIGHT_DOWN_END 264



#define NB_BARS 13

#define FACE_FRONT  0
#define FACE_LEFT   1
#define FACE_RIGHT  2
#define FACE_UP     3
#define FACE_BACK   4

#define NB_SNAKES_MAX   20



class Lights {

  private:

    CRGB leds[NUM_LEDS];
    int i,outputIndex,ledIndex;
    
  public:

    //Faces - Bars dimmers (float 0 à 1)
    float masterFace[5];
    float masterBar[NB_BARS];

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
    
    int chaser2Index;
    long chaser2Timer;
    long chaser2Delay;

    //Effect5
    float heartIntensity;
    bool heartInspire;


    //Effect6
    int brightSpan;
    int choixTint;
    int nbColors = 5;


    //Effect7
    int snakeIndex;
    int tintScroll;

    typedef struct Snake
    {
      int Pos;
      int Size;
      int Speed; 
      bool Forward; 
      long speedTimer;
      CRGB MainColor;
    };

    Snake snakes[NB_SNAKES_MAX];
    
    int choixEffect=5;

    
    ColorPalette palette;
    
    
    Lights() 
    {
      FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    }

    void process(char in[]) 
    {
      bool present = true;
      
        
        if(strstr((char*)in, "A28") != 0) {
          choixEffect++;
        }else if(strstr((char*)in, "A34") != 0) {
          choixEffect--;
        }else if(strstr((char*)in, "A29") != 0) {
          brightSpan++;
        }else if(strstr((char*)in, "A35") != 0) {
          brightSpan--;
        }else if(strstr((char*)in, "A26") != 0) {
          choixTint++;
        }else if(strstr((char*)in, "A32") != 0) {
          choixTint--;
        }else if(strstr((char*)in, "A27") != 0) {
          nbColors++;
        }else if(strstr((char*)in, "A33") != 0) {
          nbColors--;
        }else
          present = false;

        if (present)
          Log.notice("effet %d | brightSpan %d | tint %d  | nbColors %d \n",choixEffect, brightSpan, choixTint,nbColors );
          
      
      switch (choixEffect)
      {
        case 1: Effet1();break;
        case 2: Effet2();break;
        case 3: Effet3();break;
        case 4: Effet4();break;
        case 5: Effet5();break;
        case 6: Effet6();break;
        case 7: Effet7();break;
        case 8: Effet8();break;
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
            float faceIntens = FaceMaster(ledIndex);
            float barIntens = BarMaster(ledIndex);
            leds[ledIndex] = Brightness(color, faceIntens*barIntens);
            ledIndex++;
            if(ledIndex>=NUM_LEDS)
              break;
          }
          if(ledIndex>=NUM_LEDS)
            break;
        }
      }

      //Stars
      if (starsEnabled)
      {
        leds[starIndex] = starColor;
      }

      
      FastLED.show();
    }

    void SetAllFaces(float intens)
    {
      int i;
      for(i=0;i<5;i++)
      {
        masterFace[i]=intens;
      }
    }

    void SetAllBar(float intens)
    {
      int i;
      for(i=0;i<NB_BARS;i++)
        masterBar[i]=intens;

    }

    //Chase rainbow 5
    void Effet1()
    {
      
      SetAllFaces(1);
      SetAllBar(1);

      //Motif
      nbOutputs=NUM_LEDS;
      sizeOutput=1;
      
     
      //stars
      starsEnabled = false;
      
      
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
          int tint = map(j,0,nbOutputs,0,360);

          motif[i] = ColorTint(tint);
        }
        
        
      }


      


    
      
    }

    void StarsEffectA()
    {
      if (millis() - starsTimer > starsDelay)
        {
          starsTimer = millis();

          starIndex = random(NUM_LEDS);
          starColor = RandomColor();
          starColor = RandomColorTint();
          
        } 
    }


    

    //Chase 8
    void Effet2()
    {
      
      SetAllFaces(1);
      SetAllBar(1);


      //Motif
      nbOutputs=10;
      sizeOutput=1;

      //stars
      starsEnabled = false;
      
      
      //Effect
      chaserDelay=50;
      
      if (millis() - chaserTimer > chaserDelay) 
      {
        chaserTimer = millis();
        chaserIndex++;
        if (chaserIndex>=nbOutputs)
          chaserIndex=0;

        for (i=0;i<nbOutputs;i++)
        {
          int tint = map(i,0,nbOutputs,0,360);
          CRGB color = ColorTint(tint);
          float intens=0;
          if (i == chaserIndex)
            intens = 1;
          motif[i] =  Brightness(color, intens);
        }
        
        
      }

      
         
      
   }




    //Test Faces
    void Effet3()
    {
      SetAllFaces(1);
      SetAllBar(1);

      //Motif
      nbOutputs=NUM_LEDS;
      sizeOutput=1;

      //stars
      starsEnabled = false;
      
      
      //Effect
      chaserDelay=50;
      chaser2Delay=500;


      
      
    if (millis() - chaserTimer > chaserDelay) 
    {
      chaserTimer = millis();
      for (i=0;i<NUM_LEDS;i++)
      {

          int barIndex = FindBarIndex(i);
          int barModulo = barIndex%4;
          int tint = map(barModulo,0,NB_BARS,0,360);
          CRGB color = ColorTint(tint);
          
          motif[i] =  color;
      }
        
        
    }

      
      
   }


      
    //Test Faces
    void Effet4()
    {
      SetAllFaces(1);
//      SetAllBar(1);

      //Motif
      nbOutputs=1;
      sizeOutput=1;

      //stars
      starsEnabled = false;
      
      
      //Effect
      chaserDelay=50;
      chaser2Delay=500;
      
      if (millis() - chaserTimer > chaserDelay) 
      {
        chaserTimer = millis();
        chaserIndex++;
        if (chaserIndex>=nbOutputs)
          chaserIndex=0;

        int tint = random(360);
        motif[0] = ColorTint(tint);

      }

      
      if (millis() - chaser2Timer > chaser2Delay) 
      {
        chaser2Timer = millis();
        chaser2Index++;
        if (chaser2Index>=NB_BARS)
          chaser2Index=0;

        for (i=0;i<NB_BARS;i++)
        {
          masterBar[i]=0;
          if (i == chaser2Index)
            masterBar[i]=1;
        }
        
        
      }
         
      
   }


    //Heart Beat
    void Effet5()
    {
//      SetAllFaces(1);
      SetAllBar(1);

      //Motif
      nbOutputs=1;
      sizeOutput=1;

      //stars
      starsEnabled = false;
      
      
      //Effect
      chaserDelay=50;
      chaser2Delay=500;
      
      if (millis() - chaserTimer > chaserDelay) 
      {
        chaserTimer = millis();

        if (heartInspire)
        {
          heartIntensity+=0.1;
          if (heartIntensity>=1)
            heartInspire=false;
        }
        else
        {
          heartIntensity-=0.1;
          if (heartIntensity<=0)
            heartInspire=true;
        }
        
        motif[0] =   CRGB::Red;
        SetAllFaces(heartIntensity);
      }

      
      
   }






   
    //Test Palette
    void Effet6()
    {
      SetAllFaces(1);
      SetAllBar(1);

      //Motif
      nbOutputs=nbColors;
      sizeOutput=3;

      //stars
      starsEnabled = false;
      
      
      //Effect
      chaserDelay=300;
      
      if (millis() - chaserTimer > chaserDelay) 
      {
        chaserTimer = millis();

        int tint = map(choixTint,0,10,0,360);
        int span = 90;


        float brightDelta = MapFloat(brightSpan,0,10,0,1);
        float brightnessMin = 1-brightDelta;
        float brightnessMax = 1+brightDelta;
        
        palette.Init(tint,span,brightnessMin,brightnessMax,nbColors);
        palette.GenerateRandom();
        //palette.GenerateBright(0);
        

      }
  
      int barIndex;
      int paletteIndex;

      for (i=0;i<nbColors;i++)
      {
        motif[i] = palette.Palette[i];
      }

      /*
      //1 couleur par barre de led
      for (i=0;i<NUM_LEDS;i++)
      {
        barIndex = FindBarIndex(i);
        paletteIndex = barIndex % palette.NbColors;
        motif[i] = palette.Palette[paletteIndex];
      }
      */
         
      
   }



  //Snakes
   void Effet7()
   {
     SetAllFaces(1);
     SetAllBar(1);

     //Motif
     nbOutputs=NUM_LEDS;
     sizeOutput=1;

      //Palette
      brightSpan=6;
      nbColors=5;


     //stars
      starsEnabled = false;

  

     //Creation
     while (snakeIndex<NB_SNAKES_MAX)
     {
        InitSnake(&snakes[snakeIndex]);
        snakeIndex++;
     }
   
     //Clear Outputs
     for (i=0;i<NUM_LEDS;i++)
        motif[i] = CRGB::Black;
      
     int i,j,ledIndex;
     for(i=0;i<NB_SNAKES_MAX;i++)
     {
        //Refresh Movement
        ExecuteSnake(&snakes[i]);
        

        //Paint
        for(j=0;j<snakes[i].Size;j++)
        {
          ledIndex=snakes[i].Pos+j;
          if (ledIndex>NUM_LEDS)
            ledIndex-=NUM_LEDS; //Bouclage ecran

          motif[ledIndex] = snakes[i].MainColor;
        }
        
     }



    //Changement Palette
     chaserDelay=1000;
     if (millis() - chaserTimer > chaserDelay) 
     {
        chaserTimer = millis();

        tintScroll+=10;
        if (tintScroll>360)
          tintScroll-=360;
          
        int tint = tintScroll;//map(choixTint,0,10,0,360);
        int span = 90;


        float brightDelta = MapFloat(brightSpan,0,10,0,1);
        float brightnessMin = 1-brightDelta;
        float brightnessMax = 1+brightDelta;
        
        palette.Init(tint,span,brightnessMin,brightnessMax,nbColors);
        palette.GenerateRandom();

        ChangeSnakePalette();
     }

     
     
   }

   void InitSnake(Snake *s)
   {
      s->Pos = random(NUM_LEDS);
      s->Size = random(2,10);
      s->Speed = random(1,50);
      s->Forward = random(0,2);
   }
  
   void ExecuteSnake(Snake *s)
   {
     if (millis() - s->speedTimer > s->Speed) 
      {
        s->speedTimer = millis();
        if (s->Forward)
          s->Pos++;
        else
          s->Pos--;


        if (s->Pos>NUM_LEDS)
          s->Pos-=NUM_LEDS;
        if (s->Pos<0)
          s->Pos+=NUM_LEDS;
      }
   }

   void ChangeSnakePalette()
   {
      int paletteIndex;
      for(i=0;i<NB_SNAKES_MAX;i++)
      {
        paletteIndex = i % palette.NbColors;
        snakes[i].MainColor = palette.Palette[paletteIndex];
      }
   }

    
  //Stars
   void Effet8()
   {
     SetAllFaces(0);
      SetAllBar(0);

      //stars
      starsEnabled = true;

      StarsEffectA();
   }
    
    float FaceMaster(int ledIndex)
    {
      int faceIndex = FindFaceIndex(ledIndex);
      return masterFace[faceIndex];
    }
    
    int FindFaceIndex(int ledIndex)
    {
      if(ledIndex>=LINE_FRONT_BOTTOM_LEFT_START && ledIndex<=LINE_FRONT_MIDDLE_UP_END)
        return FACE_FRONT;
      if(ledIndex>=LEFT_DOWN_START && ledIndex<=LEFT_DOWN_END)
        return FACE_LEFT;
      if(ledIndex>=BACK_START && ledIndex<=BACK_END)
        return FACE_BACK;
      if(ledIndex>=LEFT_UP_START && ledIndex<=LEFT_UP_END)
        return FACE_LEFT;
      if(ledIndex>=UP_LEFT_START && ledIndex<=UP_RIGHT_END)
        return FACE_UP;
      if(ledIndex>=RIGHT_UP_START && ledIndex<=RIGHT_DOWN_END)
        return FACE_RIGHT;
    }



    float BarMaster(int ledIndex)
    {
      byte barIndex = FindBarIndex(ledIndex);
        return masterBar[barIndex];
    }



    byte FindBarIndex(int ledIndex)
    {
      //Front
      if(ledIndex>=LINE_FRONT_BOTTOM_LEFT_START && ledIndex<=LINE_FRONT_BOTTOM_LEFT_END)
        return 0;
      if(ledIndex>=LINE_FRONT_UP_LEFT_START && ledIndex<=LINE_FRONT_UP_LEFT_END)
        return 1;
      if(ledIndex>=LINE_FRONT_UP_RIGHT_START && ledIndex<=LINE_FRONT_UP_RIGHT_END)
        return 2;
      if(ledIndex>=LINE_FRONT_BOTTOM_RIGHT_START && ledIndex<=LINE_FRONT_BOTTOM_RIGHT_END)
        return 3;
      if(ledIndex>=LINE_FRONT_MIDDLE_DOWN_START && ledIndex<=LINE_FRONT_MIDDLE_DOWN_END)
        return 4;
      if(ledIndex>=LINE_FRONT_MIDDLE_UP_START && ledIndex<=LINE_FRONT_MIDDLE_UP_END)
        return 5;

      //Left
      if(ledIndex>=LEFT_DOWN_START && ledIndex<=LEFT_DOWN_END)
        return 6;

      //Back 
      if(ledIndex>=BACK_START && ledIndex<=BACK_END)
        return 7;

      //Left
      if(ledIndex>=LEFT_UP_START && ledIndex<=LEFT_UP_END)
        return 8;

      //Up
      if(ledIndex>=UP_LEFT_START && ledIndex<=UP_LEFT_END)
        return 9;
      if(ledIndex>=UP_RIGHT_START && ledIndex<=UP_RIGHT_END)
        return 10;

      //Right
      if(ledIndex>=RIGHT_UP_START && ledIndex<=RIGHT_UP_END)
        return 11;
      if(ledIndex>=RIGHT_DOWN_START && ledIndex<=RIGHT_DOWN_END)
        return 12;
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
