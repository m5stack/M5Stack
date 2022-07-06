/*
    DHT12.h - Library for DHT12 sensor.
    v0.0.1 Beta
    Created by Bobadas, July 30,2016.
    Released into the public domain.
*/
#ifndef DHT12_h
#define DHT12_h
#include "Arduino.h"
#include "Wire.h"

#define CELSIUS    1
#define KELVIN     2
#define FAHRENHEIT 3

class DHT12 {
   public:
    DHT12(uint8_t scale = 0, uint8_t id = 0);
    float readTemperature(uint8_t scale = 0);
    float readHumidity();

   private:
    uint8_t read();
    uint8_t datos[5];
    uint8_t _id;
    uint8_t _scale;
};

#endif
