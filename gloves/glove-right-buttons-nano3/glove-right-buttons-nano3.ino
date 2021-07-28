#include <Keypad.h>
#include "ArduinoLog.h"
#include <Button.h>
#include "Vibrator.cpp"

//#define LOG_LEVEL LOG_LEVEL_SILENT
//#define LOG_LEVEL LOG_LEVEL_ERROR
#define LOG_LEVEL LOG_LEVEL_VERBOSE

//Mapping mapping;
Vibrator vibrator;

const byte rows = 3; //four rows
const byte cols = 4; //three columns
char keys[rows][cols] = {
  {'F', 'I', 'L', 'C'},
  {'E', 'H', 'K', 'A'},
  {'D', 'G', 'J', 'B'},
};
byte rowPins[rows] = {8, A0, 6}; //connect to the row pinouts of the keypad
byte colPins[cols] = {2, 7, A1, 10}; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, rows, cols );

/*Button button2 = Button(2, INPUT_PULLUP); //F
  Button button3 = Button(A0, INPUT_PULLUP); //D
  Button buttonA1 = Button(A1, INPUT_PULLUP); //
  Button button10 = Button(10, INPUT_PULLUP);

  Button button6 = Button(6, INPUT_PULLUP); //E
  Button button7 = Button(7, INPUT_PULLUP);
  Button button8 = Button(8, INPUT_PULLUP);



  Button button4 = Button(4, INPUT_PULLUP);

  Button button5 = Button(5, INPUT_PULLUP);

  Button button9 = Button(9, INPUT_PULLUP);//A


  Button button11 = Button(11, INPUT_PULLUP); //C
  Button button12 = Button(12, INPUT_PULLUP);
  B*/

/*#define buttonSize 12

  Button buttons[] = {button2, button3, button4, button5, button6, button7, button8, button9, button10,
                    button11, button12, buttonA1
                   };**/

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

void loop()
{
  char key = keypad.getKey();

  if (key != NO_KEY) {
    vibrator.press();
    Serial.print("<"); Serial.print(key); Serial.println(">");
  }

  vibrator.execute();

}
