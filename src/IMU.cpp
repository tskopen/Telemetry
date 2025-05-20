/*
 * Program to take axis data from IMU to track position over time.
 *
 * Name:    Thor Kopenkoskey
 * Date:    May 11, 2025
 * Docs on IMU: https://github.com/stm32duino/LSM6DSOX
 * Docs on Magno: https://github.com/adafruit/Adafruit_LIS3MDL
 * Does NOT use adruino library, use datasheet for register info
 */
#include "IMU.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>   // read, write
#include <iostream>
#include <wiringPi.h>

using namespace std;

//command I2C for data.

uint8_t IMUClass::readRegister(int file, uint8_t reg) 
  {
    // Write the register address
    if (write(file, &reg, 1) != 1) {
        // handle error
    }
    // Read one byte from the register
    uint8_t data = 0;
    if (read(file, &data, 1) != 1) 
    {
        // handle error
    }
    return data;
}
//Read Data
int16_t IMUClass::accelData(int file, uint8_t regL, uint8_t regH)
{
    uint8_t l = readRegister(file, regL); //Read
    uint8_t h = readRegister(file, regH); //Read
    return (int16_t)((h << 8) | l); //Merge two 8 bit values into one 16 bit
}  




void IMUClass::setIMU()
{  
    cout << endl << "IMU Setup" << endl;
  
    //i2c setup
    int file = open("/dev/i2c-1", O_RDWR);
    ioctl(file, I2C_SLAVE, 0x6A);
    
    // Set accelerometer settings
    uint8_t config1[2] = {0x10, 0x60}; // CTRL1_XL: 104Hz, 2g
    write(file, config1, 2);
      
    // CTRL2_G: ODR = 104 Hz, FS = 250 dps, gyro config
    uint8_t configGyro[2] = {0x11, 0x60};  // 0b01100000
    write(file, configGyro, 2);
      
    // Optional: safer reads
    uint8_t config2[2] = {0x12, 0x44}; // CTRL3_C: BDU + IF_INC
    write(file, config2, 2);
}


void IMUClass::readIMUData(int ledOUT)
{
      //Calculations... todo, auto detect mode
      float accelX = (accelData(file, LSM6DSOX_REG_OUTX_L_A, LSM6DSOX_REG_OUTX_H_A) / 16384.0) * 9.80665;  // data/16384.0 = 1.0 g on ±2g mode
      float accelY = (accelData(file, LSM6DSOX_REG_OUTY_L_A, LSM6DSOX_REG_OUTY_H_A) / 16384.0) * 9.80665;
      float accelZ = (accelData(file, LSM6DSOX_REG_OUTZ_L_A, LSM6DSOX_REG_OUTZ_H_A) / 16384.0) * 9.80665;
      float gyroX = (accelData(file, LSM6DSOX_REG_OUTX_L_G, LSM6DSOX_REG_OUTX_H_G) / 131.0); //converts to DPS for ±250 dps
      float gyroY = (accelData(file, LSM6DSOX_REG_OUTY_L_G, LSM6DSOX_REG_OUTY_H_G) / 131.0);
      float gyroZ = (accelData(file, LSM6DSOX_REG_OUTZ_L_G, LSM6DSOX_REG_OUTZ_H_G) / 131.0);

      digitalWrite(ledOUT, HIGH);
      delay(200);
    // COUT variables
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
