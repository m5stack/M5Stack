/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information:
* 获取更多资料请访问:
* https://docs.m5stack.com/en/module/RS232F%Module%13.2
* https://docs.m5stack.com/zh_CN/module/RS232F%Module%13.2
*
* Describe: RS232.
* Date: 2023/3/23
*******************************************************************************
*/

#include <M5Stack.h>

/* Define your pin here */
/* And should be the same as you module setup */
#define RX_PIN 16
#define TX_PIN 17

void setup() {
    M5.begin();
    M5.Power.begin();
    M5.Lcd.setTextSize(1);
    M5.Lcd.drawString("Module RS232 test", 75, 3, 4);
    Serial2.begin(
        115200, SERIAL_8N1, RX_PIN,
        TX_PIN);  // Set the baud rate of serial port 2 to 115200,8 data bits,
                  // no parity bits, and 1 stop bit, and set RX to 16 and TX
                  // to 17.
                  // 设置串口二的波特率为115200,8位数据位,没有校验位,1位停止位,并设置RX,TX引脚
}

void loop() {
    Serial2.write("Hello\n");

    if (Serial2.available()) {
        M5.Lcd.print(char(Serial2.read()));
    }
    delay(100);
}
