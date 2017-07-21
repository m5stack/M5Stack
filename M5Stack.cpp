// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "M5Stack.h"

void M5Stack::begin() {
    
    // UART 
    Serial.begin(115200);
    Serial.flush();
    Serial.println("M5Stack init...");

    // TONE
    Speaker.begin();

    // Setup the button with an internal pull-up
    pinMode(BUTTON_A_PIN, INPUT_PULLUP);
    pinMode(BUTTON_B_PIN, INPUT_PULLUP);
    pinMode(BUTTON_C_PIN, INPUT_PULLUP);

    // TF Card
    SD.begin(TFCARD_CS_PIN);

    // M5 LCD INIT
    Lcd.begin();
    Lcd.fillScreen(BLACK);
    Lcd.setCursor(0, 0);
    Lcd.setTextColor(WHITE);
    Lcd.setTextSize(1);
    Lcd.setBrightness(80);
}

void M5Stack::update() {

    //Button update
    BtnA.read();
    BtnB.read();
    BtnC.read();

    //Speaker update
    Speaker.update();
}

M5Stack m5;
