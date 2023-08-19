/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/products
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/products
*
* Describe: AC Socket.
* Date: 2021/9/1
*******************************************************************************
  Click button B to turn on the power. Click button A to turn off the power.
  单击按钮 B 打开电源。 单击按钮 A 关闭电源。
*/
#include <Arduino.h>
#include <M5Stack.h>

#include "modbus.h"
#include "protocol.h"

volatile uint32_t tim = 0;

uint32_t time_now  = 0;
uint8_t ucTestFlag = 0;
bool ubCoilState   = false;

void setup() {
    M5.begin(true, false, true, false);
    M5.Power.begin();
    mb_init(0xac, 9600, &tim);
    Serial1.begin(9600, SERIAL_8N1, 16, 17);
    time_now = millis();
    // put your setup code here, to run once:
}

void loop() {
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

void mb_send_one_byte(uint8_t data) {
    Serial1.write(data);
}

void protocol_callback(CmdData cmd) {
    Serial.printf("got ... \r\n");
}
