/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/key
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/key
*
* Describe: KEY.  按键.
* Date: 2022/6/1
*******************************************************************************
  Please follow the steps below to add FastLED library:
  - Arduino menu --> Manage Libraries... --> FastLED --> install
  在烧录前请按以下步骤添加 FastLED 库:
  - Arduino menu --> Manage Libraries... --> FastLED --> install
*/

#include <FastLED.h>
#include <M5Stack.h>

uint8_t ledColor = 0;

#define KEY_PIN  36
#define DATA_PIN 26

CRGB LED[1];

void setup() {
    M5.begin();
    M5.Lcd.setTextSize(3);
    M5.Lcd.print("\n UNIT-KEY Example\n\n    Key State:");
    /* Init key pin */
    pinMode(KEY_PIN, INPUT_PULLUP);
    /* Init RGB led */
    FastLED.addLeds<SK6812, DATA_PIN, GRB>(LED, 1);
    LED[0] = CRGB::Blue;
    FastLED.setBrightness(0);
}

void loop() {
    /* If Key was pressed */
    if (!digitalRead(KEY_PIN)) {
        M5.Lcd.setCursor(75, 130);
        M5.Lcd.print((" Pressed "));
        FastLED.setBrightness(255);
        FastLED.show();
        /* Hold until the key released */
        while (!digitalRead(KEY_PIN))
            ;
    } else {
        M5.Lcd.setCursor(75, 130);
        M5.Lcd.println(("Released"));
        FastLED.setBrightness(0);
        FastLED.show();
    }
    delay(100);
}