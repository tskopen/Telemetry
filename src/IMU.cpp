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

uint8_t readRegister(int file, uint8_t reg) 
  {
    // Write the register address
    if (write(file, &reg, 1) != 1) {
        // handle error
    }
    // Read one byte from the register
    uint8_t data;
    if (read(file, &data, 1) != 1) {
        // handle error
    }
    return data;
}
//Read Data
uint16_t accelData (int file, uint8_t regL, uint8_t regH)
{
  uint8_t l = readRegister(file, regL); //Read
  uint8_t h = readRegister(file, regH); //Read
  return (int16_t)((h << 8) | l); //Merge two 8 bit values into one 16 bit
}  

int main()
{




int ledOUT = 26;

cout << endl << "System Startup" << endl;

//i2c setup
int file = open("/dev/i2c-1", O_RDWR);
ioctl(file, I2C_SLAVE, 0x6A);

// Set accelerometer settings
uint8_t config1[2] = {0x10, 0x60}; // CTRL1_XL: 104Hz, 2g
write(file, config1, 2);

// Optional: safer reads
uint8_t config2[2] = {0x12, 0x44}; // CTRL3_C: BDU + IF_INC
write(file, config2, 2);
  
wiringPiSetup();
pinMode(ledOUT, OUTPUT);

digitalWrite(ledOUT, HIGH);
delay(1000);


digitalWrite(ledOUT, LOW);
delay(1000);
for (int i = 0; i < 25; i++)
  {
  cout << "X axis " << accelData(file, LSM6DSOX_REG_OUTX_L_A, LSM6DSOX_REG_OUTX_H_A) << endl;
  cout << "Y axis: " << accelData(file, LSM6DSOX_REG_OUTY_L_A, LSM6DSOX_REG_OUTY_H_A) << endl;
  cout << "Z axis: " << accelData(file, LSM6DSOX_REG_OUTZ_L_A, LSM6DSOX_REG_OUTZ_H_A) << endl;
  }
cout << endl << "System Started" << endl;
}
