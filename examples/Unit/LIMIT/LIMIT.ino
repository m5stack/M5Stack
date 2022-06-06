/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information:
* 获取更多资料请访问:
*
* describe: Limit.
* date: 2022/6/1
*******************************************************************************
*/

#include <M5Stack.h>

#define KEY_PIN 22  // Define Limit Pin.  定义Limit连接引脚

void setup() {
    M5.begin();  // Init M5Stack  初始化M5Stack
    M5.Lcd.setTextSize(4);
    M5.Lcd.print(("\n  UNIT-LIMIT\n  Example"));

    pinMode(KEY_PIN, INPUT_PULLUP);  // Init Limit pin.  初始化Limit引脚.
}

void loop() {
    if (!digitalRead(KEY_PIN)) {  // If Limit was hit.  如果触碰了Limit.
        M5.Lcd.setCursor(0, 130);
        M5.Lcd.print(("  Hit limit!"));
    } else {
        M5.Lcd.setCursor(0, 130);
        M5.Lcd.println(("            "));
    }
    delay(100);
}