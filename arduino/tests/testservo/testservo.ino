#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include "ArduinoLog.h"
#define LOG_LEVEL LOG_LEVEL_VERBOSE

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVOMIN  125 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  575 // this is the 'maximum' pulse length count (out of 4096)

int SERVOCURRENT = 125; // this is the 'minimum' pulse length count (out of 4096)
char input;

void setup() {
  Serial.begin(9600);
  delay(2000);
  Log.begin   (LOG_LEVEL, &Serial);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
}

void loop() {
  if (Serial.available()) {
    input = Serial.read();
    Serial.println(input);
  }

  if (input == 'a') {

    if (SERVOCURRENT - 10 >= SERVOMIN) {
      SERVOCURRENT = SERVOCURRENT - 10;
      pwm.setPWM(0, 0, SERVOCURRENT );
      Log.notice("move to %d\n", SERVOCURRENT);
    }

  } else if (input == 'z') {
    if (SERVOCURRENT + 10 <= SERVOMAX) {
      SERVOCURRENT = SERVOCURRENT + 10;
      pwm.setPWM(0, 0, SERVOCURRENT );
      Log.notice("move to %d\n", SERVOCURRENT);
    }
  }

  /*
    //for ( int angle = 0; angle < 45; angle += 10) {
    delay(500);
    pwm.setPWM(0, 0, angleToPulse(angle) );
    delay(500);
    pwm.setPWM(1, 0, angleToPulse(angle) );
    delay(500);
    pwm.setPWM(2, 0, angleToPulse(angle) );
    }//*/

  delay(10);

}

/*
   angleToPulse(int ang)
   gets angle in degree and returns the pulse width
   also prints the value on seial monitor
   written by Ahmad Nejrabi for Robojax, Robojax.com
*/
int angleToPulse(int ang) {
  int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX); // map angle of 0 to 180 to Servo min and Servo max
  Serial.print("Angle: "); Serial.print(ang);
  Serial.print(" pulse: "); Serial.println(pulse);
  return pulse;
}
