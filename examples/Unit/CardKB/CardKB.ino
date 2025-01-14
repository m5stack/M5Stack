/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit CardKB
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <M5Stack.h>

#define CARDKB_ADDR (0x5F)  // Define the I2C address of CardKB.  定义CardKB的I2C地址

void setup()
{
    M5.begin();             // Init M5Stack.  初始化M5Stack
    M5.Power.begin();       // Init power  初始化电源模块
    M5.lcd.setTextSize(2);  // Set the text size to 2.  设置文字大小为2
    M5.Lcd.printf("CardKB Test\n");
    M5.Lcd.printf(">>");
}
void loop()
{
    Wire.requestFrom(CARDKB_ADDR, 1);  // Request 1 byte from the slave device.  向从设备请求1字节
    while (Wire.available())           // If received data is detected.  如果检测到收到数据
    {
        char c = Wire.read();  // Store the received data.  将接收到的数据存储
        if (c != 0) {
            M5.Lcd.printf("%c", c);
            Serial.println(c, HEX);
        }
    }
}
