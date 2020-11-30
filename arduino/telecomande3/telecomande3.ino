// LoRa 9x_TX8

// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example LoRa9x_RX

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

char charVoltage[20];
char msg[4];
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
  Serial.begin(9600);
  delay(100);

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

void sendMsg(int msg) {

  char charNumber[10] = "";
  itoa ( msg, charNumber, 10);
  Serial.print("int to char "); Serial.println(charNumber);
  sendMsg(charNumber);
}

void sendMsg(char input[]) {

  msg[0] = '$';
  msg[1] = mod[0];
  msg[2] = input[0];
  msg[3] = input[1];
  Serial.print("Sending "); Serial.println(msg);
  rf95.send((uint8_t *)msg, 4);
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



  u8g.firstPage();
  do {
    draw();
    u8g.setColorIndex(1);
  } while ( u8g.nextPage() );



  if (buttonPressed == 48)
    mod = "A";
  else if (buttonPressed == 42)
    mod = "B";
  else if (buttonPressed == 49)
    mod = "C";
  else if (buttonPressed == 43)
    mod = "D";

  delay(30);
}
