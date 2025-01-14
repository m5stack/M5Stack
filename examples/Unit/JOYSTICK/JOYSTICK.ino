/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit Joystick v1.1
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <M5Stack.h>

#define JOY_ADDR (0x52)  // define Joystick I2C address.  定义摇杆的I2C地址

void setup()
{
    M5.begin();
    M5.Power.begin();
    M5.Lcd.setCursor(70, 0, 4);
    M5.Lcd.println(("Joystick Test"));
    dacWrite(25, 0);  // disable the speak noise.  禁用语音噪音
    Wire.begin(21, 22, 400000UL);
}

char data[100];
void loop()
{
    static uint8_t x_data, y_data, button_data;
    Wire.requestFrom(JOY_ADDR,
                     3);     // Request 3 bytes from the slave device.  向从设备请求3个字节
    if (Wire.available()) {  // If data is received.  如果接收到数据
        x_data      = Wire.read();
        y_data      = Wire.read();
        button_data = Wire.read();
        sprintf(data, "x:%d y:%d button:%d\n", x_data, y_data, button_data);
        Serial.print(data);

        M5.Lcd.setCursor(100, 50, 4);
        M5.Lcd.printf("X:%d      ", x_data);
        M5.Lcd.setCursor(100, 80, 4);
        M5.Lcd.printf("Y:%d      ", y_data);
        M5.Lcd.setCursor(100, 110, 4);
        M5.Lcd.printf("B:%d      ", button_data);
    }
    delay(200);
}
