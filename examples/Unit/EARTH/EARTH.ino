/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/gray
*
* Describe: EARTH.  土壤湿度
* Date: 2021/8/11
*******************************************************************************
  Please connect to Port B(26、36),Read the analog quantity and digital quantity
returned by the EARTH unit, and convert the analog quantity into 12-bit data and
display it on the screen. 请连接端口B(26、36),读取EARTH Unit
返回的模拟量和数字量，并将模拟量转换为12位数据显示在屏幕上。
*/

#include <M5Stack.h>

void setup() {
    M5.begin();             // Init M5Stack.  初始化M5Stack
    M5.Power.begin();       // Init power  初始化电源模块
    M5.lcd.setTextSize(2);  // Set the text size to 2.  设置文字大小为2
    M5.Lcd.printf("UNIT_EARTH EXAMPLE\n");
    pinMode(26, INPUT);  // Set pin 26 to input mode.  将引脚26设置为输入模式
    dacWrite(25, 0);  // disable the speak noise.  禁用喇叭
}

void loop() {
    M5.Lcd.setCursor(0, 80);  // Set the cursor at (0,80).  将光标设置在(0,80)
    M5.Lcd.printf("AnalogRead:%d\n", analogRead(36));
    M5.Lcd.printf("DigitalRead:%d\n", digitalRead(26));
    delay(1000);
}
