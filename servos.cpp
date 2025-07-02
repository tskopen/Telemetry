#include "servos.h"


AxisServo servos[] = {
  {"+Y", Servo(), 12},
  {"-X", Servo(), 13},
  {"-Y", Servo(), 27},
  {"+X", Servo(), 14}
};

const int NUM_SERVOS = sizeof(servos) / sizeof(servos[0]);

void ControlServo(String name, double angle) {
  for (int i = 0; i < NUM_SERVOS; i++) {
    if (servos[i].name == name) {
      servos[i].writeAngle(angle);
    }
  }


}
