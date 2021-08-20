/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/gray
*
* describe：Relay.  继电器
* date：2021/8/16
*******************************************************************************
  Please connect to Port B(26),Use RELAY to switch on and off the circuit.
  请连接端口B(26),使用继电器开关电路。
*/

#include <M5Stack.h>

void setup() {
  M5.begin(); //Init M5Stack.  初始化M5Stack
  M5.Power.begin(); //Init power  初始化电源模块
  M5.lcd.setTextSize(2);  //Set the text size to 2.  设置文字大小为2
  M5.Lcd.setCursor(50, 0);
  M5.Lcd.println(("Relay Example"));
  dacWrite(25, 0);  //disable the speak noise.  禁用喇叭
  pinMode(26, OUTPUT);  //Set pin 26 to output mode.  设置26号引脚为输出模式
}

void loop(void) {
  M5.Lcd.setCursor(100, 40);
  M5.Lcd.print("ON");
  digitalWrite(26, HIGH);
  delay(1000);
  M5.Lcd.fillRect(100,40,60,50,BLACK);
  M5.Lcd.print("OFF");
  digitalWrite(26, LOW);
  delay(1000);
  M5.Lcd.fillRect(100,40,60,50,BLACK);
}
