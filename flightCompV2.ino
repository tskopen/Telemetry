#include <sensorfusion.h>
#include <servos.h>



void setup() {
  //155200 and above Baud avoid packetloss
  Serial.begin(115200);

  Wire.begin(I2C2_SDA, I2C2_SCL);
  Wire.setClock(100000);

  IMU.begin();
  magnetometer.begin_I2C(LIS3MDL_ADDR, &Wire);
  launch = false;

  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].init_servo();
  }
}

void loop(){

  readRawSensorData(); 
  controlLoop();

  if (!launch){
    
  preFlightPositionalTracking();
  preFlightOrientationTracking();

    if (gForceAccelerationZAxis > 1.5)
    {
      launch = true;
    }
  }

  if (launch){
  duringFlightPositionalTracking();
  duringFlightOrientationTracking();
  }
  
  /*

  Serial.print(gForceAccelerationXAxis); Serial.print('\t');
  Serial.print(gForceAccelerationYAxis); Serial.print('\t');
  Serial.print(gForceAccelerationZAxis); Serial.print('\t');

  Serial.print(deadReckoningXAxis); Serial.print('\t');
  Serial.print(deadReckoningYAxis); Serial.print('\t');
  Serial.print(deadReckoningZAxis); Serial.println('\t');
  */

/*
  Serial.print(gyroAccelerationXAxis); Serial.print('\t');
  Serial.print(gyroAccelerationYAxis); Serial.print('\t');
  Serial.print(gyroAccelerationZAxis); Serial.print('\t');
*/
  Serial.print(gyroPositionXAxis); Serial.print('\t');
  Serial.print(gyroPositionYAxis); Serial.print('\t');
  Serial.print(gyroPositionZAxis); Serial.println('\t');
/*
  Serial.print(magneticXAxis); Serial.print('\t');
  Serial.print(magneticYAxis); Serial.print('\t');
  Serial.println(magneticZAxis); 
  */
}