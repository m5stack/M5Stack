/*
    Description: Use ENV Unit to read temperature, humidity, atmospheric pressure, and display the data on the screen.
    Please install library before compiling:  
    Adafruit BMP280: https://github.com/adafruit/Adafruit_BMP280_Library
*/

#include <M5Stack.h>
#include "DHT12.h"
#include <Wire.h> //The DHT12 uses I2C comunication.
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>

DHT12 dht12; //Preset scale CELSIUS and ID 0x5c.
Adafruit_BMP280 bme;

void setup() {
    M5.begin();
    M5.Power.begin();
    Wire.begin();

    M5.Lcd.setBrightness(10);

    Serial.println(F("ENV Unit(DHT12 and BMP280) test..."));

    while (!bme.begin(0x76)){  
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
      M5.Lcd.println("Could not find a valid BMP280 sensor, check wiring!");
    }
    M5.Lcd.clear(BLACK);
    M5.Lcd.println("ENV Unit test...");
}

void loop() {
 
    float tmp = dht12.readTemperature();
    float hum = dht12.readHumidity();
    float pressure = bme.readPressure();
    Serial.printf("Temperatura: %2.2f*C  Humedad: %0.2f%%  Pressure: %0.2fPa\r\n", tmp, hum, pressure);

    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setTextSize(3);
    M5.Lcd.printf("Temp: %2.1f  \r\nHumi: %2.0f%%  \r\nPressure:%2.0fPa\r\n", tmp, hum, pressure);

    delay(100);
}
