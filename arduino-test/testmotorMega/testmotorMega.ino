//define pin name
#define dir_1 26
#define pwm_1 27
//#define switch_1 13
//#define switch_2 12
#define dir_2 28
#define pwm_2 29
//#define pot 1 //potentiometer

int speed = 1000;



void move(int dir, int turn)
{

  digitalWrite(dir_1, dir);
  digitalWrite(dir_2, dir);

  // go straight forward and backward
  if ( turn > 3 && turn < 7) {
    analogWrite(pwm_1, speed);
    analogWrite(pwm_2, speed);
    //turn right
  } else if ( turn < 4) {
    analogWrite(pwm_1, speed);
    if (turn == 1) {
      analogWrite(pwm_2, 0);
    } else if (turn == 2) {
      analogWrite(pwm_2, (speed / 2));
    } else if (turn == 3) {
      analogWrite(pwm_2, speed);
    }

  } else {
    analogWrite(pwm_2, speed);
    if (turn == 9) {
      analogWrite(pwm_1, 0);
    } else if (turn == 8) {
      analogWrite(pwm_1, (speed / 2));
    } else if (turn == 7) {
      analogWrite(pwm_1, speed);
    }
  }
}

void forward() {
  move(0, 5);
}

void backward() {
  move(1, 5);
}

void right() {
  move(1, 1);
}

void left() {
  move(1, 9);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(pwm_1, OUTPUT);
  pinMode(dir_1, OUTPUT);
  pinMode(pwm_2, OUTPUT);
  pinMode(dir_2, OUTPUT);
  //pinMode(pot, INPUT);
  //pinMode(switch_1, INPUT);
  //pinMode(switch_2, INPUT);
  Serial.begin(9600); //I am using Serial Monitor instead of LCD display
}
void loop() {
  forward();
  delay(500);
  backward();
  delay(500);
  right();
  delay(500);
  left();
  delay(500);
}
