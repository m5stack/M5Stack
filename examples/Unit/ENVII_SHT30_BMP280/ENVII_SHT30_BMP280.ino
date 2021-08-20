/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/gray
*
* describe：ENVII_SHT30_BMP280.  环境传感器
* date：2021/8/11
*******************************************************************************
  Please connect to Port A(22、21),Read temperature, humidity and atmospheric pressure and display them on the display screen
  请连接端口A(22、21),读取温度、湿度和大气压强并在显示屏上显示
*/

#include <M5Stack.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "UNIT_ENV.h"

SHT3X sht30;
Adafruit_BMP280 bme;

float tmp = 0.0;
float hum = 0.0;
float pressure = 0.0;

void setup() {
  M5.begin(); //Init M5Stack.  初始化M5Stack
  M5.Power.begin(); //Init power  初始化电源模块
  M5.lcd.setTextSize(2);  //Set the text size to 2.  设置文字大小为2
  Wire.begin(); //Wire init, adding the I2C bus.  Wire初始化, 加入i2c总线
  M5.lcd.println(F("ENV Unit(SHT30 and BMP280) test...\n"));
}

void loop() {
  while (!bme.begin(0x76)){ //初始化bme传感器.  Init the sensor of bme
    M5.Lcd.println("Could not find a valid BMP280 sensor, check wiring!");
  }
  pressure = bme.readPressure();  //Stores the pressure gained by BMP.  存储bmp获取到的压强
  sht30.get();  //Obtain the data of shT30.  获取sht30的数据
  tmp = sht30.cTemp;  //Store the temperature obtained from shT30.  将sht30获取到的温度存储
  hum = sht30.humidity; //Store the humidity obtained from the SHT30.  将sht30获取到的湿度存储
  M5.lcd.fillRect(0,20,100,60,BLACK); //Fill the screen with black (to clear the screen).  将屏幕填充黑色(用来清屏)
  M5.lcd.setCursor(0,20);
  M5.Lcd.printf("Temp: %2.1f  \r\nHumi: %2.0f%%  \r\nPressure:%2.0fPa\r\n", tmp, hum, pressure);
  delay(2000);
}
