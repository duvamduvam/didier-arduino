#include "ArduinoLog.h"
#include <Button.h>


//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

//Mapping mapping;

//Button button2 = Button(2, PULLUP);
Button button3 = Button(3, PULLUP);
Button button4 = Button(4, PULLUP);
Button button5 = Button(5, PULLUP);
Button button6 = Button(6, PULLUP);
Button button7 = Button(7, PULLUP);
Button button8 = Button(8, PULLUP);
Button button9 = Button(9, PULLUP);
Button button10 = Button(10, PULLUP);
Button button11 = Button(11, PULLUP);
Button button12 = Button(12, PULLUP);
//Button button13 = Button(13, PULLUP);

#define buttonSize 12

Button buttons[] = {button3, button4, button5, button6, button7, button8, button9,
                    button10, button11, button12,
                   };

void setup()
{
  Serial.begin(115200);
  Log.begin   (LOG_LEVEL, &Serial);
}

int checkButton() {
  for (int i = 0; i < buttonSize; i++) {
    //Serial.print("test "); Serial.println(buttons[i].getPin());
    //    if (buttons[i].uniquePress())
    if (buttons[i].isPressed())
    {
      //Serial.print(buttons[i].getPin()); Serial.println(" pressed");
      return buttons[i].getPin();
    }
  }
  return 0;
}

void loop()
{
  int key = checkButton();
  if (key != 0) {
    Serial.print(key);
  }

}
