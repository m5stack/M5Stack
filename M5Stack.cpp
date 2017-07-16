// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "M5Stack.h"

void M5Stack::begin() {
  //UART 
  Serial.begin(115200);

  // Beep init
  pinMode(BEEP_PIN, OUTPUT);
  digitalWrite(BEEP_PIN, 0);

  // TONE
  ledcSetup(TONE_PIN_CHANNEL, 0, 8);
  ledcAttachPin(SPEAKER_PIN, 0);

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
  // m5.lcd.drawPicture(47, 38, 120, 96, gImage_logo);

  //TF Card
  if(!SD.begin(TFCARD_CS_PIN)) {
      Serial.println("Card Mount Failed");
  }
}

void M5Stack::update() {
  //button 
  BtnA.read();
  BtnB.read();
  BtnC.read();
}

/*
 * tone
 */
void M5Stack::tone(double freq) {
    ledcWriteTone(TONE_PIN_CHANNEL, freq);
}

/*
 * noTone
 */
void M5Stack::noTone() {
    ledcWriteTone(TONE_PIN_CHANNEL, 0);
    digitalWrite(SPEAKER_PIN, 0);
}

M5Stack m5;
