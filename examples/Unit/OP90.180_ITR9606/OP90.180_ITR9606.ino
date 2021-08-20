/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/gray
*
* describe：OP 180/90.  非接触式光电限位开关
* date：2021/8/16
*******************************************************************************
  Please connect to Port B(36),Detect the current OP 90/180 Unit Photoelectric switch status.
  请连接端口B(36),检测当前OP 90/180单元光电开关状态。
*/

#include <M5Stack.h>

void setup() {
  M5.begin(); //Init M5Stack.  初始化M5Stack
  M5.Power.begin(); //Init power  初始化电源模块
  M5.lcd.setTextSize(2);  //Set the text size to 2.  设置文字大小为2
  M5.Lcd.setCursor(80, 10); //Set the cursor at (80,10).  将光标设置在(80,10)处
  M5.Lcd.println("90/180 OPTICAL");
  pinMode(36,INPUT_PULLUP); //Set pin 36 to input pull-up mode.  设置36号引脚为输入上拉模式
}

void loop() {
  M5.Lcd.setCursor(80, 120);
  M5.Lcd.printf("IR Receive: %d",digitalRead(36));  //Output the value of pin 36.  输出36号引脚的值
}
