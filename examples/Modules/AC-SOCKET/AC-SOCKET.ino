/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + AC-Socket
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <Arduino.h>
#include <M5Stack.h>
#include "modbus.h"
#include "protocol.h"

// Click button B to turn on the power. Click button A to turn off the power.
// 单击按钮 B 打开电源。 单击按钮 A 关闭电源。

volatile uint32_t tim = 0;
uint32_t time_now     = 0;
uint8_t ucTestFlag    = 0;
bool ubCoilState      = false;

void setup()
{
    M5.begin(true, false, true, false);
    M5.Power.begin();
    mb_init(0xac, 9600, &tim);
    Serial1.begin(9600, SERIAL_8N1, 16, 17);
    time_now = millis();
    // put your setup code here, to run once:
}

void loop()
{
    M5.update();

    if (millis() - time_now > 60000UL) {
        time_now = millis();
        if (ucTestFlag) {
            if (ubCoilState) {
                char data_str[] = {0xAA, 5, 0x00, 0x00, 0x00, 0x00};
                mb_send_frame((uint8_t *)data_str, 6);
            } else {
                char data_str[] = {0xAA, 5, 0x00, 0x00, 0xff, 0x00};
                mb_send_frame((uint8_t *)data_str, 6);
            }
            ubCoilState = 1 - ubCoilState;
        }
    }

    if (M5.BtnA.wasPressed()) {
        char data_str[] = {0xAA, 5, 0x00, 0x00, 0x00, 0x00};
        mb_send_frame((uint8_t *)data_str, 6);
    }

    if (M5.BtnB.wasPressed()) {
        char data_str[] = {0xAA, 5, 0x00, 0x00, 0xff, 0x00};
        mb_send_frame((uint8_t *)data_str, 6);
    }

    if (M5.BtnC.wasPressed()) {
        ucTestFlag = 1 - ucTestFlag;
    }

    vTaskDelay(pdMS_TO_TICKS(10));

    // while(Serial1.available()) {
    //   protocol_rec_put(Serial1.read());
    //   micros();
    // }
    // put your main code here, to run repeatedly:
}

void mb_send_one_byte(uint8_t data)
{
    Serial1.write(data);
}

void protocol_callback(CmdData cmd)
{
    Serial.printf("got ... \r\n");
}
