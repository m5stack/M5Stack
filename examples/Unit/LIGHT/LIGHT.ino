/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/gray
*
* describe: Light.  环境光传感器
* date：2021/8/18
*******************************************************************************
  Please connect to Port B,Use the Light Unit screen to display the current ambient lighting value
  请连接端口 B ,使用Light Unit 屏幕显示当前环境光照值。
*/

#include <M5Stack.h>

void setup() {
  M5.begin(); //Init M5Stack.  初始化M5Stack
  M5.Power.begin(); //Init power  初始化电源模块
  M5.lcd.setTextSize(2);  //Set the text size to 2.  设置文字大小为2
  M5.Lcd.printf("UNIT_LIGHT EXAMPLE\n\n");
  M5.Lcd.println("Analog:");
  M5.Lcd.println("Digital:");
  pinMode(26, INPUT); //Set pin 26 as input mode.  设置引脚26为输入模式
}

void loop() {
  static uint16_t digitalRead_value = 0, analogRead_value = 0;
  analogRead_value = analogRead(36);  //Store the analog quantity read from pin 36.  将36号引脚读取到的模拟量存储
  digitalRead_value = digitalRead(26);  //Store the number read from pin 26.  将26号引脚读取到的数字量存储
  M5.Lcd.setCursor(90, 30);
  M5.Lcd.printf("%d\n", analogRead_value);
  M5.Lcd.setCursor(90, 50);
  M5.Lcd.printf("%d\n", digitalRead_value);
  delay(10);
}
