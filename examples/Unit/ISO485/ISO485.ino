/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/iso485
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/iso485
*
* Describe: iso485.
* Date: 2021/8/30
*******************************************************************************
  Please connect to PortC,Pressed ButtonA :send "hello world"
  请连接端口C,Pressed ButtonA :send "hello world"
*/

#include <M5Stack.h>

String str = "";

void setup() {
    M5.begin();
    M5.Lcd.drawString("ISO485", 20, 0, 2);
    Serial2.begin(115200, SERIAL_8N1, 16, 17);
    M5.Lcd.setCursor(0, 20);
}

void loop() {
    if (M5.BtnA.wasPressed()) {
        Serial2.write("Hello World\r\n");
    }

    if (Serial2.available()) {
        char ch = Serial2.read();
        str += ch;
        if (str.endsWith("\r\n")) {
            Serial.print(str);
            M5.Lcd.print(str);
            str = "";
        }
    }
    M5.update();
}
