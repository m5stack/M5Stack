/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Dependent Library:
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * M5GFX@^0.2.3: https://github.com/m5stack/M5GFX
 * M5Unified@^0.2.2: https://github.com/m5stack/M5Unified
 * ClosedCube_I2C_Arduino: https://github.com/HariMurti12/ClosedCube_I2C_Arduino
 * ClosedCube_TCA9548A_Arduino: https://github.com/blacksn0w13/ClosedCube_TCA9548A_Arduino
 */

#include <M5Unified.h>
#include <Wire.h>
#include "ClosedCube_TCA9548A.h"

#define FRONT    (2)
#define X_LOCAL  (100)
#define Y_LOCAL  (35)
#define X_OFFSET (160)
#define Y_OFFSET (34)

#define PaHub_I2C_ADDRESS (0x70)

ClosedCube::Wired::TCA9548A tca9548a;

void setup()
{
    M5.begin();
    M5.Power.begin();
    tca9548a.address(PaHub_I2C_ADDRESS);  // Set the I2C address.  设置I2C地址
    M5.Lcd.setTextFont(4);
    M5.Lcd.setCursor(70, 0, 4);
    M5.Lcd.setTextColor(YELLOW, TFT_BLACK);
    M5.Lcd.println(("PaHUB Example"));
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
}

void loop()
{
    uint8_t returnCode = 0;
    uint8_t address;
    // Please connect to Port A, Use PaHUB Unit to expand multiple I2C devices and scan the I2C addresses of the slave
    // devices in order. 请连接端口A, 使用PaHUB Unit扩展多个I2C设备，并依次扫描从设备的I2C地址。
    for (uint8_t channel = 0; channel < TCA9548A_MAX_CHANNELS; channel++) {
        M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET * channel, FRONT);
        M5.Lcd.printf("                                                                ");
        M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET * channel, FRONT);
        M5.Lcd.printf("CH%d : ", channel);
        returnCode = tca9548a.selectChannel(channel);
        if (returnCode == 0) {
            for (address = 0x01; address < 0x7F; address++) {
                Wire.beginTransmission(address);
                returnCode = Wire.endTransmission();
                if (returnCode == 0) {
                    Serial.print("I2C device = ");
                    M5.Lcd.printf("0X%X  ", address);
                }
            }
        }
        delay(200);
    }
}
