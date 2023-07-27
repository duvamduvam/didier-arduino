//define pin name
/*#define dir_left 26
#define pwm_left 27
//#define switch_1 13
//#define switch_2 12
#define dir_right 28
#define pwm_right 29*/

//left
#define pwm_left 3
#define dir_left 22

//right
#define pwm_right 4
#define dir_right 24


//#define pot 1 //potentiometer

int speed = 255;
 


void move(int dir, int turn)
{

  digitalWrite(dir_left, dir);
  digitalWrite(dir_right, dir);

  // go straight forward and backward
  if ( turn > 3 && turn < 7) {
    analogWrite(pwm_left, speed);
    analogWrite(pwm_right, speed);
    //turn right
  } else if ( turn < 4) {
    analogWrite(pwm_left, speed);
    if (turn == 1) {
      analogWrite(pwm_right, 0);
    } else if (turn == 2) {
      analogWrite(pwm_right, (speed / 2));
    } else if (turn == 3) {
      analogWrite(pwm_right, speed);
    }

  } else {
    analogWrite(pwm_right, speed);
    if (turn == 9) {
      analogWrite(pwm_left, 0);
    } else if (turn == 8) {
      analogWrite(pwm_left, (speed / 2));
    } else if (turn == 7) {
      analogWrite(pwm_left, speed);
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
  pinMode(pwm_left, OUTPUT);
  pinMode(dir_left, OUTPUT);
  pinMode(pwm_right, OUTPUT);
  pinMode(dir_right, OUTPUT);
  //pinMode(pot, INPUT);
  //pinMode(switch_1, INPUT);
  //pinMode(switch_2, INPUT);
}
void loop() {
  forward();
  delay(3000);
  backward();
  delay(3000);
  right();
  delay(3000);
  left();
  delay(3000);
}
