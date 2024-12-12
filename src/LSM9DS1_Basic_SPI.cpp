/*****************************************************************
LSM9DS1_Basic_SPI.ino
SFE_LSM9DS1 Library Simple Example Code - SPI Interface
Jim Lindblom @ SparkFun Electronics
Original Creation Date: April 29, 2015
https://github.com/sparkfun/LSM9DS1_Breakout

The LSM9DS1 is a versatile 9DOF sensor. It has a built-in
accelerometer, gyroscope, and magnetometer. Very cool! Plus it
functions over either SPI or I2C.

This Arduino sketch is a demo of the simple side of the
SFE_LSM9DS1 library. It'll demo the following:
* How to create a LSM9DS1 object, using a constructor (global
  variables section).
* How to use the begin() function of the LSM9DS1 class.
* How to read the gyroscope, accelerometer, and magnetometer
  using the readGryo(), readAccel(), readMag() functions and 
  the gx, gy, gz, ax, ay, az, mx, my, and mz variables.
* How to calculate actual acceleration, rotation speed, 
  magnetic field strength using the calcAccel(), calcGyro() 
  and calcMag() functions.
* How to use the data from the LSM9DS1 to calculate 
  orientation and heading.

Hardware setup: This example demonstrates how to use the
LSM9DS1 with an SPI interface. The pin-out is as follows:
	LSM9DS1 --------- Arduino
          CS_AG ------------- 9
          CS_M ------------- 10
          SDO_AG ----------- 12
          SDO_M ------------ 12 (tied to SDO_AG)
          SCL -------------- 13
          SDA -------------- 11
          VDD -------------- 3.3V
          GND -------------- GND

The LSM9DS1 has a maximum voltage of 3.6V. Make sure you power it
off the 3.3V rail! Signals going into the LSM9DS1, at least,
should be level shifted down to 3.3V - that's CSG, CSXM,
SCL, and SDA.

Better yet, use a 3.3V Arduino (e.g. the Pro or Pro Mini)!

Development environment specifics:
	IDE: Arduino 1.6.3
	Hardware Platform: Arduino Pro 3.3V
	LSM9DS1 Breakout Version: 1.0

This code is beerware. If you see me (or any other SparkFun 
employee) at the local, and you've found our code helpful, 
please buy us a round!

Distributed as-is; no warranty is given.
*****************************************************************/
// The SFE_LSM9DS1 library requires both Wire and SPI be
// included BEFORE including the 9DS1 library.

////////////////////			//gmmorte
#include <stdint.h>
#include <inttypes.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <math.h>
#define PI 3.141592654
/////using namespace std;
//#include <errno.h>  //necessary?
#include <stdio.h>
#include <unistd.h>
#include <iostream>  //to use cout, need std and iostream
//#include </home/pi/SparkFun_LSM9DS1_Arduino_Library/src/ino_to_raspi.cpp>
#include <cstdlib>    
////////////////////			//gmmorte

//#include <SPI.h> //gmmorte
//#include </home/pi/SparkFun_LSM9DS1_Arduino_Library/src/SparkFunLSM9DS1.h>
#include <SparkFunLSM9DS1.h>
//#include </home/pi/SparkFun_LSM9DS1_Arduino_Library/src/SparkFunLSM9DS1.cpp>

//////////////////////////
// LSM9DS1 Library Init //
//////////////////////////
// Use the LSM9DS1 class to create an object. [imu] can be
// named anything, we'll refer to that throught the sketch.
LSM9DS1 imu; //gmmorte
//LSM9DS1 imu(IMU_MODE_SPI, 4, 5);

///////////////////////
// Example SPI Setup //
///////////////////////
// Define the pins used for our SPI chip selects. We're
// using hardware SPI, so other signal pins are set in stone.
//#define LSM9DS1_M_CS	10 // Can be any digital pin
//#define LSM9DS1_AG_CS	9  // Can be any other digital pin
#define LSM9DS1_M_CS  4 // pin 16 / GPIO 23 / WiringPi 4
#define LSM9DS1_AG_CS 5 // pin 18 / GPIO 24 / WiringPi 5

////////////////////////////
// Sketch Output Settings //
////////////////////////////
#define PRINT_CALCULATED
//#define PRINT_RAW
#define PRINT_SPEED 250 // 250 ms between prints

// Earth's magnetic field varies by location. Add or subtract 
// a declination to get a more accurate heading. Calculate 
// your's here:
// http://www.ngdc.noaa.gov/geomag-web/#declination
#define DECLINATION -10.66 // Declination (degrees) in Northern Virginia


void setup() 
{
  wiringPiSetup(); //gmmorte
  if (wiringPiSPISetup (0, 500000) < 0) { //gmmorte
	std::cout<<"SPI Setup Failed"<<std::endl; }
  //Serial.begin(115200);  //gmmorte
  
  // Before initializing the IMU, there are a few settings
  // we may need to adjust. Use the settings struct to set
  // the device's communication mode and addresses:
  imu.settings.device.commInterface = IMU_MODE_SPI;
  imu.settings.device.mAddress = LSM9DS1_M_CS;
  imu.settings.device.agAddress = LSM9DS1_AG_CS;
  // The above lines will only take effect AFTER calling
  // imu.begin(), which verifies communication with the IMU
  // and turns it on.
std::cout <<"mAddress, agAddress: "<<int(imu.settings.device.mAddress) << ", " << int(imu.settings.device.agAddress) <<std::endl;
  if (!imu.begin())
  {
    //Serial.println("Failed to communicate with LSM9DS1.");
    std::cout << "Failed to communicate with LSM9DS1." << std::endl;
    //Serial.println("Double-check wiring.");
    std::cout << "Double-check wiring." << std::endl;
    //Serial.println("Default settings in this sketch will " 
    //              "work for an out of the box LSM9DS1 " 
    //              "Breakout, but may need to be modified " 
    //              "if the board jumpers are.");
    while (1)
      ;
  }
}



void printGyro()
{
  // To read from the gyroscope, you must first call the
  // readGyro() function. When this exits, it'll update the
  // gx, gy, and gz variables with the most current data.
  imu.readGyro();
  
  // Now we can use the gx, gy, and gz variables as we please.
  // Either print them as raw ADC values, or calculated in DPS.
  //Serial.print("G: ");
  std::cout << "G: ";
  #ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcGyro helper function to convert a raw ADC value to
  // DPS. Give the function the value that you want to convert.
  //Serial.print(imu.calcGyro(imu.gx), 2);
  //Serial.print(", ");
  //Serial.print(imu.calcGyro(imu.gy), 2);
  //Serial.print(", ");
  //Serial.println(imu.calcGyro(imu.gz), 2); 
  std::cout << imu.calcGyro(imu.gx) << ", ";
  std::cout << imu.calcGyro(imu.gy) << ", ";
  std::cout << imu.calcGyro(imu.gz) << std::endl;

  #elif defined PRINT_RAW
  //Serial.print(imu.gx);
  //Serial.print(", ");
  //Serial.print(imu.gy);
  //Serial.print(", ");
  //Serial.println(imu.gz);
  std::cout << imu.gx << ", ";
  std::cout << imu.gy << ", ";
  std::cout << imu.gz << std::endl;
  #endif
}

void printAccel()
{
  // To read from the accelerometer, you must first call the
  // readAccel() function. When this exits, it'll update the
  // ax, ay, and az variables with the most current data.
  imu.readAccel();
  
  // Now we can use the ax, ay, and az variables as we please.
  // Either print them as raw ADC values, or calculated in g's.
  //Serial.print("A: ");
  std::cout << "A: ";
  #ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcAccel helper function to convert a raw ADC value to
  // g's. Give the function the value that you want to convert.
  //Serial.print(imu.calcAccel(imu.ax), 2);
  //Serial.print(", ");
  //Serial.print(imu.calcAccel(imu.ay), 2);
  //Serial.print(", ");
  //Serial.println(imu.calcAccel(imu.az), 2);
  std::cout << imu.calcAccel(imu.ax) << ", ";
  std::cout << imu.calcAccel(imu.ay) << ", ";
  std::cout << imu.calcAccel(imu.az) << std::endl;
  #elif defined PRINT_RAW 
  //Serial.print(imu.ax);
  //Serial.print(", ");
  //Serial.print(imu.ay);
  //Serial.print(", ");
  //Serial.println(imu.az);
  std::cout << (imu.ax) << ", ";
  std::cout << (imu.ay) << ", ";
  std::cout << (imu.az) << std::endl;
  #endif

}

void printMag()
{
  // To read from the magnetometer, you must first call the
  // readMag() function. When this exits, it'll update the
  // mx, my, and mz variables with the most current data.
  imu.readMag();
  
  // Now we can use the mx, my, and mz variables as we please.
  // Either print them as raw ADC values, or calculated in Gauss.
  //Serial.print("M: ");
  std::cout << "M: ";
  #ifdef PRINT_CALCULATED
  // If you want to print calculated values, you can use the
  // calcMag helper function to convert a raw ADC value to
  // Gauss. Give the function the value that you want to convert.
  //Serial.print(imu.calcMag(imu.mx), 2);
  //Serial.print(", ");
  //Serial.print(imu.calcMag(imu.my), 2);
  //Serial.print(", ");
  //Serial.println(imu.calcMag(imu.mz), 2);
  std::cout << imu.calcMag(imu.mx) << ", ";
  std::cout << imu.calcMag(imu.my) << ", ";
  std::cout << imu.calcMag(imu.mz) << std::endl;
  #elif defined PRINT_RAW
  //Serial.print(imu.mx);
  //Serial.print(", ");
  //Serial.print(imu.my);
  //Serial.print(", ");
  //Serial.println(imu.mz);
  std::cout << imu.mx << ", ";
  std::cout << imu.my << ", ";
  std::cout << imu.mz << std::endl;
  #endif
}

// Calculate pitch, roll, and heading.
// Pitch/roll calculations take from this app note:
// http://cache.freescale.com/files/sensors/doc/app_note/AN3461.pdf?fpsp=1
// Heading calculations taken from this app note:
// http://www51.honeywell.com/aero/common/documents/myaerospacecatalog-documents/Defense_Brochures-documents/Magnetic__Literature_Application_notes-documents/AN203_Compass_Heading_Using_Magnetometers.pdf
void printAttitude(
float ax, float ay, float az, float mx, float my, float mz)
{
  float roll = atan2(ay, az);
  float pitch = atan2(-ax, sqrt(ay * ay + az * az));
  
  float heading;
  if (my == 0)
    heading = (mx < 0) ? 180.0 : 0;
  else
    heading = atan2(mx, my);
    
  heading -= DECLINATION * PI / 180;
  
  if (heading > PI) heading -= (2 * PI);
  else if (heading < -PI) heading += (2 * PI);
  else if (heading < 0) heading += 2 * PI;
  
  // Convert everything from radians to degrees:
  heading *= 180.0 / PI;
  pitch *= 180.0 / PI;
  roll  *= 180.0 / PI;
  
  //Serial.print("Pitch, Roll: ");
  //Serial.print(pitch, 2);
  //Serial.print(", ");
  //Serial.println(roll, 2);
  //Serial.print("Heading: "); Serial.println(heading, 2);
  std::cout << "Pitch, Roll: " << pitch << ", " << roll << std::endl;
  std::cout << "Heading: " << heading;
}

void loop()
{
  printGyro();  // Print "G: gx, gy, gz"
  printAccel(); // Print "A: ax, ay, az"
  printMag();   // Print "M: mx, my, mz"
  
  // Print the heading and orientation for fun!
  // Call print attitude. The LSM9DS1's magnetometer x and y
  // axes are opposite to the accelerometer, so my and mx are
  // substituted for each other.
  printAttitude(imu.ax, imu.ay, imu.az, -imu.my, -imu.mx, imu.mz);
  //Serial.println();
  std::cout <<  std::endl;

  delay(PRINT_SPEED);
}


int main (void)
{
  setup();
  while(1)
  {    
    loop();
    loop();
    loop();
	//break; //for test
  };
  return 0;
};
