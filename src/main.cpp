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
#include "magneto.h"
#include <wiringPi.h>

using namespace std;

int main()
{  
      wiringPiSetup();
      IMUClass IMU;
      magnetometerClass magneto;
      int ledOUT = 26;
      
      IMU.setIMU();
      magneto.setMagneto();

      while (true)
      {
          IMU.readIMUData(ledOUT);
          magneto.readMagnetoData();
          delay(100);
      }
  
}
