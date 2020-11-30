#include <SPI.h>
#include <SD.h>
#include <FTOLED.h>
#include <fonts/SystemFont5x7.h>
#include <fonts/Arial14.h>
#include <fonts/Arial_Black_16.h>
#include <fonts/Droid_Sans_36.h>

#include <Button.h>

// NB: This sketch won't fit on older Arduino Duemilanoves or any other
// Arduino with less than 32k of onboard flash storage.
//
// If the sketch doesn't fit, disable one of the fonts by deleting one of
// the blocks below

//button


//int buttons[] = {22, 23, 24,25,26,27};



//screen
const byte pin_cs = 7;
const byte pin_dc = 2;
const byte pin_reset = 3;

OLED oled(pin_cs, pin_dc, pin_reset);

Button button22 = Button(22, PULLUP);
Button button23 = Button(23, PULLUP);
Button button24 = Button(24, PULLUP);
Button button25 = Button(25, PULLUP);
Button button26 = Button(26, PULLUP);
Button button27 = Button(27, PULLUP);
#define buttonSize 6

Button buttons[] = {button22, button23, button24, button25, button26, button27};

void setup() {
  Serial.begin(9600);
  Serial.print("init...");


  //pinMode(23, INPUT_PULLUP);

  oled.begin();

  /*oled.selectFont(Arial14);
    oled.drawString(6, 20, F("Arial 14"), VIOLET, BLACK);

    oled.selectFont(Arial_Black_16);
    oled.drawString(6, 38, F("Arial Black 16"), WHITE, BLACK);
  */
  oled.selectFont(SystemFont5x7);
  oled.drawString(6, 70, F("System 5x7\nOn Two Lines"), RED, BLACK);

  //oled.selectFont(Droid_Sans_36);
  oled.drawString(6, 78, F("DS 36"), GREEN, BLACK);

  // Finally, use the String object to build a dynamic string
  // instead of the previous strings which were all constant
  // "character arrays" (always the same value)
  String msg = "Sketch ran in\n";
  msg += millis();
  msg += " ms";
  oled.selectFont(SystemFont5x7);
  oled.drawString(6, 8, msg, WHITE, BLACK);

  //

}

int checkButton() {

  //bool buttonPressed = false;
  //int buttonSize = sizeof(buttons) - 1;
  //Serial.print("buttons size "); Serial.println(buttonSize);
  for (int i = 0; i < buttonSize; i++) {
    //Serial.print("test "); Serial.println(buttons[i].getPin());
    if (buttons[i].uniquePress()) {
      Serial.print(buttons[i].getPin()); Serial.println(" pressed");
      return buttons[i].getPin();
      //Serial.print(buttons[i]); Serial.println(" pressed");
      //lastPressed = buttons[i];
      //buttonPressed = true;
    }
  }

  /*if (!buttonPressed && lastPressed != 0) {
    Serial.print(lastPressed); Serial.println(" released");
    lastPressed = 0;
    }*/

}

void loop() {
  // Do nothing, we're finished!


  checkButton();

  //int buttonValue = digitalRead(23);
  //Serial.print(" 23 : "); Serial.println(buttonValue);
  /*oled.selectFont(Arial14);
    if (buttonValue == 0) {
    oled.drawString(6, 20, F("buckowski"), VIOLET, BLACK);
    Serial.println("pressed");
    } else {
    oled.drawString(6, 20, F("sarkozi"), VIOLET, BLACK);
    //Serial.println("sarkozi");
    }*/


  /*for (int i = 0; i < sizeof(buttons)-1; i++) {
    //Serial.print("test "); Serial.print(buttons[i]); Serial.print("value "); Serial.println(digitalRead(buttons[i]));
    if (digitalRead(buttons[i]) == 0) {
      Serial.print(buttons[i]); Serial.println(" pressed");
    }
    }*/

  delay(30);

}
