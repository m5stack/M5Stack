
/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/can
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/can
*
* Describe: CA-IS3050G.  CAN transceiver unit
* Date: 2021/11/01
******************************************** ***********************************
  Please connect port B (26, 36), the device will automatically send and receive
messages through the CAN bus If there are phenomena such as failure of normal
communication, please check the communication rate or add a terminal resistance
between the H and L phases Please install library before compiling: ESP32CAN:
https://github.com/miwagner/ESP32-Arduino-CAN
*/

#include <CAN_config.h>
#include <ESP32CAN.h>
#include <M5GFX.h>
#include <M5Stack.h>

M5GFX display;
M5Canvas canvas(&display);

CAN_device_t CAN_cfg;  // CAN Config
unsigned long previousMillis =
    0;  // will store last time a CAN Message was send
const int interval =
    1000;  // interval at which send CAN Messages (milliseconds)
const int rx_queue_size = 10;  // Receive Queue size

uint8_t count = 0;

void setup() {
    M5.begin();
    Serial.println("Basic Demo - ESP32-Arduino-CAN");

    display.begin();

    canvas.setColorDepth(1);  // mono color
    canvas.createSprite(display.width(), display.height());
    canvas.setTextSize(1);
    canvas.setPaletteColor(1, GREEN);
    canvas.setTextScroll(true);

    CAN_cfg.speed     = CAN_SPEED_125KBPS;
    CAN_cfg.tx_pin_id = GPIO_NUM_26;
    CAN_cfg.rx_pin_id = GPIO_NUM_36;
    CAN_cfg.rx_queue  = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
    // Init CAN Module
    ESP32Can.CANInit();

    canvas.println("Init CAN Module.....");
    canvas.println("Waiting Frame");
    canvas.println(
        "If there are phenomena such as failure of normal communication");
    canvas.println(
        "Please check the communication rate or add a terminal resistance "
        "between the H and L phases");
    canvas.pushSprite(0, 0);
}

void loop() {
    CAN_frame_t rx_frame;

    unsigned long currentMillis = millis();

    // Receive next CAN frame from queue
    if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) ==
        pdTRUE) {
        if (rx_frame.FIR.B.FF == CAN_frame_std) {
            Serial.println("New standard frame");
            canvas.println("New standard frame");
        } else {
            Serial.println("New extended frame");
            canvas.println("New extended frame");
        }

        if (rx_frame.FIR.B.RTR == CAN_RTR) {
            Serial.printf("RTR from 0x%08X, DLC %d\r\n", rx_frame.MsgID,
                          rx_frame.FIR.B.DLC);
            canvas.printf("RTR from 0x%08X, DLC %d\r\n", rx_frame.MsgID,
                          rx_frame.FIR.B.DLC);
        } else {
            Serial.printf("from 0x%08X, DLC %d, Data \r\n", rx_frame.MsgID,
                          rx_frame.FIR.B.DLC);
            canvas.printf("from 0x%08X, DLC %d, Data \r\n", rx_frame.MsgID,
                          rx_frame.FIR.B.DLC);
            for (int i = 0; i < rx_frame.FIR.B.DLC; i++) {
                Serial.printf("0x%02X ", rx_frame.data.u8[i]);
                canvas.printf("0x%02X ", rx_frame.data.u8[i]);
            }
            Serial.println("\n");
            canvas.println("\n");
        }
        canvas.pushSprite(0, 0);
    }
    // Send CAN Message
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        CAN_frame_t tx_frame;
        tx_frame.FIR.B.FF   = CAN_frame_std;
        tx_frame.MsgID      = 0x001;
        tx_frame.FIR.B.DLC  = 8;
        tx_frame.data.u8[0] = 0x00;
        tx_frame.data.u8[1] = 0x01;
        tx_frame.data.u8[2] = 0x02;
        tx_frame.data.u8[3] = 0x03;
        tx_frame.data.u8[4] = 0x04;
        tx_frame.data.u8[5] = 0x05;
        tx_frame.data.u8[6] = 0x06;
        tx_frame.data.u8[7] = 0x07;
        ESP32Can.CANWriteFrame(&tx_frame);
    }
    delay(1);
}
