#include <LoRa.h>

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

int frequency = 915E6
int slavePIN = 1
int resetPIN = 2
int dio0PIN = 3
LoRa.setSPI(spi);
LoRa.setPins(slavePIN, resetPIN, dio0PIN);


LoRa.begin(frequency);
