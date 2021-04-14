#include "ArduinoLog.h"
#include <Button.h>
#include "U8glib.h"

//SDA 4 SCL 5
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);  // I2C / TWI
char mod = 'C';

//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

//Mapping mapping;

//index bleu
Button buttonA2 = Button(A2, PULLUP); //F
Button buttonA0 = Button(A0, PULLUP); //D
Button button2 = Button(2, PULLUP);

//majeur blanc
Button button6 = Button(6, PULLUP);
Button buttonA1 = Button(A1, PULLUP); //E
Button button5 = Button(5, PULLUP);

//annulaire jaune
Button button7 = Button(7, PULLUP);
Button button10 = Button(10, PULLUP);//A
Button button8 = Button(8, PULLUP);
//auriculaire vert
Button button12 = Button(12, PULLUP); //C
Button button4 = Button(4, PULLUP);
Button button11 = Button(11, PULLUP); //B

#define buttonSize 12

Button buttons[] = {button2, button4, button5, button6, button7, button8, button10,
                    button11, button12, buttonA0, buttonA1, buttonA2,
                   };

//dead 9 / 6 / 3 / A3 / A6 / A7

char key[2];

void setup()
{
  Serial.begin(115200);
  Log.begin   (LOG_LEVEL, &Serial);
  //analogWrite(VIBRATION, 0);
}

boolean checkButton() {
  for (int i = 0; i < buttonSize; i++) {
    //Serial.print("test "); Serial.println(buttons[i].getPin());
    //    if (buttons[i].uniquePress())
    if (buttons[i].isPressed())
    {
      //Serial.print(buttons[i].getPin()); Serial.println(" pressed");
      int pin = buttons[i].getPin();
      if (pin >= 10) {
        char pinChar;
        if (pin == 10) {
          key[0] = 'A';
          key[1] = 0;
        } else if (pin == 11) {
          key[0] = 'B';
          key[1] = 0;
        } else if (pin == 12) {
          key[0] = 'C';
          key[1] = 0;
        } else if (pin == 14) { //A0
          key[0] = 'D';
          key[1] = 0;
        } else if (pin == 15) { //A1
          key[0] = 'E';
          key[1] = 0;
        } else if (pin == 16) { //A2
          key[0] = 'F';
          key[1] = 0;
        } else if (pin == 17) { //A3
          key[0] = 'G';
          key[1] = 0;
          /*} else if (pin == 18) { //A4
            key[0] = 'F';
            key[1] = 0;
            } else if (pin == 19) { //A5
            key[0] = 'G';
            key[1] = 0;*/
        } else if (pin == 20) { //A6
          key[0] = 'H';
          key[1] = 0;
        } else if (pin == 21) { //A7
          key[0] = 'I';
          key[1] = 0;
        }
      } else {
        itoa(pin, key, 10);
      }

      if (key[0] == '4' || key[0] == 'B' || key[0] == 'C') {
        mod = key[0];
      }

      return true;
    }
  }
  return false;
}

void draw()
{
  u8g.setColorIndex(1);

  //Mode
  u8g.setFont(u8g_font_gdr30r);
  char modScreen[2] = "";
  modScreen[0] = mod;
  u8g.drawStr(3, 30, modScreen);

  //Config
  /*u8g.setFont(u8g_font_gdr14r);
    char conf[6] = "A1 B1";
    conf[1] = '0' + ConfA;
    conf[4] = '0' + ConfB;
    u8g.drawStr(60, 26, conf);*/

  //Message
  u8g.setFont(u8g_font_gdr14r);
  char printKey[3] = {mod};
  printKey[1] = key[0];
  u8g.drawStr(3, 60, printKey);

  //u8g.drawLine(7+a, 10, 40, 55);
}

void loop()
{
  if (checkButton()) {
    Serial.print(mod); Serial.println(key);
  }

  u8g.firstPage();
  do {
    draw();
    u8g.setColorIndex(1);
  } while ( u8g.nextPage() );

  delay(20);
}
