#include "FonctionsMega.h"
#include "ArduinoLog.h"

#include <SPI.h>
#include <Button.h>
#include <Radio.h>

#include "Inclino.cpp"
#include "Oled.cpp"
#include "Move.cpp"
#include "Keyboard.cpp"

//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#define KEYBOARD_DELAY  10
#define REFRESH_RATE 50
long screenTime;

/*
  //Joy Playstation
  #define JOY_CALIB_X_MIN 0
  #define JOY_CALIB_X_MAX 1023
  #define JOY_CALIB_Y_MIN 1023
  #define JOY_CALIB_Y_MAX 0
*/
#define ANGLE_INCLINO 30

/********************************************************************
                  PIN MAP
                     _ _ /--\
            \/_ _ _/(_(_(_o o)
             (_(_(_(/      ^
********************************************************************/

/*---------------/
  /     RADIO    /
  /---------------*/

char lastMsg[6];


/*---------------/
  /     BATTERY    /
  /---------------*/

//TODO a changer
#define battery 22

/*---------------/
  /     LEDS    /
  /---------------*/

#define redLed 13
#define yellowLed 12

/*---------------/
  /     DIGITAL    /
  /---------------*/
#define PIEZZO_GATE 40

#define CONF_A_1    21
#define CONF_A_2    23
#define CONF_A_3    25
#define CONF_A_4    27

#define CONF_B_1    29
#define CONF_B_2    31
#define CONF_B_3    33
#define CONF_B_4    35


/*---------------/
  /     ANALOG     /
  /---------------*/


#define PIEZZO  A4

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);  // I2C / TWI


/********************************************************************
                      GLOBAL VAR
                  __(.)< __(.)> __(.)=
                  \___)  \___)  \___)
********************************************************************/

/*-------------+
  |   KEYBOARD   |
  +-------------*/



char mod = 'A';

unsigned long t_hold;
#define GLOVE_SIZE
char glove1[3] = {0};
char glove2[3] = {0};

/*-------------+
  |     LOGIC    |
  +-------------*/
char ConfA, ConfB;

/*-------------+
  |    PIEZZO    |
  +-------------*/
int piezzo;

Inclino inclino;
Radio radio;
bool newData = false;
Move move;
Matrix matrix;
Oled oled(u8g);


void setup()
{
  oled.draw(mod, lastMsg);

  /*---------------/
    /    PIN INIT    /
    /---------------*/

  //BP
  pinMode(BP_CALIB, INPUT_PULLUP);

  //COMMUT
  pinMode(CONF_A_1, INPUT_PULLUP);
  pinMode(CONF_A_2, INPUT_PULLUP);
  pinMode(CONF_A_3, INPUT_PULLUP);
  pinMode(CONF_A_4, INPUT_PULLUP);

  pinMode(CONF_B_1, INPUT_PULLUP);
  pinMode(CONF_B_2, INPUT_PULLUP);
  pinMode(CONF_B_3, INPUT_PULLUP);
  pinMode(CONF_B_4, INPUT_PULLUP);

  pinMode(redLed, OUTPUT);
  pinMode(yellowLed, OUTPUT);

  /*---------------/
    /     SERIAL     /
    /---------------*/

  while (!Serial);
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  delay(100);
  Log.begin   (LOG_LEVEL, &Serial);
  Serial.println("start modem");

  radio.init();
}



/*  ____          ____
   |                  |
   |      PIEZZO      |
   |____          ____|   */
void aqqPiezzo() {
  piezzo = map(analogRead(PIEZZO), 0, 500, 0, 126);
}


/*  ____          ____
   |                  |
   |     KEYBOARD     |
   |____          ____|   */


char aqqGlove() {
  bool newData1 = recvWithStartEndMarkersMega(glove1, false,  3, 1);
  bool newData2 = recvWithStartEndMarkersMega(glove2, false,  5, 2);
  if (newData1) {
    Log.notice("glove 1 : %s\n", glove1);
  } else if (newData2) {
    Log.notice("glove 2 : %s\n", glove2);
  }
}

/*  ____          ____
   |                  |
   |     REMOTE     |
   |____          ____|   */
void processRemote() {
  char radioMsg[6] = {};

  if (digitalRead(FADER_ON) == 0) {
  //if(move.isJoystickMoving()){
    strcpy(radioMsg, move.getMsg());
    //radioMsg[2] = ' ';
    newData = true;
  } else {
    radioMsg[0] = ' ';
    radioMsg[1] = ' ';
    radioMsg[2] = ' ';
  }
  radioMsg[3] = ' ';
  radioMsg[4] = ' ';
  radioMsg[5] = 0;

  if (matrix.newKey())
  {
    newData = true;

    radioMsg[3] = matrix.getMod();
    radioMsg[4] = matrix.getKey();
    //Serial.println(key);
    //radio.sendCharMsg(mod, key);
  }

  /*if (key != NO_KEY) {
    Serial.print("<"); Serial.print(key); Serial.println(">");
    holdKey = key;
    }

    if (keypad.getState() == HOLD) {
    if ((millis() - t_hold) > 100 ) {
      Serial.print("<"); Serial.print(holdKey); Serial.println(">");
      t_hold = millis();
    }
    }*/


  if (glove1[0] != 0) {
    newData = true;
    radioMsg[3] = glove1[0];
    glove1[0] = 0;

  }

  if (glove2[0] != 0) {
    newData = true;
    radioMsg[4] = glove2[0];
    glove2[0] = 0;
  }

  if (newData) {
    strcpy(lastMsg, radioMsg);
    radio.sendMsg(radioMsg, 5);
    //Log.notice("radioMsg : %s\n", radioMsg);
    radio.msgset();
    newData = false;
  }

}


/*  ____          ____
   |                  |
   |      COMMUT      |
   |____          ____|   */


void ProcessCommut()
{
  int A, B;
  if (digitalRead(CONF_A_1) == 0)
    A = 1;
  if (digitalRead(CONF_A_2) == 0)
    A = 2;
  if (digitalRead(CONF_A_3) == 0)
    A = 3;
  if (digitalRead(CONF_A_4) == 0)
    A = 4;

  if (digitalRead(CONF_B_1) == 0)
    B = 1;
  if (digitalRead(CONF_B_2) == 0)
    B = 2;
  if (digitalRead(CONF_B_3) == 0)
    B = 3;
  if (digitalRead(CONF_B_4) == 0)
    B = 4;

  if (A != ConfA || B != ConfB) {
    ConfA = A; ConfB = B;
    //Log.notice("A1 : %d A2 : %d A3 : %d A4 : %d B1 : %d B2 : %d B3 : %d B4 : %d\n", digitalRead(CONF_A_1), digitalRead(CONF_A_2),
    //           digitalRead(CONF_A_3), digitalRead(CONF_A_4), digitalRead(CONF_B_1), digitalRead(CONF_B_2), digitalRead(CONF_B_3),
    //           digitalRead(CONF_B_4));
    Log.notice("CONF A : %d B : %d\n", ConfA, ConfB);
  }

}


/********************************************************************
                    EXECUTE
                    ,~~.
                   (  9 )-_,
              (\___ )=='-'
               \ .   ) )
                \ `-' /
                 `~j-'
********************************************************************/

//Programme normal
void loopRelease()
{


  matrix.aqqKeyboard(); //Perif qui donne accès
  aqqGlove();
  //aqqPiezzo();
  //Fonction qui envoie

  //move.aqqJoystick();
  //move.processJoystick();

  move.aqqFaders();
  //move.processFaders();
  //move.processHead();
  processRemote();

  //try to fix random freeze
  delay(50);
}


//Test Piezzo
void loopTest1()
{
  //aqqPiezzo();
  aqqGlove();
  move.aqqFaders();

  //processHead();
  processRemote();
}

//Test Tete
void loopTest2()
{
  matrix.aqqKeyboard();
  move.aqqFaders();
  //processHead();

}

//Test Inclinomètre
void loopTest3()
{
  //aqqInclino();
  //processInclino();

}

void loop()
{

  //faderL, faderR, head;
  /*Serial.print("L : ");
  Serial.print(analogRead(LIN_L));
  Serial.print(" R : ");
  Serial.print(analogRead(LIN_R));
  Serial.print(" H : ");
  Serial.println(analogRead(HEAD));*/
  digitalWrite(redLed, HIGH);
  digitalWrite(yellowLed, HIGH);
  
  ProcessCommut();
  switch (ConfA)
  {
    case 1: loopRelease(); break;
    case 2: loopTest1(); break;
    case 3: loopTest2(); break;
    case 4: loopTest3(); break;
  }
  //screen stop working in few seconds without this
  if ((screenTime + REFRESH_RATE) < millis()) {
    oled.processLCD(mod, lastMsg);
    screenTime = REFRESH_RATE + millis();
  }

}
