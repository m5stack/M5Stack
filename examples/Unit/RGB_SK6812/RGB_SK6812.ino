/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit RGB
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 * Adafruit_NeoPixel: https://github.com/adafruit/Adafruit_NeoPixel
 */

#include <Adafruit_NeoPixel.h>
#include <M5Stack.h>

#define PIN       (26)  // 定义NeoPixel的控制引脚
#define NUMPIXELS (3)   // 定义NeoPixel控制灯灯数量

Adafruit_NeoPixel pixels =
    Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);  // set number of LEDs, pin number, LED type.
                                                              // 设置灯的数量,控制引脚编号,灯灯类型

void setup()
{
    M5.begin();        // Init M5Stack.  初始化M5Stack
    M5.Power.begin();  // Init power  初始化电源模块
    pixels.begin();    // Init the NeoPixel library.  初始化NeoPixel库
    M5.Lcd.println(("RGB Example"));
}

int i = 0, j = 1, k = 2;

void loop()
{
    pixels.setPixelColor(i++, pixels.Color(100, 0, 0));  // Bright red
    pixels.setPixelColor(j++, pixels.Color(0, 100, 0));  // Bright green
    pixels.setPixelColor(k++, pixels.Color(0, 0, 100));  // Bright blue
    pixels.show();                                       // sends the updated color to the hardware.
                                                         // 将更新后的颜色发送到硬件。
    delay(100);
    if (i == 3)
        i = 0;
    else if (j == 3)
        j = 0;
    else if (k == 3)
        k = 0;
}
