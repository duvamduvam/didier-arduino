
#include <SoftwareSerial.h>

//radio transmission
SoftwareSerial hc12(2, 3); //RX, TX



void setup() {
  // put your setup code here, to run once:
  hc12.begin(9600);
  Serial.begin(9600);
}

void loop() {
   hc12.println("test");
  if (hc12.available() > 1) {
    int input = hc12.parseInt();//read serial input and convert to integer (-32,768 to 32,767)

    //if (input > 100) {
      Serial.println(input);
      hc12.flush();//clear the serial buffer for unwanted inputs
    //}
  }
}
