#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_PWMServoDriver.h>


SoftwareSerial hc12(2, 3); // RX, TX
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


uint8_t servo0 = 0;
uint8_t servo1 = 1;

#define SERVOMIN0  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX0  320 // this is the 'maximum' pulse length count (out of 4096)

#define SERVOMIN1  0 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX1  500 // this is the 'maximum' pulse length count (out of 4096)

int curServPos0 = SERVOMIN0;
int curServPos1 = SERVOMIN1;

boolean debugMsg = true;


int headStep = 10;

unsigned long last = millis();//set timer

void printDebug(String msg) {
  if (debugMsg == true) {
    Serial.println(msg);
  }
}

int getDirNumber(String msg, int pos) {
  String s = (String)msg.charAt(pos);
  int p = s.toInt();
  //printDebug((String)p);
  return p;
}


void moveHead(int p0, int p1 ) {

  //move head up and dow
  curServPos0 = moveServo(p0, servo0, curServPos0, SERVOMIN0, SERVOMAX0);
  curServPos1 = moveServo(p1, servo1, curServPos1, SERVOMIN1, SERVOMAX1);

  if(curServPos0 == 0 && curServPos1 == 0){
      printDebug("alerte");
  }

}

int moveServo(int p, int servo, int currentPos, int MIN, int MAX) {

  printDebug("servo " + (String)servo);
  printDebug("pos " + (String)p);
  printDebug((String)currentPos);

  if (p > 7 && currentPos < MAX) {
    currentPos = currentPos + headStep;
    pwm.setPWM(servo, 0, currentPos);
  }else if (p < 3 && currentPos > MIN) {
    currentPos = currentPos - headStep;
    pwm.setPWM(servo, 0, currentPos);
  }

  return currentPos;
}

void setup() {
  hc12.begin(9600);
  Serial.begin(9600);

  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  pwm.setPWM(servo0, 0, curServPos0);
  pwm.setPWM(servo1, 0, curServPos1);

  delay(10);
}

void loop() {

  if (hc12.available() > 1) {
    int input = hc12.parseInt();//read serial input and convert to integer (-32,768 to 32,767)

    if (input > 100) {

      String inputString = (String) input;
      if (inputString.startsWith("100")) {
        printDebug("button pressed");
        Serial.println(input);

      } else if (inputString.startsWith("200")) {
        //printDebug("direction");
        int p0 = (int)getDirNumber((String)input, 3);
        int p1 = (int)getDirNumber((String)input, 4);
        //printDebug((String)p);
        moveHead(p0, p1);
      }
      //printDebug(input);
    }

    hc12.flush();//clear the serial buffer for unwanted inputs
    last = millis();//reset timer
  }

  delay(20);
}
