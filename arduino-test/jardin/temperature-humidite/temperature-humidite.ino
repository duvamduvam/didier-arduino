#include "dht.h"


#define dht_apin A0 // Analog Pin sensor is connected to

dht DHT;
int pinRelay = 2;

void setup() {

  Serial.begin(115200);
  delay(500);//Delay to let system boot
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  delay(1000);//Wait before accessing Sensor
  pinMode(pinRelay, OUTPUT);

}//end "setup()"

void loop() {
  //Start of Program

  DHT.read11(dht_apin);
  
  Serial.print("temperature = ");
  Serial.print(DHT.temperature);
  Serial.println("C  ");

  if(DHT.temperature<25){
    digitalWrite(pinRelay, HIGH);
  }else{
    digitalWrite(pinRelay, LOW);
  }

  delay(1000);

}// end loop()
