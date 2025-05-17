/*
 *
 * LSM6DSOX
 * Name:    Thor Kopenkoskey
 * Date:    May 11, 2025
 * Docs on IMU: https://github.com/stm32duino/LSM6DSOX
 * Docs on Magno: https://github.com/adafruit/Adafruit_LIS3MDL
 */
//Includes
#include <stdint.h>
//Defines

// LSM6DSOX Register Address Map
#define LSM6DSOX_REG_FUNC_CFG_ACCESS     0x01
#define LSM6DSOX_REG_PIN_CTRL            0x02
#define LSM6DSOX_REG_S4S_TPH_L           0x04
#define LSM6DSOX_REG_S4S_TPH_H           0x05
#define LSM6DSOX_REG_S4S_RR              0x06

// FIFO control registers
#define LSM6DSOX_REG_FIFO_CTRL1          0x07
#define LSM6DSOX_REG_FIFO_CTRL2          0x08
#define LSM6DSOX_REG_FIFO_CTRL3          0x09
#define LSM6DSOX_REG_FIFO_CTRL4          0x0A

// Batching and interrupt control
#define LSM6DSOX_REG_COUNTER_BDR_REG1    0x0B
#define LSM6DSOX_REG_COUNTER_BDR_REG2    0x0C
#define LSM6DSOX_REG_INT1_CTRL           0x0D
#define LSM6DSOX_REG_INT2_CTRL           0x0E

// Identification
#define LSM6DSOX_REG_WHO_AM_I            0x0F  // Default: 0x6C

// Control registers
#define LSM6DSOX_REG_CTRL1_XL            0x10
#define LSM6DSOX_REG_CTRL2_G             0x11
#define LSM6DSOX_REG_CTRL3_C             0x12
#define LSM6DSOX_REG_CTRL4_C             0x13
#define LSM6DSOX_REG_CTRL5_C             0x14
#define LSM6DSOX_REG_CTRL6_C             0x15
#define LSM6DSOX_REG_CTRL7_G             0x16
#define LSM6DSOX_REG_CTRL8_XL            0x17
#define LSM6DSOX_REG_CTRL9_XL            0x18
#define LSM6DSOX_REG_CTRL10_C            0x19

// Status and source registers
#define LSM6DSOX_REG_ALL_INT_SRC         0x1A
#define LSM6DSOX_REG_WAKE_UP_SRC         0x1B
#define LSM6DSOX_REG_TAP_SRC             0x1C
#define LSM6DSOX_REG_D6D_SRC             0x1D
#define LSM6DSOX_REG_STATUS_REG          0x1E

// Output data registers
#define LSM6DSOX_REG_OUT_TEMP_L          0x20
#define LSM6DSOX_REG_OUT_TEMP_H          0x21
#define LSM6DSOX_REG_OUTX_L_G            0x22
#define LSM6DSOX_REG_OUTX_H_G            0x23
#define LSM6DSOX_REG_OUTY_L_G            0x24
#define LSM6DSOX_REG_OUTY_H_G            0x25
#define LSM6DSOX_REG_OUTZ_L_G            0x26
#define LSM6DSOX_REG_OUTZ_H_G            0x27

#define LSM6DSOX_REG_OUTX_L_A            0x28
#define LSM6DSOX_REG_OUTX_H_A            0x29
#define LSM6DSOX_REG_OUTY_L_A            0x2A
#define LSM6DSOX_REG_OUTY_H_A            0x2B
#define LSM6DSOX_REG_OUTZ_L_A            0x2C
#define LSM6DSOX_REG_OUTZ_H_A            0x2D

// Function-specific output status
#define LSM6DSOX_REG_EMB_FUNC_STATUS     0x35
#define LSM6DSOX_REG_FSM_STATUS_A        0x36
#define LSM6DSOX_REG_FSM_STATUS_B        0x37
#define LSM6DSOX_REG_MLC_STATUS          0x38
#define LSM6DSOX_REG_STATUS_MASTER       0x39

// FIFO status
#define LSM6DSOX_REG_FIFO_STATUS1        0x3A
#define LSM6DSOX_REG_FIFO_STATUS2        0x3B

// Timestamp registers
#define LSM6DSOX_REG_TIMESTAMP0          0x40
#define LSM6DSOX_REG_TIMESTAMP1          0x41
#define LSM6DSOX_REG_TIMESTAMP2          0x42
#define LSM6DSOX_REG_TIMESTAMP3          0x43

// OIS output and status registers
#define LSM6DSOX_REG_UI_STATUS_OIS       0x49
#define LSM6DSOX_REG_UI_OUTX_L_G_OIS     0x4A
#define LSM6DSOX_REG_UI_OUTX_H_G_OIS     0x4B
#define LSM6DSOX_REG_UI_OUTY_L_G_OIS     0x4C
#define LSM6DSOX_REG_UI_OUTY_H_G_OIS     0x4D
#define LSM6DSOX_REG_UI_OUTZ_L_G_OIS     0x4E
#define LSM6DSOX_REG_UI_OUTZ_H_G_OIS     0x4F

#define LSM6DSOX_REG_UI_OUTX_L_A_OIS     0x50
#define LSM6DSOX_REG_UI_OUTX_H_A_OIS     0x51
#define LSM6DSOX_REG_UI_OUTY_L_A_OIS     0x52
#define LSM6DSOX_REG_UI_OUTY_H_A_OIS     0x53
#define LSM6DSOX_REG_UI_OUTZ_L_A_OIS     0x54
#define LSM6DSOX_REG_UI_OUTZ_H_A_OIS     0x55

// Tap and motion configuration
#define LSM6DSOX_REG_TAP_CFG0            0x56
#define LSM6DSOX_REG_TAP_CFG1            0x57
#define LSM6DSOX_REG_TAP_CFG2            0x58
#define LSM6DSOX_REG_TAP_THS_6D          0x59
#define LSM6DSOX_REG_INT_DUR2            0x5A
#define LSM6DSOX_REG_WAKE_UP_THS         0x5B
#define LSM6DSOX_REG_WAKE_UP_DUR         0x5C
#define LSM6DSOX_REG_FREE_FALL           0x5D
#define LSM6DSOX_REG_MD1_CFG             0x5E
#define LSM6DSOX_REG_MD2_CFG             0x5F

// Synchronization and I3C
#define LSM6DSOX_REG_S4S_ST_CMD_CODE     0x60
#define LSM6DSOX_REG_S4S_DT_REG          0x61
#define LSM6DSOX_REG_I3C_BUS_AVB         0x62
#define LSM6DSOX_REG_INTERNAL_FREQ_FINE  0x63

// OIS control and offset
#define LSM6DSOX_REG_UI_INT_OIS          0x6F
#define LSM6DSOX_REG_UI_CTRL1_OIS        0x70
#define LSM6DSOX_REG_UI_CTRL2_OIS        0x71
#define LSM6DSOX_REG_UI_CTRL3_OIS        0x72
#define LSM6DSOX_REG_X_OFS_USR           0x73
#define LSM6DSOX_REG_Y_OFS_USR           0x74
#define LSM6DSOX_REG_Z_OFS_USR           0x75

// FIFO data output
#define LSM6DSOX_REG_FIFO_DATA_OUT_TAG   0x78
#define LSM6DSOX_REG_FIFO_DATA_OUT_X_L   0x79
#define LSM6DSOX_REG_FIFO_DATA_OUT_X_H   0x7A
#define LSM6DSOX_REG_FIFO_DATA_OUT_Y_L   0x7B
#define LSM6DSOX_REG_FIFO_DATA_OUT_Y_H   0x7C
#define LSM6DSOX_REG_FIFO_DATA_OUT_Z_L   0x7D
#define LSM6DSOX_REG_FIFO_DATA_OUT_Z_H   0x7E

#define LSM6DSOX_REG_

