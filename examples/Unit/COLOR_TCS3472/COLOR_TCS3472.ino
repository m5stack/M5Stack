/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/gray
*
* Describe: COLOR_TCS3472.  颜色识别
* Date: 2021/8/11
*******************************************************************************
  Please connect to Port A(22、21),Use COLOR Unit to read C, R, G, B values
  请连接端口A(22、21),使用COLOR Unit读取C, R, G, B值
*/

#include <M5Stack.h>

#include "Adafruit_TCS34725.h"

#define commonAnode \
    true  // set to false if using a common cathode LED.
          // //如果使用普通阴极LED，则设置为false

byte gammatable[256];  // our RGB -> eye-recognized gamma color

static uint16_t color16(uint16_t r, uint16_t g, uint16_t b) {
    uint16_t _color;
    _color = (uint16_t)(r & 0xF8) << 8;
    _color |= (uint16_t)(g & 0xFC) << 3;
    _color |= (uint16_t)(b & 0xF8) >> 3;
    return _color;
}

Adafruit_TCS34725 tcs =
    Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

void setup() {
    M5.begin();             // Init M5Stack.  初始化M5Stack
    M5.Power.begin();       // Init power  初始化电源模块
    M5.lcd.setTextSize(2);  // Set the text size to 2.  设置文字大小为2
    Serial.println("Color View Test!");
    while (!tcs.begin()) {  //如果color unit未能初始化
        Serial.println("No TCS34725 found ... check your connections");
        M5.Lcd.drawString("No Found sensor.", 50, 100, 4);
        delay(1000);
    }
    tcs.setIntegrationTime(
        TCS34725_INTEGRATIONTIME_154MS);  // Sets the integration time for the
                                          // TC34725.  设置TC34725的集成时间
    tcs.setGain(TCS34725_GAIN_4X);  // Adjusts the gain on the TCS34725.
                                    // 调整TCS34725上的增益
}

void loop() {
    uint16_t clear, red, green, blue;
    tcs.getRawData(&red, &green, &blue,
                   &clear);  // Reads the raw red, green, blue and clear channel
                             // values.  读取原始的红、绿、蓝和清晰的通道值

    // Figure out some basic hex code for visualization.  生成对应的十六进制代码
    uint32_t sum = clear;
    float r, g, b;
    r = red;
    r /= sum;
    g = green;
    g /= sum;
    b = blue;
    b /= sum;
    r *= 256;
    g *= 256;
    b *= 256;
    uint16_t _color = color16((int)r, (int)g, (int)b);

    M5.lcd.setCursor(0, 20);  // Place the cursor at (0,20).  将光标固定在(0,20)
    M5.lcd.fillRect(
        0, 20, 120, 80,
        BLACK);  // Fill the screen with a black rectangle.  将屏幕填充黑色矩形

    M5.Lcd.print("C:");
    M5.Lcd.println(clear);
    M5.Lcd.print("R:");
    M5.Lcd.println(red);
    M5.Lcd.print("G:");
    M5.Lcd.println(green);
    M5.Lcd.print("B:");
    M5.Lcd.println(blue);
    M5.Lcd.print("0x");
    M5.Lcd.print((int)r, HEX);
    M5.Lcd.print((int)g, HEX);
    M5.Lcd.print((int)b, HEX);

    delay(1000);
}
