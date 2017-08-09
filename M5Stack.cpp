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

void M5Stack::startupLogo() {
    static uint8_t brightness, pre_brightness;
    uint32_t length = strlen((char*)m5stack_startup_music);
    uint8_t _volume = 2;
    uint16_t delay_interval = ((uint32_t)1000000/20000);
    Lcd.setBrightness(0);
    Lcd.drawBitmap(0, 0, 320, 240, (uint16_t *)gImage_logoM5);
    if(_volume != 0) {
        for(int i=0; i<length; i++) {
            dacWrite(SPEAKER_PIN, m5stack_startup_music[i]>>2);
            delayMicroseconds(40);
            brightness = (i/157);
            if(pre_brightness != brightness) {
                pre_brightness = brightness;
                Lcd.setBrightness(brightness);
            }
        }
    }

    for(int i=32; i>0; i--) {
        dacWrite(SPEAKER_PIN, i);
        delay(2);
    }

    for(int i=255; i>=0; i--) {
        lcd.setBrightness(i);
        delay(2);
    }
    Lcd.fillScreen(BLACK);
    delay(1000);
    for(int i=0; i>=100; i++) {
        Lcd.setBrightness(i);
        delay(2);
    }
    Lcd.setBrightness(100);
}

M5Stack m5;
