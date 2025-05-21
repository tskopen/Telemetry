/*
 * Generalized routines/functions for flight computer... LED BLINK, timers, etc
 *
 * Name:    Thor Kopenkoskey
 * Date:    May 21, 2025
 */
#include "routines.h"
#include <chrono>
#include <wiringPi.h>

using namespace std;

void routineClass::LEDblinker(unsigned long duration, int pin, int blinks) {
    wiringPiSetup();
    pinMode(pin, OUTPUT);
    digitalWrite(pin, LOW);

    static unsigned long startMillis= 0;
    static int blinkCount = 0;

    // Calculate the interval between each blink
    unsigned long interval = duration / (blinks);
  
    for(int i = 0; i < blinks; i++)
      // Initialize the timer on the first call
      if (i < blinks) 
      {
          startMillis = millis();
      }
  
      // Check if it's time to toggle the LED state
      if (millis() - startMillis >= interval) 
      {
        digitalWrite(pin, HIGH);
          
        startMillis = millis(); // Reset the timer
        blinkCount++;

        }
    }
}

