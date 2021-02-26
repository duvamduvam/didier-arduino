
void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);

}
void loop()
{
  if (Serial.available() > 0) {
    String s0 = Serial.readStringUntil('\n');
    Serial.print("serial zero received:"); Serial.println(s0);
  }
  Serial.println("serial tout court");

  if (Serial1.available() > 0) {
    String s1 = Serial1.readStringUntil('\n');
    Serial.print("serial 1 received:"); Serial.println(s1);
  }
  Serial1.println("serial1 out");

  delay(2000);

  if (Serial2.available() > 0) {
    String s2 = Serial2.readStringUntil('\n');
    Serial.print("serial 2 received:"); Serial.println(s2);
  }

  Serial2.println("serial2 out");
  delay(2000);

  if (Serial3.available() > 0) {
    String s3 = Serial3.readStringUntil('\n');
    Serial.print("serial 3 received:"); Serial.println(s3);
  }
  Serial3.println("serial3 out");
  delay(2000);



}
