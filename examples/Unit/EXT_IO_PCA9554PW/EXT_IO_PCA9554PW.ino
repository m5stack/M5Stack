/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit EXT.IO
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 * PCA9554: https://github.com/Tinyu-Zhao/PCA9554
 */

#include <M5Stack.h>
#include "PCA9554.h"

PCA9554 ioCon1(0x27);  // Create an object at this address.  在这个地址上创建一个对象

uint8_t res;
void setup()
{
    M5.begin();
    M5.Power.begin();
    Wire.begin();
    M5.Lcd.setTextSize(2);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.setCursor(0, 90);
    M5.Lcd.print("UNIT_IO EXAMPLE\n");

    ioCon1.twiWrite(21, 22);  // Sets the I2C pin of the connection.  设置连接的I2C引脚
    delay(10);
    res = 1;
    ioCon1.twiRead(res);
    Serial.printf("res:%d\r\n", res);

    ioCon1.portMode(ALLOUTPUT);  // Set the port as all output.  设置所有引脚为输出模式
}

void loop()
{
    for (int i = 0; i < 8; i++) ioCon1.digitalWrite(i, LOW);
    delay(1000);
    for (int i = 0; i < 8; i++) ioCon1.digitalWrite(i, HIGH);
    delay(1000);

    // write 0-7 HIGH.  设置0~7号引脚为高电平
    Serial.println(ioCon1.digitalWritePort(0xff));
    delay(200);

    // write 0-7 LOW.  设置0~7号引脚为低电平
    Serial.println(ioCon1.digitalWritePort(0x00));
    delay(200);

    // write Port, the same read
    for (byte i = 0; i < 8; i++) {
        ioCon1.digitalWritePort((1 << i));
        delay(200);
    }
}
