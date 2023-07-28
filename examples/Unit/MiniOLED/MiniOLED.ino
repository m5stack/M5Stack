/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/MiniOLED%20Unit
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/MiniOLED%20Unit
*
* Describe: Unit MiniOLED
* Date: 2023/07/28
*******************************************************************************
  Depend Libraries:
  -[U8g2](https://github.com/olikraus/U8g2_Arduino)
  Please install the required libraries and connect the Unit MiniOLED to the
  PORT-A port (G21/22)
  请安装依赖库并将Unit MiniOLED 连接到PORT-A端口(G21/22)
*/
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

#define SDA_PIN 21
#define SCL_PIN 22

U8G2_SSD1306_72X40_ER_F_HW_I2C u8g2(
    U8G2_R0, /* reset=*/U8X8_PIN_NONE);  // EastRising 0.42" OLED

void setup(void) {
    Wire.begin(SDA_PIN, SCL_PIN);
    u8g2.begin();
}

void loop(void) {
    u8g2.clearBuffer();                  // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr);  // choose a suitable font
    u8g2.drawStr(10, 20, "Hello");  // write something to the internal memory
    u8g2.drawStr(10, 30, "M5Stack!");
    u8g2.sendBuffer();  // transfer internal memory to the display
    delay(1000);
}
