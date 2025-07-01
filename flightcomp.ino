#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_LIS3MDL.h>
#include <Arduino_LSM6DSOX.h>
#include "servos.h"
#define SerialPort Serial
#define I2C2_SDA    21
#define I2C2_SCL    22

#define LSM6DSOX_ADDR 0x6A 
#define LIS3MDL_ADDR 0x1C 

  Adafruit_LIS3MDL magnetometer;

void setup() {

  Serial.begin(115200);
  servocontrol();



  Wire.begin(I2C2_SDA, I2C2_SCL);
  Wire.setClock(100000);

Serial.println("Starting LIS3MDL...");
  if (!magnetometer.begin_I2C(LIS3MDL_ADDR, &Wire)) 
  {
    Serial.println("ERROR: Could not find LIS3MDL at 0x1C");
    while (1);  // halt
  } else 
  {
    Serial.println("LIS3MDL OK");
  }


  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.println("IMU initialized");

}


void loop() 
{
  servostest();
  delay(25);

  //posfuser();
}
void posfuser()
{
  float ax, ay, az;
  float gx, gy, gz;
  sensors_event_t magEvent;

  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) 
  {
    IMU.readAcceleration(ax, ay, az);
    IMU.readGyroscope(gx, gy, gz);

    magnetometer.getEvent(&magEvent);

//Raw Data

    Serial.print(ax); Serial.print('\t');
    Serial.print(ay); Serial.print('\t');
    Serial.print(az); Serial.print('\t');

    Serial.print(gx); Serial.print('\t');
    Serial.print(gy); Serial.print('\t');
    Serial.print(gz); Serial.print('\t');

    Serial.print(magEvent.magnetic.x); Serial.print('\t');
    Serial.print(magEvent.magnetic.y); Serial.print('\t');
    Serial.println(magEvent.magnetic.z); Serial.print('\t');


//Calculations 

//Fusion https://www.mathworks.com/help/nav/ref/ahrs.html


    delay(25);
  }
}
