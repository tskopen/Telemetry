/*
 * LIS3MDL
 * Name:    Thor Kopenkoskey
 * Date:    May 22, 2025
 * Docs on IMU: https://github.com/stm32duino/LSM6DSOX
 * Docs on Magno: https://github.com/adafruit/Adafruit_LIS3MDL
 */
#include "magneto.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>
#include <unistd.h>   // read, write
#include <iostream>
#include <wiringPi.h>

uint8_t magnetometerClass::readRegister(uint8_t reg) 
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

int16_t magnetometerClass::magnetoData(uint8_t regL, uint8_t regH)
{
    uint8_t l = readRegister(regL); //Read
    uint8_t h = readRegister(regH); //Read
    return (int16_t)((h << 8) | l); //Merge two 8 bit values into one 16 bit
}  
void magnetometerClass::setmagneto() {
    cout << endl << "Magno Setup" << endl;

    // Open I2C bus
    file = open("/dev/i2c-1", O_RDWR);
    if (file < 0) {
        cerr << "Failed to open I2C bus" << endl;
        return;
    }

    // Set slave address
    if (ioctl(file, I2C_SLAVE, 0x1C) < 0) { // 0x1C is the default I2C address for LIS3MDL
        cerr << "Failed to set I2C address" << endl;
        return;
    }

    // Verify WHO_AM_I register (should return 0x3D)
    uint8_t whoami = readRegister(0x0F); // WHO_AM_I register address
    cout << "WHO_AM_I: 0x" << hex << (int)whoami << dec << endl;
    if (whoami != 0x3D) {
        cerr << "Incorrect WHO_AM_I value" << endl;
        return;
    }

    // Configure CTRL_REG1 (20h): enable temperature sensor, set output data rate, and operating mode
    uint8_t ctrl_reg1 = 0x70; // 0x70 = TEMP_EN | DO[2:0] = 100 (80 Hz), OM[1:0] = 00 (Low-power mode)
    if (write(file, &ctrl_reg1, 1) != 1) {
        cerr << "Failed to configure CTRL_REG1" << endl;
    }

    // Configure CTRL_REG2 (21h): set full-scale range
    uint8_t ctrl_reg2 = 0x00; // FS[1:0] = 00 (±4 gauss)
    if (write(file, &ctrl_reg2, 1) != 1) {
        cerr << "Failed to configure CTRL_REG2" << endl;
    }

    // Configure CTRL_REG3 (22h): set operating mode to continuous-conversion mode
    uint8_t ctrl_reg3 = 0x00; // MD[1:0] = 00 (Continuous-conversion mode)
    if (write(file, &ctrl_reg3, 1) != 1) {
        cerr << "Failed to configure CTRL_REG3" << endl;
    }

    // Configure CTRL_REG4 (23h): set Z-axis operating mode
    uint8_t ctrl_reg4 = 0x00; // OMZ[1:0] = 00 (Low-power mode)
    if (write(file, &ctrl_reg4, 1) != 1) {
        cerr << "Failed to configure CTRL_REG4" << endl;
    }

    // Configure CTRL_REG5 (24h): enable block data update
    uint8_t ctrl_reg5 = 0x40; // BDU = 1 (Block data update)
    if (write(file, &ctrl_reg5, 1) != 1) {
        cerr << "Failed to configure CTRL_REG5" << endl;
    }

    cout << "Magnetometer configured successfully" << endl;
}

void magnetometerClass::readMagentoData()
{
      //Calculations... todo, auto detect mode
      float magnoX = (magnetoData(LIS3MDL_REG_OUT_X_L, LIS3MDL_REG_OUT_X_H); 
      float magnoY = (magnetoData(LIS3MDL_REG_OUT_Y_L, LIS3MDL_REG_OUT_Y_H);
      float magnoZ = (magnetoData(LIS3MDL_REG_OUT_Z_L, LIS3MDL_REG_OUT_Z_H);


    // COUT variables
      cout << "Magno Data: " << magnoX << " M/S" << endl;
      cout << "Y axis acceleration: " << magnoY << " M/S" << endl;
      cout << "Z axis acceleration: " << magnoZ << " M/S" << endl;    
      cout << endl;
}
