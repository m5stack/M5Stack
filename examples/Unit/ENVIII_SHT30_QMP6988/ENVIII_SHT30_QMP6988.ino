/*
    Description: Use ENV III Unit to read temperature, humidity, atmospheric pressure, and display the data on the screen.
    Please install library before compiling:  
    UNIT_ENV: https://github.com/m5stack/UNIT_ENV
*/
#include <M5Stack.h>
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "UNIT_ENV.h"

SHT3X sht30;
QMP6988 qmp6988;

float tmp = 0.0;
float hum = 0.0;
float pressure = 0.0;

void setup() {
  M5.begin();
  M5.Power.begin();
  Wire.begin();
  M5.Lcd.setBrightness(10);
  M5.Lcd.setTextSize(3);
  qmp6988.init();
  M5.Lcd.clear(BLACK);
}

void loop() {
  pressure = qmp6988.calcPressure();
  if(sht30.get()==0){
    tmp = sht30.cTemp;
    hum = sht30.humidity;
  }
  Serial.printf("Temperatura: %2.2f*C  Humedad: %0.2f%%  Pressure: %0.2fPa\r\n", tmp, hum, pressure);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(WHITE, BLACK);
  
  M5.Lcd.printf("Temp: %2.1f  \r\nHumi: %2.0f%%  \r\nPressure:%2.0fPa\r\n", tmp, hum, pressure);

  delay(100);

}
