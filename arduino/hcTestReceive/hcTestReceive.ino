#include <Wire.h>
#include <SoftwareSerial.h>
#include <Adafruit_PWMServoDriver.h>


SoftwareSerial hc12(2, 3); // RX, TX
// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define SERVOMIN  150 // this is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  320 // this is the 'maximum' pulse length count (out of 4096)

boolean debugMsg = true;

// our servo # counter
uint8_t servonum = 0;
int head1 = SERVOMIN;
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
  printDebug((String)p);
  return p;
}


void moveHead(int p) {
  printDebug((String)p);

  if (p == 9 && head1 < SERVOMAX) {
    head1 = head1 + headStep;
    pwm.setPWM(servonum, 0, head1);
  }

  if (p == 1 &&  head1 > SERVOMIN) {
    head1 = head1 - headStep;
    pwm.setPWM(servonum, 0, head1);
  }
}

void setup() {
  hc12.begin(9600);
  Serial.begin(9600);

  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  pwm.setPWM(servonum, 0, head1);

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
        printDebug("direction");
        int p = (int)getDirNumber((String)input, 3);
        printDebug((String)p);
        moveHead(p);
      }
      //printDebug(input);
    }

    hc12.flush();//clear the serial buffer for unwanted inputs
    last = millis();//reset timer
  }

 /* if (Serial.available()) {
    int s = Serial.read();
    hc12.println(s);
  }
  //hc12.println("1212");
  hc12.println(Serial.available());*/


  delay(20);
}
