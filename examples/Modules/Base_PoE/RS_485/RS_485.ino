/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Base LAN PoE
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <M5Stack.h>

void setup()
{
    M5.begin(true, false, true, false);  // Init M5Core.  初始化 M5Core
    M5.Power.begin();                    // Init Power module.  初始化电源模块
    M5.Lcd.setTextColor(YELLOW);         // Set the font color to yellow.  设置字体颜色为黄色
    M5.Lcd.setTextSize(2);               // Set the font size.  设置字体大小为2
    M5.Lcd.drawString("RS485", 130, 10);
    M5.Lcd.drawString("Signal", 0, 60);
    Serial2.begin(115200, SERIAL_8N1, 5, 15);
}

void loop()
{
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