/*
 * Program to take axis data from IMU to track position over time.
 *
 * Name:    Thor Kopenkoskey
 * Date:    May 19, 2025
 * Docs on IMU: https://github.com/stm32duino/LSM6DSOX
 * Docs on Magno: https://github.com/adafruit/Adafruit_LIS3MDL
 * Does NOT use adruino library, use datasheet for register info
 */

#include "IMU.cpp"
#include "IMU.h"

#include <wiringPi.h>

using namespace std;

int main()
{  
      int ledOUT = 21;
      
      wiringPiSetup();
      pinMode(ledOUT, OUTPUT);
      IMU.setIMU();

      //Main data loop
      while (true)
      {
          IMU.readIMUData(21);
      }
  
}
