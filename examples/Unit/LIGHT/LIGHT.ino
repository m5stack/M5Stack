/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit Light
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <M5Stack.h>

TFT_eSprite img = TFT_eSprite(&M5.Lcd);

void setup()
{
    M5.begin();        // Init M5Stack.  初始化M5Stack
    M5.Power.begin();  // Init power  初始化电源模块
    img.setColorDepth(8);
    img.createSprite(320, 240);  // 创建一块320x240的画布
    img.setTextSize(2);
    pinMode(26, INPUT);  // Set pin 26 as input mode.  设置引脚26为输入模式
}

void loop()
{
    static uint16_t digitalRead_value = 0, analogRead_value = 0;
    analogRead_value = analogRead(36);    // Store the analog quantity read from
                                          // pin 36. 将36号引脚读取到的模拟量存储
    digitalRead_value = digitalRead(26);  // Store the number read from pin 26. 将26号引脚读取到的数字量存储
    img.fillRect(0, 0, 320, 240, 0x00);
    img.drawString("UNIT_LIGHT EXAMPLE", 40, 0);
    img.setCursor(90, 30);
    img.printf("Analog:%d\n", analogRead_value);
    img.setCursor(90, 50);
    img.printf("Digital:%d\n", digitalRead_value);
    img.pushSprite(0, 0);  // 把画布推送到屏幕(0,0)处
    delay(10);
}
