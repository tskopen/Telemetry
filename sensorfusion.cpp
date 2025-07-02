#include "sensorfusion.h"
#include "servos.h"
#include <cmath>

#define SerialPort Serial
#define I2C2_SDA    21
#define I2C2_SCL    22

#define LSM6DSOX_ADDR 0x6A 
#define LIS3MDL_ADDR 0x1C 

Adafruit_LIS3MDL magnetometer;

double adx = 0, ady = 0, adz = 0;
double gpx = 0, gpy = 0, gpz = 0;
float fused_pitch = 0, fused_yaw = 0, fused_roll = 0;

void sensor_setup() {
  Serial.begin(115200);
  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].init_servo();
  }

  Wire.begin(I2C2_SDA, I2C2_SCL);
  Wire.setClock(100000);

  Serial.println("Starting LIS3MDL...");
  if (!magnetometer.begin_I2C(LIS3MDL_ADDR, &Wire)) {
    Serial.println("ERROR: Could not find LIS3MDL at 0x1C");
    while (1);
  } else {
    Serial.println("LIS3MDL OK");
  }

  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.println("IMU initialized");
}

void applyServoControl(float fused_pitch, float fused_yaw, float fused_roll) {
  fused_pitch = constrain(fused_pitch, -60, 60);
  fused_yaw   = constrain(fused_yaw,   -60, 60);
  fused_roll  = constrain(fused_roll,  -60, 60);

  float pitchGain = 1.5;
  float yawGain   = 1.5;
  float rollGain  = 1.5;

  float pitchCmd = fused_pitch * pitchGain;
  float yawCmd   = fused_yaw   * yawGain;
  float rollCmd  = fused_roll  * rollGain;

  float base = 90;

  ControlServo("+X", base + yawCmd - rollCmd);   // right
  ControlServo("-X", base - yawCmd + rollCmd);   // left
  ControlServo("+Y", base + pitchCmd + rollCmd); // top
  ControlServo("-Y", base - pitchCmd - rollCmd); // bottom
}

void servo_control_loop(float& fused_pitch, float& fused_yaw) {
  static unsigned long next = micros();
  const unsigned long interval = 9620;

  unsigned long now = micros();
  if ((long)(now - next) >= 0) {
    applyServoControl(fused_pitch, fused_yaw, fused_roll);
    next += interval;
  }
}

float wrapAngle(float angle) {
  while (angle > 180) angle -= 360;
  while (angle < -180) angle += 360;
  return angle;
}

void posfuser() {
  static bool initialized_gyro = false;

  sensors_event_t magEvent;
  magnetometer.getEvent(&magEvent);

  float mx = magEvent.magnetic.x;
  float my = magEvent.magnetic.y;
  float mz = magEvent.magnetic.z;

  float ax, ay, az;
  float gax, gay, gaz;

if (IMU.accelerationAvailable()) {
  IMU.readAcceleration(ax, ay, az);
}
if (IMU.gyroscopeAvailable()) {
  IMU.readGyroscope(gax, gay, gaz);
}

  gpx += gax / 104;
  gpy += gay / 104;
  gpz += gaz / 104;

  gpx = wrapAngle(gpx);
  gpy = wrapAngle(gpy);
  gpz = wrapAngle(gpz);

  float pitch = atan2(ax, sqrt(ay * ay + az * az));
  float roll  = atan2(ay, sqrt(ax * ax + az * az));
  float pitch_degrees = wrapAngle(pitch * (180.0 / PI));
  float roll_degrees  = wrapAngle(roll * (180.0 / PI));

  float mx_comp = mx * cos(pitch) + mz * sin(pitch);
  float my_comp = mx * sin(roll) * sin(pitch) + my * cos(roll) - mz * sin(roll) * cos(pitch);

  float yaw = atan2(-my_comp, mx_comp);
  float yaw_degrees = wrapAngle(yaw * (180.0 / PI));
// Set yaw offset to = 0... aka baseline is to rockets start position not magentic
static bool offsetSet = false;
static float yaw_offset = 0;

if (!offsetSet) {
  yaw_offset = yaw_degrees;
  offsetSet = true;
}

// Adjusted yaw
float adjusted_yaw = wrapAngle(yaw_degrees - yaw_offset);


  if (!initialized_gyro) {
    gpx = roll_degrees;
    gpy = pitch_degrees;
    gpz = yaw_degrees;
    initialized_gyro = true;
  }

  const float alpha = 0.98;

fused_pitch = wrapAngle(alpha * gpx + (1 - alpha) * pitch_degrees); // gpx = pitch
fused_roll  = wrapAngle(alpha * gpy + (1 - alpha) * roll_degrees);  // gpy = roll
fused_yaw   = wrapAngle(alpha * gpz + (1 - alpha) * adjusted_yaw);  // gpz = yaw

//gravity correction on IMU
  float gx = 9.81 * sin(pitch);
  float gy = 9.81 * sin(roll);
  float gz = sqrt(9.81 * 9.81 - gx * gx - gy * gy);

  float ax_real = ax * 9.81 - gx;
  float ay_real = ay * 9.81 - gy;
  float az_real = az * 9.81 - gz;

  adx += (ax_real / 104);
  ady += (ay_real / 104);
  adz += (az_real / 104);

  Serial.print(adx); Serial.print('\t');
  Serial.print(ady); Serial.print('\t');
  Serial.print(adz); Serial.print('\t');
  Serial.print(fused_pitch); Serial.print('\t');
  Serial.print(fused_roll);  Serial.print('\t');
  Serial.println(fused_yaw);
}
