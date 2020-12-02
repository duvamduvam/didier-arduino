#include <IRremote.h>
#include <SoftwareSerial.h>

#define G 16736925
#define B 16754775
#define L 16720605
#define R 16761405
#define S 16712445
#define UNKNOWN_G 5316027
#define UNKNOWN_B 2747854299
#define UNKNOWN_L 1386468383
#define UNKNOWN_R 553536955
#define UNKNOWN_S 3622325019

int RECV_PIN = 12;
int in1 = 6;
int in2 = 7;
int in3 = 8;
int in4 = 9;
int ENA = 5;
int ENB = 11;
int ABS = 115;

int delayDirection = 100;

SoftwareSerial hc12(2, 3); // RX, TX
unsigned long last = millis();//set timer

//IRrecv irrecv(RECV_PIN);
//decode_results results;
//unsigned long val;


void _mForward()
{
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(in1, HIGH); //digital output
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  Serial.println("go forward!");
  delay(delayDirection);
  _mStop();
}
void _mBack()
{
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println("go back!");
  delay(delayDirection);
  _mStop();
}
void _mleft()
{
  analogWrite(ENA, ABS);
  digitalWrite(ENB, HIGH);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  Serial.println("go left!");
  delay(50);
  _mStop();
}
void _mright()
{
  analogWrite(ENA, ABS);
  digitalWrite(ENB, HIGH);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  Serial.println("go right!");
  delay(50);
  _mStop();
}
void _mStop()
{
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  Serial.println("STOP!");
}

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  _mStop();
  // irrecv.enableIRIn();

  hc12.begin(9600);

  Serial.begin(9600);
}

void direction(String dir) {
  int x = dir.substring(2, 1).toInt();
  int y = dir.substring(1, 0).toInt();

  if (y >= 7) {
    _mForward();
  } else if (y <= 3) {
    _mBack();
  }

  if (x >= 7) {
    _mright();
  } else if (x <= 3) {
    _mleft();
  }

  //printDebug((String)x, "x");
  //printDebug((String)y, "y");
}

void printDebug(String value, String msg) {
  Serial.println(msg);
  Serial.println(value);
}

void loop() {


  if (hc12.available() > 1) {
    int input = hc12.parseInt();//read serial input and convert to integer (-32,768 to 32,767)


    if (input > 100) {

      String inputString = (String) input;
      if (inputString.startsWith("100")) {
        Serial.println("button pressed");
      } else if (inputString.startsWith("200")) {
        printDebug(inputString.substring(3), "xy");
        direction(inputString.substring(3));
      }

    }




    hc12.flush();//clear the serial buffer for unwanted inputs
    last = millis();//reset timer
  }
  delay(20);//delay little for better serial communication


 // _mForward();

  /* if (irrecv.decode(&results)){
       Serial.println("in loop!");
     val = results.value;
     Serial.println(val);
     irrecv.resume();
     switch(val){
       case G:
       case UNKNOWN_G: _mForward();break;
       case B:
       case UNKNOWN_B: _mBack(); break;
       case L:
       case UNKNOWN_L: _mleft(); break;
       case R:
       case UNKNOWN_R: _mright();break;
       case S:
       case UNKNOWN_S: _mStop(); break;
       default:break;
     }*/

}
