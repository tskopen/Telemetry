#include "servos.h"
#include "sensorfusion.h"

void setup() {
  sensor_setup();
}

void loop() {
  posfuser();
  servo_control_loop(fused_pitch, fused_yaw);
}
