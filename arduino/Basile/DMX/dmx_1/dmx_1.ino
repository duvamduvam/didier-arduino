#include <DmxSimple.h>

void setup() {
  // put your setup code here, to run once:
  DmxSimple.usePin(2);
  DmxSimple.maxChannel(20);
}

byte i;
byte col;

void loop() {
  // put your main code here, to run repeatedly:

  i++;
  DmxSimple.write(1, 0);
  DmxSimple.write(col+2, i);
  if (i==0)
  {
    col++;
    if (col>2)
      col = 0;
  }
  
  delay(10);
}


