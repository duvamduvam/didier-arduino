#include <Arduino.h>
#include "ArduinoLog.h"


const int VIBRATION = 13;

class Vibramode
{
  private :

    bool actionEnCours; //Si un message est en cours d'envoi
    int actionIndex; //numéro de pas du message
    char* sendMessage; //Message à envoyer
    byte tH,tB,nb; //indices de temps et nombre de pas
    int delayHaut,delayBas; //delay en milisecondes
    bool state; //Etat du niveau de sortie actuel
    int currentDelay;
    int lastMillis;

  public:

    Vibramode()
    {
      pinMode(VIBRATION, OUTPUT);
    }

    //Exemple de commande:
    //"1521314152"
    //"1,2,3,4,5 temps haut
    //"5,1,1,1,2 temps bas
    //voir mapDelay pour table de durées prédéfinies

    void process(char* message) {
      
      if (strcmp((char*)message, "") == 0) return;
      strcpy(sendMessage,message);
      Log.notice("Vibramode.cpp 57 Process: %s\n", sendMessage);

      nb = strlen(sendMessage)/2;
      actionEnCours = 1;
      actionIndex = 0;
      state=0;
      next();

    }
    
    void setHigh(){

      currentDelay = delayHaut;
      lastMillis = millis();
      digitalWrite(VIBRATION, 1);
      state = 1;
    }

    void setLow(){

      currentDelay = delayBas;
      lastMillis = millis();
      digitalWrite(VIBRATION, 0);
      state = 0;
    }


    int mapDelay(char indice){
      byte val = indice - '0'; //char BCD->byte
      switch (val){
        case 1:return 10;
        case 2:return 20;
        case 3:return 30;
        case 4:return 40;
        case 5:return 50;
        case 6:return 100;
        case 7:return 200;
        case 8:return 500;
        case 9:return 1000;
        case 10:return 2000;
      }
        
    }
    
    void next() {
      tH = sendMessage[actionIndex*2];
      tB = sendMessage[actionIndex*2+1];

      delayHaut = mapDelay(tH);
      delayBas = mapDelay(tB);

      setHigh();
      if (actionIndex<nb)
        actionIndex++;
      else{
        actionEnCours=0;//stop
        setLow();
      }
    }

    


  

    void execute()
    {
      if (!actionEnCours) return;
      if (millis() - lastMillis > currentDelay)
      {
       
        if (state)
          setLow();
        else
          next();
      }
    }

};
