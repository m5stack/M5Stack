// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "M5Stack.h"

void m5_update();

void M5Stack::begin() {
    
    //UART 
    Serial.begin(115200);

    // TONE
    Speaker.begin();

    // Setup the button with an internal pull-up
    pinMode(BUTTON_A_PIN, INPUT_PULLUP);
    pinMode(BUTTON_B_PIN, INPUT_PULLUP);
    pinMode(BUTTON_C_PIN, INPUT_PULLUP);

    // M5 LCD INIT
    lcd.begin();
    lcd.setBrightness(100);
    lcd.fillScreen(BLACK);
    lcd.setCursor(0, 0);
    lcd.setTextColor(WHITE);
    lcd.setTextSize(1);

    //TF Card
    if(!SD.begin(TFCARD_CS_PIN)) {
        Serial.println("Card Mount Failed");
    }
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
