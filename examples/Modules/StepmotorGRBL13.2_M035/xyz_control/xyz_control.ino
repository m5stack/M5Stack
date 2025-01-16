/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Module13.2 GRBL
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 * M5Module-GRBL-13.2：https://github.com/m5stack/M5Module-GRBL-13.2
 */

#include <M5Stack.h>
#include "Module_GRBL_13.2.h"

// GRBL 13.2 Module TEST Example,use I2C to control stepper motors
// 步进电机模块测试示例,使用I2C控制步进电机

/*
 * The I2C address of GRBL 13.2  Module is 0x70 by default.
 * GRBL 13.2 模块的 I2C 地址默认为 0x70。
 * You could use the DIP Switch for modify I2C address to 0x71
 * 您可以使用拨码开关将 I2C 地址修改为 0x71
 */

#define STEPMOTOR_I2C_ADDR 0x70
// #define STEPMOTOR_I2C_ADDR 0x71

Module_GRBL _GRBL(STEPMOTOR_I2C_ADDR);

void setup()
{
    // put your setup code here, to run once:
    M5.begin();
    M5.Power.begin();
    Wire.begin(21, 22);
    _GRBL.Init(&Wire);
    Serial.begin(115200);
    m5.Lcd.setTextColor(WHITE, BLACK);
    m5.Lcd.setTextSize(3);
    m5.lcd.setBrightness(100);
    M5.Lcd.setCursor(80, 40);
    M5.Lcd.println("GRBL 13.2");
    M5.Lcd.setCursor(50, 80);
    M5.Lcd.println("Press Btn A/B");
    M5.Lcd.setCursor(50, 120);
    M5.Lcd.println("Control Motor");
    _GRBL.setMode("absolute");
}

void loop()
{
    // If Button A was pressed,stepmotor will rotate back and forth at a time
    // 如果按下按钮 A，步进电机将一次来回旋转
    if (M5.BtnA.wasPressed())  // A button
    {
        Serial.print(_GRBL.readStatus());
        _GRBL.setMotor(5, 5, 5, 200);
        _GRBL.setMotor(0, 0, 0, 200);
    }

    if (M5.BtnB.wasPressed()) {
        // USE Gcode
        _GRBL.sendGcode("G1 X5Y5Z5 F200");
        _GRBL.sendGcode("G1 X0Y0Z0 F200");
    }

    if (M5.BtnC.wasReleased()) {
        _GRBL.unLock();
    }
    M5.update();
}
