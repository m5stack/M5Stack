/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Mini Unit OLED
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 * U8g2_Arduino: https://github.com/olikraus/U8g2_Arduino
 */

#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

#define SDA_PIN (21)
#define SCL_PIN (22)

U8G2_SSD1306_72X40_ER_F_HW_I2C u8g2(U8G2_R0, /* reset=*/U8X8_PIN_NONE);  // EastRising 0.42" OLED

void setup(void)
{
    Wire.begin(SDA_PIN, SCL_PIN);
    u8g2.begin();
}

void loop(void)
{
    u8g2.clearBuffer();                  // clear the internal memory
    u8g2.setFont(u8g2_font_ncenB08_tr);  // choose a suitable font
    u8g2.drawStr(10, 20, "Hello");       // write something to the internal memory
    u8g2.drawStr(10, 30, "M5Stack!");
    u8g2.sendBuffer();  // transfer internal memory to the display
    delay(1000);
}
