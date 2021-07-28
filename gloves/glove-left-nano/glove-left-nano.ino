#include "ArduinoLog.h"
#include <Button.h>
//#include "Vibrator.h"

//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

#define VIBRATION_PIN 3

//Vibrator vibrator(VIBRATION_PIN);
//Mapping mapping;

//index bleu
Button buttonA2 = Button(A2, INPUT_PULLUP); //F
Button buttonA0 = Button(A0, INPUT_PULLUP); //D
Button button2 = Button(2, INPUT_PULLUP);

//majeur blanc
Button button6 = Button(6, INPUT_PULLUP);
Button buttonA1 = Button(A1, INPUT_PULLUP); //E
Button button5 = Button(5, INPUT_PULLUP);

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

//dead 9 / 3 / A3 / A6 / A7

char msg[4];
char key;

void setup()
{
  Serial.begin(115200);
  Log.begin   (LOG_LEVEL, &Serial);
  Log.notice("start glove left");

  //pinMode(VIBRATION_PIN, OUTPUT);
  //analogWrite(VIBRATION, 0);

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
      Serial.print(buttons[i].getPin()); Serial.println(" pressed");
      int pin = buttons[i].getPin();
      switch (pin) {
        case 2 :
          key = 'l';
          break;
        case 4 :
          key = 'c';
          break;
        case 5 :
          key = 'h';
          break;
        case 6 :
          key = 'i';
          break;
        case 7 :
          key = 'd';
          break;
        case 8 :
          key = 'e';
          break;
        case 10 :
          key = 'f';
          break;
        case 11 :
          key = 'a';
          break;
        case 12 :
          key = 'b';
          break;
        case 14 :
          key = 'g';
          break;
        case 15 :
          key = 'k';
          break;
        case 16 :
          key = 'j';
          break;
        /*case 11 :
          key = 'a';
          break;
        case 12 :
          key = 'b';
          break;*/
      }
      return true;
    }
  }
  return false;
}

void loop()
{
  if (checkButton()) {
    //Serial.print("key : ");Serial.println(key);
    msg[1] = key;
    Serial.println(msg);
    //vibrator.execute(100, 200);
  }

  //vibrator.process();

  delay(20);
}
