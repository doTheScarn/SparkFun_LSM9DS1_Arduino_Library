//Translate Arduino functions from Sparkfun LSM9DS1 to Raspberry Pi and its WiringPi

#include <ino_to_raspi.h>

#include <wiringPi.h>
#include <wiringPiSPI.h>

//Serial - begin, println

void ino_to_raspiSerial::begin (int clockspeed) { 
	//do nothing. It's supposed to set up Arduino's serial to computer
}

void ino_to_raspiSerial::println (
