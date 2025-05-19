/*
 * Program to take axis data from IMU to track position over time.
 *
 * Name:    Thor Kopenkoskey
 * Date:    May 11, 2025
 * Docs on IMU: https://github.com/stm32duino/LSM6DSOX
 * Docs on Magno: https://github.com/adafruit/Adafruit_LIS3MDL
 * Does NOT use adruino library, use datasheet for register info
 */

#include "IMU.cpp"

using namespace std;

int main()
{
      digitalWrite(ledOUT, HIGH);
      delay(200);
      cout << "X axis acceleration: " << accelX << " M/S" << endl;
      cout << "Y axis acceleration: " << accelY << " M/S" << endl;
      cout << "Z axis acceleration: " << accelZ << " M/S" << endl;    
      cout << "X axis Gyro: " << gyroX << " DPS" <<  endl;
      cout << "Y axis Gyro: " << gyroY << " DPS" << endl;
      cout << "Z axis Gyro: " << gyroZ << " DPS" << endl;
      cout << endl;
    
      digitalWrite(ledOUT, LOW);
      delay(200);
  
}
