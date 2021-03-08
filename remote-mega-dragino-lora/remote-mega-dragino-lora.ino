#include "ArduinoLog.h"
#include "Fonctions.h"

//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#include <SPI.h>
#include <RH_RF95.h>

#define RFM95_CS 10
#define RFM95_RST 9
#define RFM95_INT 2

#include <Button.h>


#define KEYBOARD_DELAY  100


// Change to 434.0 or other frequency, must match RX's freq!
#define RF95_FREQ 868.0

//oled
#include "U8glib.h"
//SDA 4 SCL 5
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);  // I2C / TWI

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);




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


/// JOYSTICK ///
const int JOY_X = A0; // analog pin connected to X output
const int JOY_Y = A1; // analog pin connected to Y output
char joystick[3];


char charVoltage[20];
char msg[20];
char* mod = "A";

Button button22 = Button(22, PULLUP);
Button button23 = Button(23, PULLUP);
Button button24 = Button(24, PULLUP);
Button button25 = Button(25, PULLUP);
Button button26 = Button(26, PULLUP);
Button button27 = Button(27, PULLUP);
Button button28 = Button(28, PULLUP);
Button button29 = Button(29, PULLUP);
Button button30 = Button(30, PULLUP);
Button button31 = Button(31, PULLUP);
Button button32 = Button(32, PULLUP);
Button button33 = Button(33, PULLUP);
Button button34 = Button(34, PULLUP);
Button button35 = Button(35, PULLUP);
Button button36 = Button(36, PULLUP);
Button button37 = Button(37, PULLUP);
Button button38 = Button(38, PULLUP);
Button button39 = Button(39, PULLUP);
Button button40 = Button(40, PULLUP);
Button button41 = Button(41, PULLUP);
Button button42 = Button(42, PULLUP);
Button button43 = Button(43, PULLUP);
Button button44 = Button(44, PULLUP);
Button button45 = Button(45, PULLUP);
Button button46 = Button(46, PULLUP);
Button button47 = Button(47, PULLUP);
Button button48 = Button(48, PULLUP);
Button button49 = Button(49, PULLUP);
Button button50 = Button(50, PULLUP);
Button button51 = Button(51, PULLUP);
Button button52 = Button(52, PULLUP);
Button button53 = Button(53, PULLUP);
Button button54 = Button(54, PULLUP);
#define buttonSize 30

Button buttons[] = {button22, button23, button24, button25, button26, button27, button28, button29,
                    button30, button31, button32, button33, button34, button35, button36, button37, button38, button39,
                    button40, button41, button42, button43, button44, button45, button46, button47, button48, button49,
                    button51, button53,
                   };

void setup()
{
  draw();

  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);
  while (!Serial);
  Serial.begin(115200);
  delay(100);

  Log.begin   (LOG_LEVEL, &Serial);

  Serial.println("Arduino LoRa Remote");

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

int16_t packetnum = 0;  // packet counter, we increment per xmission

void sendJoystick(int x, int y) {

  //Hearder
  msg[0] = '<';
  msg[1] = 'J';
  msg[2] = 0;

  char* packet = Concat16(msg, IntToAsciiInt(x));
  packet = Concat16(packet,",");
  packet = Concat16(packet, IntToAsciiInt(y));
  packet = Concat16(packet, ">");

  Serial.println(packet);
  int len = CharArrayLength(packet,16);
  Serial.println(len);
  rf95.send((uint8_t *)packet, len+5);

//  Serial.print("Sending "); Serial.println(msg);

}

//Commande directe des moteurs (-100 à 100)
void sendWheels(int l, int r) 
{

  //Hearder
  msg[0] = '<';
  msg[1] = 'W';
  msg[2] = 0;

  char* packet = Concat16(msg, IntToAsciiInt(l));
  packet = Concat16(packet,",");
  packet = Concat16(packet, IntToAsciiInt(r));
  packet = Concat16(packet, ">");

  Serial.println(packet);
  int len = CharArrayLength(packet,16);
  Serial.println(len);
  rf95.send((uint8_t *)packet, len+5);

//  Serial.print("Sending "); Serial.println(msg);

}


void sendMsg(int msg) {

  char charNumber[10] = "";
  itoa ( msg, charNumber, 10);
  Serial.print("int to char "); Serial.println(charNumber);
  sendMsg(charNumber);
}

void sendMsg(char input[]) {

  msg[0] = '<';
  msg[1] = mod[0];
  msg[2] = input[0];
  msg[3] = input[1];
  msg[4] = '>';

  rf95.send((uint8_t *)msg, 5);
  Serial.print("Sending "); Serial.println(msg);
}



void updateJoystick() {

  int inx = analogRead(JOY_X);
  int iny = analogRead(JOY_Y);

  //Manche radiocommande
  int x = map(inx, JOY_CALIB_X_MIN, JOY_CALIB_X_MAX, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);
  int y = map(iny, JOY_CALIB_Y_MIN, JOY_CALIB_Y_MAX, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);

  x = BoundInt(x, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);
  y = BoundInt(y, -JOY_CALIB_DATA_RANGE, JOY_CALIB_DATA_RANGE);


  if (x > JOY_SEND_MARGIN || x <  -JOY_SEND_MARGIN || y > JOY_SEND_MARGIN || y < -JOY_SEND_MARGIN) 
  {
    SpeedVector robot = ToRobot(x, y);
    int l = robot.left * 100;
    int r = robot.right* 100;
    sendWheels(l, r); 
    Log.notice("Wheels  l:%d r:%d\n", l, r);

    
    //sendJoystick(x, y);
    //Log.notice("Joystick x:%d y:%d\n", inx, iny);
//    Log.notice("Joystick x:%d y:%d\n", x, y);
  }

}

void draw(void)
{
  delay(10);

  u8g.setColorIndex(1);
  u8g.setFont(u8g_font_gdr30r);
  u8g.drawStr(3, 50, mod);

  u8g.setFont(u8g_font_gdr14r);
  u8g.drawStr(60, 20, msg);

  int sensorValue = analogRead(A0); //read the A0 pin value
  float voltage = sensorValue * (5.00 / 1023.00) * 2; //convert the value to a true voltage.

  //max 8.4V min 6V
  int voltagePourcentage = (((voltage - 6) / 2.4) * 100);

  sprintf(charVoltage, "%d", voltagePourcentage); // %f au lieu de %d
  charVoltage[2] = '%';
  //Serial.print(voltagePourcentage); Serial.print(" "); Serial.println(charVoltage);
  u8g.drawStr(60, 55, charVoltage);
}

int checkButton() {
  for (int i = 0; i < buttonSize; i++) {
    //Serial.print("test "); Serial.println(buttons[i].getPin());
    //    if (buttons[i].uniquePress())
    if (buttons[i].isPressed())
    {
      Serial.print(buttons[i].getPin()); Serial.println(" pressed");
      return buttons[i].getPin();
    }
  }
  return 0;
}

long lastKeyboardTc;
int buttonPressed;

void loop()
{
  if (millis() - lastKeyboardTc > KEYBOARD_DELAY)
  {
    lastKeyboardTc = millis();

    buttonPressed = checkButton();
    if (buttonPressed != 0)
    {
      sendMsg(buttonPressed);
    }

  }

  //updateJoystick();

  u8g.firstPage();
  do {
    draw();
    u8g.setColorIndex(1);
  } while ( u8g.nextPage() );



  if (buttonPressed == 31)
    mod = "A";
  else if (buttonPressed == 37)
    mod = "B";
  else if (buttonPressed == 30)
    mod = "C";
  else if (buttonPressed == 36)
    mod = "D";

  delay(30);
}
