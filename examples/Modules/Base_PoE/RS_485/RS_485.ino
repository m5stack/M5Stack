/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/gray
*
* Describe: RS485
* Date: 2022/5/21
*******************************************************************************
*/
#include <M5Stack.h>

void setup() {
    M5.begin(true, false, true, false);  // Init M5Core.  初始化 M5Core
    M5.Power.begin();  // Init Power module.  初始化电源模块
    M5.Lcd.setTextColor(
        YELLOW);  // Set the font color to yellow.  设置字体颜色为黄色
    M5.Lcd.setTextSize(2);  // Set the font size.  设置字体大小为2
    M5.Lcd.drawString("RS485", 130, 10);
    M5.Lcd.drawString("Signal", 0, 60);
    Serial2.begin(115200, SERIAL_8N1, 5, 15);
}

void loop() {
    if (M5.BtnA.wasPressed()) {  // Press the buttonA to send a
                                 // message.按下按键A发送信息
        M5.Lcd.fillCircle(100, 65, 15,
                          GREEN);  // Set the light to Green. 设置灯为绿色
        M5.update();
        Serial2.write('a');
        delay(50);

    } else {
        M5.Lcd.fillCircle(100, 65, 15, WHITE);
    }
    if (Serial2.available()) {    // If the serial port receives a
                                  // message. 如果串口收到信息
        int ch = Serial2.read();  // Read the message. 读取信息
        Serial.write(ch);
        M5.Lcd.fillCircle(100, 65, 15, GREEN);
        M5.update();
        delay(50);
    } else {
        M5.Lcd.fillCircle(100, 65, 15, WHITE);
    }
    M5.update();
}