//define pin name
    //left
#define dir_1 7
#define pwm_1 6

    //right
#define dir_2 4
#define pwm_2 5
//#define pot 1 //potentiometer

int speed = 1000;



void move(int dir, int turn)
{

  digitalWrite(dir_1, dir);
  digitalWrite(dir_2, dir);

  // go straight forward and backward
  if ( turn > 3 && turn < 7) {
    Serial.println("straight");
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
  pinMode(pwm_1, OUTPUT);
  pinMode(dir_1, OUTPUT);
  pinMode(pwm_2, OUTPUT);
  pinMode(dir_2, OUTPUT);
  //pinMode(pot, INPUT);
  //pinMode(switch_1, INPUT);
  //pinMode(switch_2, INPUT);
  Serial.begin(9600);
}
void loop() {

    /*digitalWrite(dir_1, 1); //controls the direction the motor
    analogWrite(pwm_1, 200); 

delay(500);

    digitalWrite(dir_2, 1);
    analogWrite(pwm_2, 200);

delay(500);*/

  forward();
  delay(500);
  backward();
  delay(500);
  right();
  delay(500);
  left();
  delay(500);
}
