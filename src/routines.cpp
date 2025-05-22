/*
 * Generalized routines/functions for flight computer... LED BLINK, timers, etc
 *
 * Name:    Thor Kopenkoskey
 * Date:    May 21, 2025
 */
#include "routines.h"
#include <chrono>
#include <thread>
#include <wiringPi.h>

using namespace std;

void routineClass::LEDblinker(unsigned long duration, int pin, int blinks) 
{
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);
    digitalWrite(pin, HIGH);
    digitalWrite(pin, LOW);

    digitalWrite(pin, HIGH);
    digitalWrite(pin, LOW);
}

