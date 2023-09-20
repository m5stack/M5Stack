/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information:
https://docs.m5stack.com/en/module/stepmotor_driver
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/module/stepmotor_driver
*
* Describe: GRBL 13.2 Module.
* Date: 2021/11/14
*******************************************************************************
  GRBL 13.2 Module TEST Example,use I2C to control stepper motors(Stack two
Module at the same time)
  步进电机模块测试示例,使用I2C控制步进电机(同时堆叠两个模块)
*/
#include <M5Stack.h>

#include "Module_GRBL_13.2.h"

/*
 * The I2C address of GRBL 13.2  Module is 0x70 by default.
 * GRBL 13.2 模块的 I2C 地址默认为 0x70。
 * You could use the DIP Switch for modify I2C address to 0x71
 * 您可以使用拨码开关将 I2C 地址修改为 0x71
 */

#define STEPMOTOR_I2C_ADDR_1 0x70
#define STEPMOTOR_I2C_ADDR_2 0x71

Module_GRBL _GRBL_A(STEPMOTOR_I2C_ADDR_1);
Module_GRBL _GRBL_B(STEPMOTOR_I2C_ADDR_2);

void setup() {
    // put your setup code here, to run once:
    M5.begin();
    M5.Power.begin();
    Wire.begin(21, 22);
    _GRBL_A.Init(&Wire);
    _GRBL_B.Init(&Wire);
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
    _GRBL_A.setMode("absolute");
    _GRBL_B.setMode("absolute");
}

void loop() {
    /*
        If Button A was pressed,
        stepmotor will rotate back and forth at a time
     */
    if (M5.BtnA.wasPressed())  // A button
    {
        Serial.print(_GRBL_A.readStatus());
        _GRBL_A.setMotor(5, 5, 5, 200);
        _GRBL_B.setMotor(5, 5, 5, 200);
        _GRBL_A.setMotor(0, 0, 0, 200);
        _GRBL_B.setMotor(0, 0, 0, 200);
    }

    if (M5.BtnB.wasPressed()) {
        // USE Gcode
        _GRBL_A.sendGcode("G1 X5Y5Z5 F200");
        _GRBL_B.sendGcode("G1 X5Y5Z5 F200");
        _GRBL_A.sendGcode("G1 X0Y0Z0 F200");
        _GRBL_B.sendGcode("G1 X0Y0Z0 F200");
    }

    if (M5.BtnC.wasReleased()) {
        _GRBL_A.unLock();
        _GRBL_B.unLock();
    }
    M5.update();
}
