/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Stick-C sample source code
*                          配套  M5Stick-C 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/m5stickc
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/m5stickc
*
* describe：WiFi connect.  wifi连接
* date：2021/7/27
*******************************************************************************
*/

/*
    Description: Read ANGLE Unit input analog quantity, and convert to 12-bit digital quantity for display 
*/

#include <M5Stack.h>
// select the input pin for the potentiometer
int sensorPin = 36;
// last variable to store the value coming from the sensor
int last_sensorValue = 100;
// current variable to store the value coming from the sensor
int cur_sensorValue = 0;

void setup() {
  M5.begin();
  M5.Power.begin();
  pinMode(sensorPin, INPUT);
  dacWrite(25, 0);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("the value of ANGLE: ");
}

void loop() {
  // read the value from the sensor:
  cur_sensorValue = analogRead(sensorPin);
  M5.Lcd.setCursor(0, 25);
  if(abs(cur_sensorValue - last_sensorValue) > 10){//debaunce
    M5.Lcd.fillRect(0, 25, 100, 25, BLACK);
    M5.Lcd.print(cur_sensorValue);
    last_sensorValue = cur_sensorValue;
  }
  delay(50);
}
