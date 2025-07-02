#ifndef SENSORS_H
#define SENSORS_H

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_LIS3MDL.h>
#include <Arduino_LSM6DSOX.h>

extern Adafruit_LIS3MDL magnetometer;

extern double adx, ady, adz;
extern double gpx, gpy, gpz;
extern float fused_pitch, fused_yaw, fused_roll;

void sensor_setup();
void applyServoControl(float fused_pitch, float fused_yaw, float fused_roll);
void servo_control_loop(float& fused_pitch, float& fused_yaw);
void posfuser();
float wrapAngle(float angle);

#endif
