#include <Arduino.h>
#include "ArduinoLog.h"
#include "Vibrator.h"


Vibrator::Vibrator(int p){
	pin = p;
	timer = 0;
	strengh = 0;
}

void Vibrator::execute(int duration, int s){
	timer = millis() + duration;
	strengh = s;
}

void Vibrator::process(){

	if(timer>millis()){
		analogWrite(pin, strengh);
		//Log.notice("vbrator on pin %d with strengh %d\n",pin, strengh);
	}else{
		analogWrite(pin, LOW);
	}

}
