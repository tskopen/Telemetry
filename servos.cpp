#include "servos.h"
/*
p12 +y
p13 +x
p25 -y
p14 -x
*/
Servo servop12;
Servo servop13;
Servo servop25;
Servo servop14;

int p12_plus_y = 12
int p13_plus_x = 13
int p25_minus_y = 25
int p14_minus_x = 14

void servo_plus_y() {
  servo1.setPeriodHertz(50);
  servo1.attach(p12_plus_y);

  servo1.writeMicroseconds(6000);
}
void servo_plus_x() {
  servo1.setPeriodHertz(50);
  servo1.attach(p13_plus_x);

  servo1.writeMicroseconds(6000);
}
void servo_minus_y() {
  servo1.setPeriodHertz(50);
  servo1.attach(p25_minus_y);

  servo1.writeMicroseconds(6000);
}
void servo_minus_x() {
  servo1.setPeriodHertz(50);
  servo1.attach(p14_minus_x);

  servo1.writeMicroseconds(6000);
}

void servostest()
{
  servo_plus_y();
  servo_plus_x();
  servo_minus_y();
  servo_minus_x();
  delay(1000);

}