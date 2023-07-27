//define pin name
#define dir_1 2
#define pwm_1 3
#define switch_1 8
#define switch_2 9
#define dir_2 5
#define pwm_2 4
#define pot A1 //potentiometer

#define dirA 0
#define dirB 0
void setup() {
  //declare pins as INPUT/OUTPUT
  pinMode(pwm_1, OUTPUT);
  pinMode(dir_1, OUTPUT);
  pinMode(pwm_2, OUTPUT);
  pinMode(dir_2, OUTPUT);
  //pinMode(pot, INPUT);
  //pinMode(switch_1, INPUT);
  //pinMode(switch_2, INPUT);
  Serial.begin(115200); //I am using Serial Monitor instead of LCD display
}
void loop() {
  int pwm_value;
  //for (pwm_value = 0; pwm_value <= 255; pwm_value++) {
    digitalWrite(dir_1, 1); //controls the direction the motor
    digitalWrite(dir_2, 1);
    analogWrite(pwm_2, 50); //increase the speed of the motor from 0 to 255
    analogWrite(pwm_1, 50); //decrease the speed of the motor from 255 to 0
    Serial.println("PWM: ");
    Serial.println(pwm_value); //Display the value of PWM
    delay(100);
  //}
  //while (1) continue; //terminate the program
}
