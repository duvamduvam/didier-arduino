

#include <SoftwareSerial.h>

//radio transmission
SoftwareSerial hc12(2, 3); //RX, TX

// leds
int led1State = 0;
int led1 = 4;
int led2 = 5;
int led3 = 6;

//buttons
int digitBut[] = {8, 9, 10, 11, 12, 13};
int anlogBut[] = {0, 1, 2, 3, 4, 5};

boolean isAnalogPressed(int i) {
  if (analogRead(i) > 1000) {
    return true;
  }
  return false;
}

/* button config */
int buttonValue() {
  int value = -1;

  for (int i = 0; i < 6; i++) {
    //Serial.println(digitBut[i]);
    if (digitalRead(digitBut[i]) == HIGH) {
      value = digitBut[i] - 4;
      return value;
    }
  }

  for (int i = 0; i < 7; i++) {
    if (isAnalogPressed(i) == true) {
      if (i == 0) {
        return 111;
      } else if (i == 5) {
        return 222;
      } else if (i == 4) {
        return 333;
      }
      return i - 1;
    }
  }
  return value;
}


String buttonValueMod() {
  int button = buttonValue();
  if (button != -1) {
    return getModValue() + (String)buttonValue();
  }
  return "-1";
}

/* end button */

void manageLed(int button) {

  if (button == 111) {
    ledOnOff(led1);
  } else if (button == 222) {
    ledOnOff(led2);
  } else if (button == 333) {
    ledOnOff(led3);
  }
}

void ledOnOff(int led) {
  if (digitalRead(led) == LOW) {
    digitalWrite(led, HIGH);
  } else {
    digitalWrite(led, LOW);
  }
  delay(200);
}

int getModValue() {
  //String bin = (String)digitalRead(led1) + (String)digitalRead(led2) + (String)digitalRead(led3);
  String bin = (String)digitalRead(led1) + 0 + 0;
  int hexa = 0;
  if (bin == "000") {
    hexa = 0;
  } else if (bin == "001") {
    hexa = 1;
  } else if (bin == "010") {
    hexa = 2;
  } else if (bin == "011") {
    hexa = 3;
  } else if (bin == "100") {
    hexa = 4;
  } else if (bin == "101") {
    hexa = 5;
  } else if (bin == "110") {
    hexa = 6;
  } else {
    hexa = 7;
  }
  return  hexa;
}

/* Joystick */
int getJystickValue() {
  //Serial.println(digitalRead(SW_pin));
  int x = simplifyAnalogValue(analogRead(7));
  int y = simplifyAnalogValue(analogRead(6));

  if (x > 3 && x < 7 && y > 3 && y < 7) {
    return 0;
  }
  // printDebug(String(x), "x");
  //printDebug(String(y), "y");
  if (x == 0) {
    x = 10;
  } else {
    x = x * 10;
  }
  return  x + y;
}

int simplifyAnalogValue(int v) {
  return ((v * 10) / 1024);
}

/*  End Joystick */


void printDebug(String value, String msg) {
  Serial.println(msg);
  Serial.println(value);
}

void setup() {

  // radio transmission
  hc12.begin(9600);
  Serial.begin(9600);

  //leds
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  for (int i = 8; i < 14; i++) {
    pinMode(i, INPUT);
  }

}

void loop() {

  int b = buttonValue();
  manageLed(b);
  if ((b != -1) && b < 15) {
    String value = "100" + buttonValueMod();
    Serial.println(value);
    hc12.println(value);
  }


  int joystick = getJystickValue();

  if (joystick != 0) {
    String prefix = "0";
    if(digitalRead(led1) == LOW){
      prefix = "200";
    }else{
      prefix = "300";
    }
    String joyStickValue = prefix + (String)joystick;
    Serial.println(joyStickValue);
    hc12.println(joyStickValue);
  }

  // test receive form uno
  if (hc12.available() > 1) {
    int input = hc12.parseInt();//read serial input and convert to integer (-32,768 to 32,767)

    //if (input > 100) {
      Serial.println(input);
      hc12.flush();//clear the serial buffer for unwanted inputs
    //}
  }


  /*digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);*/

  delay(100);

}
