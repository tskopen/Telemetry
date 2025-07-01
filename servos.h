

#ifndef SERVOS_H
#define SERVOS_H

#include <ESP32Servo.h>

// Declare the servo instance (but don't define it here)
extern Servo servo;

// Declare your function
void servocontrol();
void servomove();
#endif