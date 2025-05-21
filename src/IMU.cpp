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

uint8_t IMUClass::readRegister(uint8_t reg) 
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
int16_t IMUClass::accelData(uint8_t regL, uint8_t regH)
{
    uint8_t l = readRegister(regL); //Read
    uint8_t h = readRegister(regH); //Read
    return (int16_t)((h << 8) | l); //Merge two 8 bit values into one 16 bit
}  




void IMUClass::setIMU() {  
    cout << endl << "IMU Setup" << endl;
  
    // Open I2C bus
    file = open("/dev/i2c-1", O_RDWR);
    if (file < 0) {
        cerr << "Failed to open I2C bus" << endl;
        return;
    }

    // Set slave address
    if (ioctl(file, I2C_SLAVE, 0x6A) < 0) {
        cerr << "Failed to set I2C address" << endl;
        return;
    }

    // Verify WHO_AM_I register (should return 0x6C)
    uint8_t whoami = readRegister(LSM6DSOX_REG_WHO_AM_I);
    cout << "WHO_AM_I: 0x" << hex << (int)whoami << dec << endl;
    if (whoami != 0x6C) {
        cerr << "Incorrect WHO_AM_I value" << endl;
        return;
    }

    // Configure accelerometer (104Hz, ±2g)
    uint8_t config1[2] = {LSM6DSOX_REG_CTRL1_XL, 0x40}; // 0x40 = 104Hz, ±2g
    if (write(file, config1, 2) != 2) {
        cerr << "Failed to configure accelerometer" << endl;
    }

    // Configure gyroscope (104Hz, ±250dps)
    uint8_t configGyro[2] = {LSM6DSOX_REG_CTRL2_G, 0x40}; // 0x40 = 104Hz, ±250dps
    if (write(file, configGyro, 2) != 2) {
        cerr << "Failed to configure gyroscope" << endl;
    }

    // Enable block data update (prevent reading halfway through update)
    uint8_t config3[2] = {LSM6DSOX_REG_CTRL3_C, 0x44}; // BDU + IF_INC
    if (write(file, config3, 2) != 2) {
        cerr << "Failed to set CTRL3_C" << endl;
    }

    cout << "IMU configured successfully" << endl;
}

void IMUClass::readIMUData(int ledOUT)
{
      //Calculations... todo, auto detect mode
      float accelX = (accelData(LSM6DSOX_REG_OUTX_L_A, LSM6DSOX_REG_OUTX_H_A) / 16384.0 /*convert to G/s*/) * 9.80665/*convert to M/s*/; 
      float accelY = (accelData(LSM6DSOX_REG_OUTY_L_A, LSM6DSOX_REG_OUTY_H_A) / 16384.0) * 9.80665;
      float accelZ = (accelData(LSM6DSOX_REG_OUTZ_L_A, LSM6DSOX_REG_OUTZ_H_A) / 16384.0) * 9.80665;
      float gyroX = (accelData(LSM6DSOX_REG_OUTX_L_G, LSM6DSOX_REG_OUTX_H_G) / 131.0); //converts to DPS for ±250 dps
      float gyroY = (accelData(LSM6DSOX_REG_OUTY_L_G, LSM6DSOX_REG_OUTY_H_G) / 131.0);
      float gyroZ = (accelData(LSM6DSOX_REG_OUTZ_L_G, LSM6DSOX_REG_OUTZ_H_G) / 131.0);


    // COUT variables
      cout << "X axis acceleration: " << accelX << " M/S" << endl;
      cout << "Y axis acceleration: " << accelY << " M/S" << endl;
      cout << "Z axis acceleration: " << accelZ << " M/S" << endl;    
      cout << "X axis Gyro: " << gyroX << " DPS" <<  endl;
      cout << "Y axis Gyro: " << gyroY << " DPS" << endl;
      cout << "Z axis Gyro: " << gyroZ << " DPS" << endl;
      cout << endl;



  //Update IMU settings
  //go from 2g acceleration to 4g
  if (abs(accelX) >= (2.0 * 9.80665) || abs(accelY) >= (2.0 * 9.80665) || abs(accelZ) >= (2.0 * 9.80665)) //2g * (m/s)/g
      {
          digitalWrite(ledOUT, HIGH); delay(200); digitalWrite(ledOUT, LOW); delay(200); //LED FLASH
          // Configure accelerometer (104Hz, ±4g)
          uint8_t config1[2] = {LSM6DSOX_REG_CTRL1_XL, 0x5A}; // 5A = 208Hz, ±4g Table 51.
      }

    if (accelData(LSM6DSOX_REG_OUTX_L_G, LSM6DSOX_REG_OUTX_H_G) >= /*250dps RAW output value*/ 32750 || accelData(LSM6DSOX_REG_OUTY_L_G, LSM6DSOX_REG_OUTY_H_G) >= 32750 || accelData(LSM6DSOX_REG_OUTZ_L_G, LSM6DSOX_REG_OUTZ_H_G) >= 32750 )//2g * (m/s)/g
      {
          digitalWrite(21, HIGH); delay(200); digitalWrite(21, LOW); delay(200); //LED FLASH
          // Configure GYRO (208HZ, ±500)
          uint8_t config1[2] = {LSM6DSOX_REG_CTRL2_G, 0x54}; // 54 =±500 dps
          cout << config1[2];
      }
  }
