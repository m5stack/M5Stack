/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit RS485
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <M5Stack.h>

#define RX_PIN (16)
#define TX_PIN (17)
#define X_OFF  (160)
#define Y_OFF  (30)

int i = 0, s = 0;

void setup()
{
    M5.begin();
    M5.Power.begin();
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("RS485 Unit test", 75, 3, 4);
    Serial2.begin(115200, SERIAL_8N1, RX_PIN,
                  TX_PIN);  // Set the baud rate of serial port 2 to 115200,8 data bits,
                            // no parity bits, and 1 stop bit, and set RX to 16 and TX
                            // to 17.
                            // 设置串口二的波特率为115200,8位数据位,没有校验位,1位停止位,并设置RX为16,TX为17
}

void loop()
{
    Serial2.write("Hello\n");

    if (Serial2.available()) {
        M5.Lcd.print(char(Serial2.read()));
    }
    delay(100);
}
