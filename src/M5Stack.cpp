// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "M5Stack.h"

M5Stack::M5Stack() : isInited(0) {

}

void M5Stack::begin(bool LCDEnable, bool SDEnable, bool SerialEnable, bool I2CEnable) {

    // Correct init once
    if (isInited) return;
    else isInited = true;

    // UART
    if (SerialEnable) {
        Serial.begin(115200);
        Serial.flush();
        delay(50);
        Serial.print("M5Stack initializing...");
    }

    // LCD INIT
    if (LCDEnable) {
        Lcd.begin();
    }

    // TF Card
    if (SDEnable) {
        SD.begin(TFCARD_CS_PIN, SPI, 40000000);
    }

    // TONE
    // Speaker.begin();

    // Set wakeup button
    Power.setWakeupButton(BUTTON_A_PIN);

    // I2C init
    if (I2CEnable) {
        Wire.begin(SDA, SCL);
    }

    if (SerialEnable) {
        Serial.println("OK!");
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

/**
  * Function has been move to Power class.(for compatibility)
  * This name will be removed in a future release.
  */
void M5Stack::setPowerBoostKeepOn(bool en) {
    M5.Power.setPowerBoostKeepOn(en);
}
/**
  * Function has been move to Power class.(for compatibility)
  * This name will be removed in a future release.
  */
void M5Stack::setWakeupButton(uint8_t button) {
    M5.Power.setWakeupButton(button);
}
/**
  * Function has been move to Power class.(for compatibility)
  * This name will be removed in a future release.
  */
void M5Stack::powerOFF() {
    M5.Power.deepSleep();
}

M5Stack M5;
