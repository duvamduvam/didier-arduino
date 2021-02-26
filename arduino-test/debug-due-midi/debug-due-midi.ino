void setup()
{  
  Serial.begin(115200);
  Serial1.begin(115200);  
  delay(100);
}

void loop()
{
  if (Serial1.available()) {
    String in = Serial1.readStringUntil('\n');
    Serial.println(in);
  }
}
