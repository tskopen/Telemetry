/*
 * Program to take axis data from IMU to track position over time.
 *
 * Name:    Thor Kopenkoskey
 * Date:    May 11, 2025
 * Docs on IMU: https://github.com/stm32duino/LSM6DSOX
 * Docs on Magno: https://github.com/adafruit/Adafruit_LIS3MDL
 */
#include "IMU.h"
#include <iostream>
#include <wiringPi.h>

using namespace std;
int main()
{
int ledOUT = 26;

cout << endl << "System Startup" << endl;

wiringPiSetup();
pinMode(ledOUT, OUTPUT);

digitalWrite(ledOUT, HIGH);
delay(1000);

digitalWrite(ledOUT, LOW);
delay(1000);

cout << endl << "System Started" << endl;
}
