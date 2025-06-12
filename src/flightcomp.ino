#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BusIO_Register.h>
#include <Adafruit_LIS3MDL.h>
#include <Arduino_LSM6DSOX.h>
#include <cmath>


#define SerialPort Serial
#define I2C2_SDA    21
#define I2C2_SCL    22

#define LSM6DSOX_ADDR 0x6A 
#define LIS3MDL_ADDR 0x1C 

  Adafruit_LIS3MDL magnetometer;

//Gyro and Accel are dead reckoning so are set to 0... Magno + GPS should give reference if needed... IMU should aim to be start level for now
double adx = 0;
double ady = 0;
double adz = 0;

double gpx = 0;
double gpy = 0;
double gpz = 0;

void setup() {
  //155200 and above Baud avoid packetloss
  Serial.begin(115200);
  while (!Serial);

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


void loop() {
  static unsigned long next = micros();
  const unsigned long interval = 9620;

  unsigned long now = micros();
  if ((long)(now - next) >= 0) {
    posfuser();
    next += interval;
  }
}


//set itital IMU pos 0 for dead reckoning

float wrapAngle(float angle) {
  while (angle > 180) angle -= 360;
  while (angle < -180) angle += 360;
  return angle;
}

void posfuser()
{
  static bool initialized_gyro = false;

  sensors_event_t magEvent;
  magnetometer.getEvent(&magEvent);


  float mx = magEvent.magnetic.x; 
  float my = magEvent.magnetic.y; 
  float mz = magEvent.magnetic.z;

    float ax, ay, az;
    float gax, gay, gaz;

    IMU.readAcceleration(ax, ay, az);
    IMU.readGyroscope(gax, gay, gaz);

//Default is 2000DPS, Library adjusted to 250... if you reimport you must change the function in the library to 250 (as its hard coded)
// gpx, gyro position axis
  gpx += (gax / 104);
  gpy += (gay / 104);
  gpz += (gaz / 104);

  gpx = wrapAngle(gpx);
  gpy = wrapAngle(gpy);
  gpz = wrapAngle(gpz);
    //Deda reckoning of Aceel data 
    // accel distance axis
// Accel + magno angle calculations
float pitch = atan2(ax, sqrt(ay * ay + az * az));
float roll  = atan2(ay, sqrt(ax * ax + az * az));
float pitch_degrees = wrapAngle(pitch * (180.0 / PI));
float roll_degrees  = wrapAngle(roll * (180.0 / PI));
//yaw tilt calculations
float mx_comp = mx * cos(pitch) + mz * sin(pitch);
float my_comp = mx * sin(roll) * sin(pitch) + my * cos(roll) - mz * sin(roll) * cos(pitch);
float yaw = atan2(-my_comp, mx_comp);  // yaw in radians
float yaw_degrees = wrapAngle(yaw * (180.0 / PI));
//Reset gyro to magno readings to start off with similar readings
  if (!initialized_gyro) {
    gpx = pitch_degrees;
    gpy = roll_degrees;
    gpz = yaw_degrees;
    initialized_gyro = true;  // only run this once
  }

  // Filter coefficient
  const float alpha = 0.5;
    // Complementary filter fusion.... ex. alpha = .98 trust gyro 98% and magno 2%
    float fused_pitch = wrapAngle(alpha * gpx + (1 - alpha) * pitch_degrees);
    float fused_roll  = wrapAngle(alpha * gpy + (1 - alpha) * roll_degrees);
    float fused_yaw   = wrapAngle(alpha * gpz + (1 - alpha) * yaw_degrees);

  // Rotate [0, 0, 9.81] by pitch/roll to find what part of gravity appears on each axis
  float gx = 9.81 * sin(pitch);  // simplified; ideally use a rotation matrix
  float gy = 9.81 * sin(roll);
  float gz = sqrt(9.81*9.81 - gx*gx - gy*gy);

  float ax_real = ax * 9.81 - gx;
  float ay_real = ay * 9.81 - gy;
  float az_real = az * 9.81 - gz;


    adx += (ax_real / 104 /*Hz*/);
    ady += (ay_real / 104);
    adz += (az_real / 104);
/*
    Serial.print(adx); Serial.print('\t');
    Serial.print(ady); Serial.print('\t');
    Serial.print(adz); Serial.print('\t');
*/

  Serial.print(pitch_degrees); Serial.print('\t');
  Serial.print(roll_degrees);  Serial.print('\t');
  Serial.print(yaw_degrees);   Serial.print("\t");

  Serial.print(fused_pitch); Serial.print('\t');
  Serial.print(fused_roll);  Serial.print('\t');
  Serial.println(fused_yaw);








//Fusion https://www.mathworks.com/help/nav/ref/ahrs.html
}

// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
float invSqrt(float x)
{
	float halfx = 0.5f * x;
	union {
	  float    f;
	  uint32_t i;
	} conv = { .f = x };
	conv.i = 0x5f3759df - (conv.i >> 1);
	conv.f *= 1.5f - (halfx * conv.f * conv.f);
	conv.f *= 1.5f - (halfx * conv.f * conv.f);
	return conv.f;
}
