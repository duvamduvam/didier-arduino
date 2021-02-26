
/*

  ADXL3xx

  Reads an Analog Devices ADXL3xx accelerometer and communicates the

  acceleration to the computer. The pins used are designed to be easily

  compatible with the breakout boards from SparkFun, available from:

  http://www.sparkfun.com/commerce/categories.php?c=80

  The circuit:

  - analog 0: accelerometer self test

  - analog 1: z-axis

  - analog 2: y-axis

  - analog 3: x-axis

  - analog 4: ground

  - analog 5: vcc

  created 2 Jul 2008

  by David A. Mellis

  modified 30 Aug 2011

  by Tom Igoe

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/ADXL3xx

*/



const int xpin = A0;                  // x-axis of the accelerometer

const int ypin = A1;                  // y-axis

const int zpin = 38;                  // z-axis (only on 3-axis models)

void setup() {

  Serial.begin(115200);

}

void loop() {

  // print the sensor values:

  Serial.print(analogRead(xpin));

  // print a tab between values:

  Serial.print("\t");

  Serial.print(analogRead(ypin));

  // print a tab between values:

//  Serial.print("\t");

//  Serial.print(analogRead(zpin));

  Serial.println();

  // delay before next reading:

  delay(100);
}
