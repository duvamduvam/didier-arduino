#include "ArduinoLog.h"
#include <Button.h>
#include "Vibrator.cpp"

//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

//Mapping mapping;
Vibrator vibrator;

Button button2 = Button(2, INPUT_PULLUP); //F
Button button3 = Button(A0, INPUT_PULLUP); //D
Button button4 = Button(4, INPUT_PULLUP);

Button button5 = Button(5, INPUT_PULLUP);
Button button6 = Button(6, INPUT_PULLUP); //E
Button button7 = Button(7, INPUT_PULLUP);

Button button8 = Button(8, INPUT_PULLUP);
Button button9 = Button(9, INPUT_PULLUP);//A
Button button10 = Button(10, INPUT_PULLUP);

Button button11 = Button(11, INPUT_PULLUP); //C
Button button12 = Button(12, INPUT_PULLUP);
Button buttonA1 = Button(A1, INPUT_PULLUP); //B

#define buttonSize 12

Button buttons[] = {button2, button3, button4, button5, button6, button7, button8, button9, button10,
                    button11, button12, buttonA1
                   };

//dead 9 / 6 / 3 / A3 / A6 / A7

char key;
char msg[5];


void setup()
{
  Serial.begin(9600);
  Log.begin   (LOG_LEVEL, &Serial);
  Log.notice("start glove right button nano");
  vibrator.init();
  msg[0] = '<';
  msg[1] = '0';
  msg[2] = '>';
  msg[3] = '\0';

}

boolean checkButton() {
  for (int i = 0; i < buttonSize; i++) {
    //Serial.print("test "); Serial.println(buttons[i].getPin());
    //    if (buttons[i].uniquePress())
    if (buttons[i].isPressed())
    {
      //Serial.print(buttons[i].getPin()); Serial.println(" pressed");
      int pin = buttons[i].getPin();

      switch (pin) {
        case 5 :
          key = 'A';
          break;
        case 11 :
          key = 'B';
          break;
        case 12 :
          key = 'C';
          break;
        case 8 :
          key = 'D';
          break;
        case 7 :
          key = 'E';
          break;
        case 6 :
          key = 'F';
          break;
        case 2 :
          key = 'G';
          break;
        case 14 :
          key = 'H';
          break;
        case 9 :
          key = 'I';
          break;
        case 4 :
          key = 'J';
          break;
        case 10 :
          key = 'K';
          break;
        case 15 :
          key = 'L';
          break;
      }
      return true;
    }
  }
  return false;
}

void loop()
{
  //Serial.println("<WQ>");
  //delay(1000);

  if (checkButton()) {
    //Serial.print("key : ");Serial.println(key);
    msg[1] = key;
    Serial.println(msg);
    vibrator.press();
    delay(50);
  }

  vibrator.execute();

}
