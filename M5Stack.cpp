// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "M5Stack.h"

void M5Stack::begin()
{
  //UART 
  Serial.begin(115200);

  // Beep init
  pinMode(BEEP_PIN, OUTPUT);
  digitalWrite(BEEP_PIN, 0);

  // LED init
  pinMode(LED_PIN, OUTPUT);

  // TONE
  ledcSetup(TONE_PIN_CHANNEL, 0, 8);
  ledcAttachPin(SPEAKER_PIN, 0);

  // Setup the button with an internal pull-up
  pinMode(BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);
  pinMode(BUTTON_C_PIN, INPUT_PULLUP);

  // M5 LCD INIT
  lcd.begin();
  lcd.setBrightness(128);
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

void M5Stack::loop()
{
  //button 
  buttonA.read();
  buttonB.read();
  buttonC.read();

  // TFTLCD button update
//   lcd.buttonUpdate();
}

/*
 * Turn ON LED
 */
void M5Stack::ledOn() {
    digitalWrite(LED_PIN, 1);
}

/*
 * Turn OFF LED
 */
void M5Stack::ledOff() {
    digitalWrite(LED_PIN, 0);
}

/*
 * ledTrig LED
 */
void M5Stack::ledTrig() {
    static bool tgbit;
    digitalWrite(LED_PIN, tgbit);
    tgbit = !tgbit;
}

/*
 * Returns true when 'button' is pressed.
 * The button has to be released for it to be triggered again.
 */
bool M5Stack::pressed(uint8_t button) {
    if(button == BTN_A) {
        return buttonA.wasPressed();
    } else if(button == BTN_B) {
        return buttonB.wasPressed();
    } else if(button == BTN_C) {
        return buttonC.wasPressed();
    }
}

/*
 * return true if 'button' is released
 */
bool M5Stack::released(uint8_t button) {
    if(button == BTN_A) {
        return buttonA.wasReleased();
    } else if(button == BTN_B) {
        return buttonB.wasReleased();
    } else if(button == BTN_C) {
        return buttonC.wasReleased();
    }
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
