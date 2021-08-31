
/*_____________________________________
  ________|                                      |_______
  \       |         REMOTE MEGA DRAGINO          |      /
  \      |                                      |     /
  /      |______________________________________|     \
  /__________)                                (_________\


  /*_________
  /          /|
  /__________/  |
  |________ |   |
  /_____  /||   |
  |Include| ||   |
  |_______|/ |   |
  || .___."||  /
  ||_______|| /
  |_________*/

#include "FonctionsMega.h"
#include "ArduinoLog.h"
#include <Keypad.h>
#include <SPI.h>
#include <Button.h>
#include <Radio.h>

//oled
#include "U8glib.h"

//INCLINO
#include "Inclino.cpp"

//User
#include "Fonctions.h"
/********************************************************************
              USER CONFIG
               _________
              /  .....  |
              |  .....  |
              |  .CFG.  |
              |  .....  |
              |_________|
********************************************************************/

//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#define KEYBOARD_DELAY  10
#define REFRESH_RATE 50
long screenTime;

#define JOY_CALIB_DATA_RANGE 100
#define JOY_SEND_MARGIN 10

//Manche radiocommande
#define JOY_CALIB_X_MIN 145
#define JOY_CALIB_X_MAX 927
#define JOY_CALIB_Y_MIN 140
#define JOY_CALIB_Y_MAX 910

#define FIRST_CHAR_NB 33

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

char lastMsg[10];

/*---------------/
  /     DIGITAL    /
  /---------------*/
#define PIEZZO_GATE 40

#define FADER_ON    A13

#define CONF_A_1    22
#define CONF_A_2    24
#define CONF_A_3    26
#define CONF_A_4    40

#define CONF_B_1    35
#define CONF_B_2    41
#define CONF_B_3    43
#define CONF_B_4    45


/*---------------/
  /     ANALOG     /
  /---------------*/

#define JOY_X   A0
#define JOY_Y   A1
#define LIN_L   A11
#define LIN_R   A10
#define HEAD  A9
#define PIEZZO  A4

/*---------------/
  /    KEYBOARD    /
  /---------------*/

#define ROWS 4
#define COLS 4
byte rowPins[ROWS] = {29, 27, 25, 23};
byte colPins[COLS] = {39, 37, 33, 31};



/********************************************************************
                      GLOBAL VAR
                  __(.)< __(.)> __(.)=
                  \___)  \___)  \___)
********************************************************************/

/*-------------+
  |   KEYBOARD   |
  +-------------*/

char keys[ROWS][COLS] =
{
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'},
};

char mod = 'A';
char key, holdKey;
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

/*-------------+
  |    NECK    |
  +-------------*/
int neck;


/*-------------+
  |     ANALOG    |
  +-------------*/
int faderL, faderR, head;
int joyX, joyY;

/********************************************************************
                       INSTALL CLASS
                                    ____________________________
   _____                          ,\\    ___________________    \
  |     `------------------------'  ||  (___________________)   `|
  |_____.------------------------._ ||  ____________________     |
                                  `//__(____________________)___/

********************************************************************/

//oled
//SDA 4 SCL 5
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);  // I2C / TWI

Inclino inclino;
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

Radio radio;
bool newData = false;


/**********************************************************************************************************************
***********************************************************************************************************************
***********************************************************************************************************************
**   DEBUT CODE   ****************************************************************************************************/





/********************************************************************
                 SETUP
               _________
              /  .....  |
              |  .....  |
              |  .CFG.  |
              |  .....  |
              |_________|
********************************************************************/
void setup()
{
  draw();

  /*---------------/
    /    PIN INIT    /
    /---------------*/

  //BP
  pinMode(FADER_ON, INPUT_PULLUP);
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

  /*---------------/
    /     SERIAL     /
    /---------------*/

  while (!Serial);
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial3.begin(115200);
  delay(100);
  Log.begin   (LOG_LEVEL, &Serial);
  Serial.println("start modem");

  radio.init();
}




/********************************************************************
                    DRIVER
                  _____
               .-(_|||_)-.
               "         "
********************************************************************/



//Abstractions
void sendWheels(int l, int r) {
  radio.sendVectorMsg('W', l, r);
}
void sendHead(int head) {
  radio.sendIntMsg('N', head);
}



/********************************************************************
                _.,----,._
              .:'        `:.
            .'              `.
           .'     PROCESS    `.
           :                  :
           `    .'`':'`'`/    '
            `.   \  |   /   ,'
              \   \ |  /   /
               `\_..,,.._/'
                {`'-,_`'-}
                {`'-,_`'-}
                {`'-,_`'-}
                 `YXXXXY'
                   ~^^~
********************************************************************/




/*  ____          ____
   |                  |
   |      HEAD        |
   |____          ____|   */

void processHead()
{
  if (head > neck + 2 || head < neck - 2) {
    sendHead(neck);
    neck = head;
  }
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
   |      FADERS      |
   |____          ____|   */

void aqqFaders()
{

  int inl = analogRead(LIN_L);
  int inr = analogRead(LIN_R);
  int inh = analogRead(HEAD);

  //Log.notice("fader l %d fader r %d head %d\n", inl, inr, inh);

  //Calibrage Potentiomètres linéaires
  faderL = map(inl, 1024, 0, FIRST_CHAR_NB, 126);
  faderR = map(inr, 1024, 0, FIRST_CHAR_NB, 126);
  head = map(inh, 1024, 0, FIRST_CHAR_NB, 126);
  //Log.notice("fader head : %i -> mapping -> %i\n", inh, head);


  //Limites en cas de décalibrage
  /*faderL = BoundInt(faderL, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);
    faderR = BoundInt(faderR, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);
    head = BoundInt(head, 0, 270);*/
}


void processFaders()
{
  if (digitalRead(FADER_ON) == 0)

    //Seuil d'envoi
    if (faderL > JOY_SEND_MARGIN || faderL <  -JOY_SEND_MARGIN || faderR > JOY_SEND_MARGIN || faderR < -JOY_SEND_MARGIN)
      sendWheels(-faderL, faderR);
  sendHead(head);
}

/*  ____          ____
   |                  |
   |     JOYSTICK     |
   |____          ____|   */
void aqqJoystick()
{
  int inx = analogRead(JOY_X);
  int iny = analogRead(JOY_Y);

  //Calibrage Manche radiocommande
  joyX = map(inx, JOY_CALIB_X_MIN, JOY_CALIB_X_MAX, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);
  joyY = map(iny, JOY_CALIB_Y_MIN, JOY_CALIB_Y_MAX, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);

  //Limites en cas de décalibrage
  joyX = BoundInt(joyX, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);
  joyY = BoundInt(joyY, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);


}

void processJoystick()
{
  if (mod != 'W')return;

  //Seuil d'envoi
  if (joyX > JOY_SEND_MARGIN || joyX <  -JOY_SEND_MARGIN || joyY > JOY_SEND_MARGIN || joyY < -JOY_SEND_MARGIN)
  {
    //Transformée géométrique
    SpeedVector robot = ToRobot(joyX, joyY);
    int l = robot.left * 100;
    int r = robot.right * 100;
    sendWheels(l, r);
  }
}


/*  ____          ____
   |                  |
   |     KEYBOARD     |
   |____          ____|   */
//Aquisition uniquement
void aqqKeyboard() {
  key = keypad.getKey();




}

char aqqGlove() {
  bool newData1 = recvWithStartEndMarkersMega(glove1, false,  3, 1);
  bool newData2 = recvWithStartEndMarkersMega(glove2, false,  3, 3);
  /*if (newData1) {
    Log.notice("glove 1 : %s", glove1);
    } else if (newData2) {
    Log.notice("glove 2 : %s", glove2);
    }*/
}

/*  ____          ____
   |                  |
   |     REMOTE     |
   |____          ____|   */
void processRemote() {
  char radioMsg[10] = {};

  if (digitalRead(FADER_ON) == 0) {
    Log.notice("faderL : %d | faderR : %d | head : %d\n", faderL, faderR, head);
    radioMsg[0] = faderL;
    radioMsg[1] = faderR;
    radioMsg[2] = head;
    newData = true;
  } else {
    radioMsg[0] = ' ';
    radioMsg[1] = ' ';
    radioMsg[2] = ' ';
  }
  radioMsg[3] = ' ';
  radioMsg[4] = ' ';
  radioMsg[5] = '\0';

  if (key != NO_KEY) {
    holdKey = key;
  }

  if ((key != NO_KEY) || (keypad.getState() == HOLD))
  {
    newData = true;

    if (key == 'A' || key == 'B' || key == 'C' || key == 'D') {
      mod = key;
    }
    radioMsg[3] = mod;
    radioMsg[4] = holdKey;
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

/*  ____          ____
   |                  |
   |        LCD       |
   |____          ____|   */

void LCD_Page1()
{

  u8g.setColorIndex(1);

  //Mode
  u8g.setFont(u8g_font_gdr30r);
  char modScreen[2] = "";
  modScreen[0] = mod;
  u8g.drawStr(3, 30, modScreen);

  //Config
  u8g.setFont(u8g_font_gdr14r);
  char conf[6] = "A1 B1";
  conf[1] = '0' + ConfA;
  conf[4] = '0' + ConfB;
  u8g.drawStr(60, 26, conf);

  //Message
  u8g.setFont(u8g_font_gdr14r);
  u8g.drawStr(3, 60, lastMsg);

  //Log.notice("LCD radio.msg : %s\n",radio.msg);

  //u8g.drawLine(7+a, 10, 40, 55);
}


void LCD_TestTete()
{
  u8g.setColorIndex(1);
  int x = map(neck, -100, 100, 0, 126);
  u8g.setFont(u8g_font_gdr14r);
  char title[10] = "Tete";
  u8g.drawStr(3, 15, title);

  u8g.drawRBox(1, 20, x + 5, 44, 1);
}


void LCD_Piezzo()
{
  u8g.setColorIndex(1);
  u8g.setFont(u8g_font_gdr14r);

  char  piezzoChar[4];
  itoa(piezzo, piezzoChar, 10);

  char title[15] = "Piezzo ";
  arrayConcat(title, 7, piezzoChar, 4);
  //Log.notice(" piezzo value = %d title %s\n xChar %d", piezzo, title, piezzoChar);
  u8g.drawStr(3, 15, title);

  u8g.drawRBox(1, 20, piezzo + 5, 44, 1);

}


/*void LCD_DrawInclinometre()
  {
  u8g.setColorIndex(1);


  int mX = map(angleX, -20, 20, 0, 128);
  int mY = map(angleY, 20, -20, 0, 64);


  //Mode
  u8g.setFont(u8g_font_gdr20r);
  u8g.drawStr(mX - 8, mY + 10, "o");
  }*/


void draw(void)
{
  switch (ConfA)
  {
    case 1: LCD_Page1(); break;
    case 2: LCD_Piezzo(); break;
    case 3: LCD_TestTete(); break;
      //case 4: LCD_DrawInclinometre(); break;
  }

}


void processLCD()
{
  u8g.firstPage();
  do {
    draw();
    u8g.setColorIndex(1);
  } while ( u8g.nextPage() );

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
  aqqKeyboard(); //Perif qui donne accès
  aqqGlove();
  //aqqPiezzo();
  //Fonction qui envoie

  //aqqJoystick();
  //processJoystick();

  aqqFaders();
  //processFaders();
  //processHead();
  processRemote();
}


//Test Piezzo
void loopTest1()
{
  //aqqPiezzo();
  aqqGlove();
  aqqFaders();

  //processHead();
  processRemote();
}

//Test Tete
void loopTest2()
{
  aqqKeyboard();
  aqqFaders();
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
  ProcessCommut();
  switch (ConfA)
  {
    case 1: loopRelease(); break;
    case 2: loopTest1(); break;
    case 3: loopTest3(); break;
    case 4: loopTest3(); break;
  }
  //screen stop working in few seconds without this
  if ((screenTime + REFRESH_RATE) < millis()) {
    processLCD();
    screenTime = REFRESH_RATE + millis();
  }

}




/*    FIN CODE    *****************************************************************************************************
***********************************************************************************************************************
***********************************************************************************************************************
**********************************************************************************************************************/





/*///
  ( o o )
  +-----.oooO--(_)--Oooo.-------+
  |                             |
  |  By David Dubreuil          |
  |                             |
  |                             |
  |       .oooO           2021  |
  |       (   )   Oooo.         |
  +--------\ (----(   )---------+
  \_)  ) /
    (*/
