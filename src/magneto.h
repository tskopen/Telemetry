/*
 * LIS3MDL
 * Name:    Thor Kopenkoskey
 * Date:    May 22, 2025
 * Docs on IMU: https://github.com/stm32duino/LSM6DSOX
 * Docs on Magno: https://github.com/adafruit/Adafruit_LIS3MDL
 */

// LIS3MDL Register Address Map
#define LIS3MDL_REG_OFFSET_X_REG_L_M    0x05
#define LIS3MDL_REG_OFFSET_X_REG_H_M    0x06
#define LIS3MDL_REG_OFFSET_Y_REG_L_M    0x07
#define LIS3MDL_REG_OFFSET_Y_REG_H_M    0x08
#define LIS3MDL_REG_OFFSET_Z_REG_L_M    0x09
#define LIS3MDL_REG_OFFSET_Z_REG_H_M    0x0A
#define LIS3MDL_REG_WHO_AM_I           0x0F
#define LIS3MDL_REG_CTRL_REG1          0x20
#define LIS3MDL_REG_CTRL_REG2          0x21
#define LIS3MDL_REG_CTRL_REG3          0x22
#define LIS3MDL_REG_CTRL_REG4          0x23
#define LIS3MDL_REG_CTRL_REG5          0x24
#define LIS3MDL_REG_STATUS_REG         0x27
#define LIS3MDL_REG_OUT_X_L            0x28
#define LIS3MDL_REG_OUT_X_H            0x29
#define LIS3MDL_REG_OUT_Y_L            0x2A
#define LIS3MDL_REG_OUT_Y_H            0x2B
#define LIS3MDL_REG_OUT_Z_L            0x2C
#define LIS3MDL_REG_OUT_Z_H            0x2D
#define LIS3MDL_REG_TEMP_OUT_L         0x2E
#define LIS3MDL_REG_TEMP_OUT_H         0x2F
#define LIS3MDL_REG_INT_CFG            0x30
#define LIS3MDL_REG_INT_SRC            0x31
#define LIS3MDL_REG_INT_THS_L          0x32
#define LIS3MDL_REG_INT_THS_H          0x33
class magnetometerClass
{
  public:
    uint8_t readRegister(uint8_t reg);
    int16_t magnetoData(uint8_t regL, uint8_t regH);
    void setMagneto();
    void readMagentoData();
  private:
    int file;
};
