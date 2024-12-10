#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <errno.h>

int main (void)
{
	wiringPiSetup();
	pinMode(0,OUTPUT);
	digitalWrite(0,HIGH);
	//int output = wiringPiSPISetup(0,500000);
	//std::cout<<output<<std::endl;
	//if (output<0)
	if (wiringPiSPISetup(0,500000)<0)
		{std::cout <<"SPI Setup Failed" <<std::endl;}
	digitalWrite(0,LOW);
}