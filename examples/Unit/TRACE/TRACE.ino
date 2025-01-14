/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit ?
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <M5Stack.h>

// #define VALUE_SPLIT

uint8_t value;
int SensorArray[4] = {0};

void SensorStatus(void)
{
    Wire.beginTransmission(0x5a);  // The data transfer to 0x5A begins.  开始向0x5a传输数据
    Wire.write(0x00);
    Wire.endTransmission();     // End the data transmission.结束数据传输
    Wire.requestFrom(0x5a, 1);  // Request a byte from 0x5a.  向0x5a请求一个字节
    while (Wire.available()) {  // If data is received.  如果数据被接收到
        value = Wire.read();
    }
    M5.Lcd.print(" value = ");
    M5.Lcd.println(value, HEX);
#ifdef VALUE_SPLIT
    SensorArray[3] = (value & 0x08) >> 3;
    SensorArray[2] = (value & 0x04) >> 2;
    SensorArray[1] = (value & 0x02) >> 1;
    SensorArray[0] = (value & 0x01) >> 0;
    M5.Lcd.println("     After splitting... ");
    M5.Lcd.print("     SensorArray[0] = ");
    M5.Lcd.println(SensorArray[0]);
    M5.Lcd.print("     SensorArray[1] = ");
    M5.Lcd.println(SensorArray[1]);
    M5.Lcd.print("     SensorArray[2] = ");
    M5.Lcd.println(SensorArray[2]);
    M5.Lcd.print("     SensorArray[3] = ");
    M5.Lcd.println(SensorArray[3]);
#endif
}

void setup()
{
    M5.begin();
    M5.Power.begin();

    Wire.begin();
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(80, 0);
    M5.Lcd.println("TRACE example");
    M5.Lcd.setTextColor(WHITE);
}

void loop()
{
    M5.Lcd.fillRect(0, 20, 320, 180, BLACK);
    M5.Lcd.setCursor(100, 70);
    SensorStatus();
    delay(100);
}