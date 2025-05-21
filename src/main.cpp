/*
 * Program to take axis data from IMU to track position over time.
 *
 * Name:    Thor Kopenkoskey
 * Date:    May 19, 2025
 * Docs on IMU: https://github.com/stm32duino/LSM6DSOX
 * Docs on Magno: https://github.com/adafruit/Adafruit_LIS3MDL
 * Does NOT use adruino library, use datasheet for register info
 */

#include "IMU.h"
#include <wiringPi.h>

using namespace std;

int main()
{  
      IMUClass IMU;
      int ledOUT = 26;
      
      wiringPiSetup();
      pinMode(ledOUT, OUTPUT);
      
      IMU.setIMU();

      //Main data loop
      while (true)
      {
          digitalWrite(21, HIGH); delay(200); digitalWrite(21, LOW); delay(200);
          IMU.readIMUData(ledOUT);
            delay(100);
      }
  
}
