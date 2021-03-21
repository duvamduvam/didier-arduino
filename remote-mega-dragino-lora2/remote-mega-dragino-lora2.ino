
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


#include "ArduinoLog.h"
#include <Keypad.h>
#include <SPI.h>
#include <RH_RF95.h>
#include <Button.h>

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

#define KEYBOARD_DELAY  100
#define RF95_FREQ 868.0  // Change to 434.0 or other frequency, must match RX's freq!


#define JOY_CALIB_DATA_RANGE 100
#define JOY_SEND_MARGIN 10

//Manche radiocommande
#define JOY_CALIB_X_MIN 145
#define JOY_CALIB_X_MAX 927
#define JOY_CALIB_Y_MIN 140
#define JOY_CALIB_Y_MAX 910

/*
  //Joy Playstation
  #define JOY_CALIB_X_MIN 0
  #define JOY_CALIB_X_MAX 1023
  #define JOY_CALIB_Y_MIN 1023
  #define JOY_CALIB_Y_MAX 0
*/


#define PROGRAM_MODE_RELEASE 1
#define PROGRAM_MODE_TEST_1 2
#define PROGRAM_MODE_TEST_2 3
#define PROGRAM_MODE_TEST_3 4

#define ANGLE_INCLINO 30

/********************************************************************
                  PIN MAP
                     _ _ /--\
            \/_ _ _/(_(_(_o o)
             (_(_(_(/      ^
********************************************************************/

/*---------------/
  /     RFM95      /
  /---------------*/

#define RFM95_CS    10
#define RFM95_RST   9
#define RFM95_INT   2

/*---------------/
  /     DIGITAL    /
  /---------------*/
#define PIEZZO      4
#define PIEZZO_GATE 40

#define FADER_ON    22
#define BP_CALIB    24

#define CONF_A_1    26
#define CONF_A_2    28
#define CONF_A_3    30
#define CONF_A_4    32

#define CONF_B_1    34
#define CONF_B_2    36
#define CONF_B_3    38
#define CONF_B_4    40


/*---------------/
  /     ANALOG     /
  /---------------*/

#define JOY_X   A0
#define JOY_Y   A1
#define LIN_L   A2
#define LIN_R   A3
#define PIEZZO  A4

/*---------------/
  /    KEYBOARD    /
  /---------------*/

#define ROWS 3
#define COLS 8
byte rowPins[ROWS] = {33, 31, 29};
byte colPins[COLS] = {49, 47, 45, 43, 41, 39, 37, 35};



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
  {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'},
  {'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P'},
  {'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X'},
};

char mod = 'X';
char key = 0;
char glove1 = 0;

/*-------------+
  |     RADIO    |
  +-------------*/
char msg[20];



/*-------------+
  |     LOGIC    |
  +-------------*/
char ConfA, ConfB;



/*-------------+
  |   INCLINO    |
  +-------------*/
int angleX, angleY, angleZ;
int angleCalibX, angleCalibY, angleCalibZ;

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
int faderL, faderR;
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

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

Inclino inclino;
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );




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

  //RFM95
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

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
  Serial2.begin(9600);
  delay(100);
  Log.begin   (LOG_LEVEL, &Serial);



  /*---------------/
    /     RFM95      /
    /---------------*/

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  rf95.setTxPower(23, false);



}




/********************************************************************
                    DRIVER
                  _____
               .-(_|||_)-.
               "         "
********************************************************************/
void RfOut()
{
  //Output
  int len = strlen(msg);
  rf95.send((uint8_t *)msg, len);
  Serial.print("Sending "); Serial.println(msg);
}

void sendCharMsg(char m, char input) {

  msg[0] = '<';
  msg[1] = m;
  msg[2] = input;
  msg[3] = '>';
  msg[4] = 0;

  RfOut();
}

void sendArrayMsg(char m, char input[]) {

  msg[0] = '<';
  msg[1] = m;
  msg[2] = input[0];
  msg[3] = input[1];
  msg[4] = '>';
  msg[5] = 0;

  RfOut();
}

void sendDecMsg(char m, int value) {

  char charNumber[10] = "";
  itoa ( value, charNumber, 10);
  sendCharMsg(m, charNumber);
}

void sendVectorMsg(char m, int valueX, int valueY) {

  char sX[7], sY[7];
  char prefixe[2] = " ";
  prefixe[0] = m;
  char header[2] = "<";

  msg[0] = 0;
  strcpy(msg, header);
  strcat(msg, prefixe);
  itoa (valueX, sX, 10);
  strcat(msg, sX);
  strcat(msg, ",");
  itoa (valueY, sY, 10);
  strcat(msg, sY);
  strcat(msg, ">");

  RfOut();
}



void sendIntMsg(char m, int value) {

  char sX[7];
  char prefixe[2] = " ";
  prefixe[0] = m;
  char header[2] = "<";

  msg[0] = 0;
  strcpy(msg, header);
  strcat(msg, prefixe);
  itoa (value, sX, 10);
  strcat(msg, sX);
  strcat(msg, ">");

  RfOut();
}


//Abstractions
void sendWheels(int l, int r) {
  sendVectorMsg('W', l, r);
}
void sendHead(int head) {
  sendIntMsg('N', head);
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
  if (faderL > neck + 2 || faderL < neck - 2) {
    sendHead(neck);
    neck = faderL;
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

  //Calibrage Potentiomètres linéaires
  faderL = map(inl, 0, 1024, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);
  faderR = map(inr, 0, 1024, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);

  //Limites en cas de décalibrage
  faderL = BoundInt(faderL, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);
  faderR = BoundInt(faderR, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);

}


void processFaders()
{
  if (digitalRead(FADER_ON) == 0)

    //Seuil d'envoi
    if (faderL > JOY_SEND_MARGIN || faderL <  -JOY_SEND_MARGIN || faderR > JOY_SEND_MARGIN || faderR < -JOY_SEND_MARGIN)
      sendWheels(faderL, faderR);
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
  glove1 = 0;
  while (Serial2.available()) {
    glove1 = Serial2.read();
  }
}


/*  ____          ____
   |                  |
   |     REMOTE     |
   |____          ____|   */
void processRemote()
{

  if (key != NO_KEY)
  {
    if (key == 'U' || key == 'V' || key == 'W' || key == 'X') {
      mod = key;
    }
    //Serial.println(key);
    sendCharMsg(mod, key);
  }

  if (glove1 != 0) {
    //Serial.println(glove1);
    sendCharMsg(mod, glove1);
  }

  if (piezzo > PIEZZO_GATE) {
    sendIntMsg('P', piezzo);
  }

}






/*  ____          ____
   |                  |
   |     TEST MENU     |
   |____          ____|   */
/* Exemple d'utilisation du clavier diférent
  void processMenu()
  {
  switch(key)
  {
  case 'A':
   break:
  }


  }
*/


/*  ____          ____
   |                  |
   |   INCLINOMETRE   |
   |____          ____|   */



void aqqInclino()
{
  inclino.read();
  if (inclino.newData)
  {
    int aX = map(JY901.stcAngle.Angle[0], -32767, 32768, -180, 180);
    int aY = map(JY901.stcAngle.Angle[1], -32767, 32768, -180, 180);
    int aZ = map(JY901.stcAngle.Angle[2], -32767, 32768, -180, 180);

    angleX = aX - angleCalibX;
    angleY = aY - angleCalibY;
    angleZ = aZ - angleCalibZ;

    if (angleX < -180)
      angleX += 360;
    if (angleY < -180)
      angleY += 360;
    if (angleZ < -180)
      angleZ += 360;

    if (digitalRead(BP_CALIB) == 0)
    {
      angleCalibX = aX;
      angleCalibY = aY;
      angleCalibZ = aZ;
    }

    Serial.print("X="); Serial.print(angleX); Serial.print("  ");
    Serial.print("Y="); Serial.print(angleY); Serial.print("  ");
    Serial.print("Z="); Serial.print(angleZ); Serial.println("  ");

    inclino.newData = false;
  }


}

void processInclino()
{
  int x = map(angleX, -ANGLE_INCLINO, ANGLE_INCLINO, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);
  int y = map(angleY, -ANGLE_INCLINO, ANGLE_INCLINO, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);

  x = BoundInt(x, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);
  y = BoundInt(y, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);

  //Seuil d'envoi
  if (x > JOY_SEND_MARGIN || x <  -JOY_SEND_MARGIN || y > JOY_SEND_MARGIN || y < -JOY_SEND_MARGIN)
  {
    //Transformée géométrique
    SpeedVector robot = ToRobot(x, y);
    int l = robot.left * 100;
    int r = robot.right * 100;
    sendWheels(l, r);
    //   Log.notice("Wheels  l:%d r:%d\n", l, r);
  }
}



/*  ____          ____
   |                  |
   |      COMMUT      |
   |____          ____|   */


void ProcessCommut()
{
  if (digitalRead(CONF_A_1) == 0)
    ConfA = 1;
  if (digitalRead(CONF_A_2) == 0)
    ConfA = 2;
  if (digitalRead(CONF_A_3) == 0)
    ConfA = 3;
  if (digitalRead(CONF_A_4) == 0)
    ConfA = 4;

  if (digitalRead(CONF_B_1) == 0)
    ConfB = 1;
  if (digitalRead(CONF_B_2) == 0)
    ConfB = 2;
  if (digitalRead(CONF_B_3) == 0)
    ConfB = 3;
  if (digitalRead(CONF_B_4) == 0)
    ConfB = 4;

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
  u8g.drawStr(3, 60, msg);

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


void LCD_DrawInclinometre()
{
  u8g.setColorIndex(1);


  int mX = map(angleX, -20, 20, 0, 128);
  int mY = map(angleY, 20, -20, 0, 64);


  //Mode
  u8g.setFont(u8g_font_gdr20r);
  u8g.drawStr(mX - 8, mY + 10, "o");
}


void draw(void)
{
  switch (ConfA)
  {
    case PROGRAM_MODE_RELEASE: LCD_Page1(); break;
    case PROGRAM_MODE_TEST_1: LCD_Piezzo(); break;
    case PROGRAM_MODE_TEST_2: LCD_TestTete(); break;
    case PROGRAM_MODE_TEST_3: LCD_DrawInclinometre(); break;
  }

}


void processLCD()
{
  u8g.firstPage();
  do {
    draw();
    u8g.setColorIndex(1);
  } while ( u8g.nextPage() );

  // delay(30);
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
//  aqqGlove();
  aqqPiezzo();
  processRemote(); //Fonction qui envoie

  aqqJoystick();
  processJoystick();

  aqqFaders();
  processFaders();
}


//Test Piezzo
void loopTest1()
{
  aqqPiezzo();
  processRemote();

}

//Test Tete
void loopTest2()
{
  aqqKeyboard();
  aqqFaders();
  processHead();

}

//Test Inclinomètre
void loopTest3()
{
  aqqInclino();
  processInclino();


}


void loop()
{
  ProcessCommut();
  switch (ConfA)
  {
    case PROGRAM_MODE_RELEASE: loopRelease(); break;
    case PROGRAM_MODE_TEST_1: loopTest1(); break;
    case PROGRAM_MODE_TEST_2: loopTest2(); break;
    case PROGRAM_MODE_TEST_3: loopTest3(); break;
  }
  processLCD();
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
