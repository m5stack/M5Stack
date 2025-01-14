/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit NCIR
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <M5Stack.h>

void setup()
{
    M5.begin();
    M5.Power.begin();
    M5.Lcd.setTextSize(3);
    M5.Lcd.setCursor(120, 0);
    M5.Lcd.print("NCIR");
}

void loop()
{
    static uint16_t result;
    static float temperature;
    Wire.beginTransmission(0x5A);  // Send Initial Signal and I2C Bus Address
                                   // 发送初始信号和I2C总线地址
    Wire.write(0x07);              // Send data only once and add one address automatically.
                                   // 只发送一次数据，并自动添加一个地址。
    Wire.endTransmission(false);   // Stop signal  停止信号
    Wire.requestFrom(0x5A, 2);     // Get 2 consecutive data from 0x5A, and the data is stored
                                   // only.  从0x5A中获取2个连续的数据，并且只存储这些数据。
    result = Wire.read();          // Receive DATA  接收数据
    result |= Wire.read() << 8;    // Receive DATA  接收数据

    temperature = result * 0.02 - 273.15;
    M5.Lcd.setCursor(70, 100);
    M5.Lcd.printf("Temp:%.3f", temperature);
    delay(500);
}
