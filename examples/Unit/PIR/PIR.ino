/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/gray
*
* Describe: PIR.  人体红外
* Date: 2021/8/11
*******************************************************************************
  Please connect to Port B(26、36),Human body detection using PIR Unit.
  请连接端口B(26、36),使用PIR Unit进行人体检测。
*/

#include <M5Stack.h>

void setup() {
    M5.begin();             // Init M5Stack.  初始化M5Stack
    M5.Power.begin();       // Init power  初始化电源模块
    M5.lcd.setTextSize(2);  // Set the text size to 2.  设置文字大小为2
    M5.Lcd.println("PIR example");
    M5.Lcd.setCursor(0,
                     25);  // Position the cursor at (0,25).  将光标固定在(0,25)
    M5.Lcd.println("Status: \nValue: ");
    pinMode(36, INPUT);  // Set pin 36 to input mode.  设置36号引脚为输入模式
}

void loop() {
    M5.Lcd.fillRect(90, 25, 180, 50,
                    BLACK);  // Draw a black rectangle 180 by 50 at (90,25).
                             // 在(90,25)处画一个宽180高50的黑的矩形
    if (digitalRead(36) ==
        1) {  // If pin 36 reads a value of 1.  如果36号引脚的读取到的值为1
        M5.Lcd.setCursor(95, 25);
        M5.Lcd.print("Sensing");
        M5.Lcd.setCursor(95, 45);
        M5.Lcd.print("1");
    } else {
        M5.Lcd.setCursor(95, 25);
        M5.Lcd.print("Not Sensed");
        M5.Lcd.setCursor(95, 45);
        M5.Lcd.print("0");
    }
    delay(500);
}