// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "M5Stack.h"

void M5Stack::begin()
{
  Serial.begin(115200);

  // Beep init
  pinMode(BEEP_PIN, OUTPUT);
  digitalWrite(BEEP_PIN, 0);

  // LED init
  pinMode(LED_PIN, OUTPUT);

  // TONE
  ledcSetup(TONE_PIN_CHANNEL, 0, 8);
  ledcAttachPin(SPEAKER_PIN, 0);
//   ledcWrite(TONE_PIN_CHANNEL, 128);
//   ledcWriteTone(TONE_PIN_CHANNEL, double freq);

  // Setup the button with an internal pull-up
  pinMode(BUTTON_A_PIN, INPUT_PULLUP);
  pinMode(BUTTON_B_PIN, INPUT_PULLUP);
  pinMode(BUTTON_C_PIN, INPUT_PULLUP);

  // M5 LCD INIT
  lcd.begin();
//   pinMode(LCD_LED_PIN, OUTPUT);
//   digitalWrite(LCD_LED_PIN, 1);
  lcd.fillScreen(BLACK);
  lcd.setCursor(0, 0);
  lcd.setTextColor(WHITE);
  lcd.setTextSize(1);

  // m5.lcd.drawPicture(47, 38, 120, 96, gImage_logo);
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

uint8_t M5Stack::bootSetup()
{
    uint8_t select_app_id;
//   //-------BOOT MENU---------
//   lcd.fillScreen(WHITE);
//   lcd.setFont(&FreeSansOblique9pt7b);

//   //--------APP STORE---------
//   lcd.drawPicture(0, 0, 220, 175, gImage_select_backguand);
//   lcd.fillRect(0, 30, 219, 120, WHITE);
//   select_app_id = selectMenu();
//   Serial.printf("downloading the app:%d\r\n", select_app_id);
//   lcd.fillScreen(BLACK);
//   lcd.setTextColor(WHITE);
//   lcd.setFont();
//   lcd.setTextSize(1);
//   lcd.setCursor(2, 20);

//   //----------downloading----------
//   lcd.fillScreen(WHITE);
//   drawTitle("LOADING...", GRAY);
//   lcd.drawPicture(47, 38, 120, 96, gImage_logo);

//   int progress_p=0;
//   while(++progress_p < 100) {
//     // Serial.printf("progress:%d%%\r\n", progress_p);
//     lcd.ProgressBar(20, 146, 180, 13, progress_p);
//     delay(1);
//   }

  return select_app_id;
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

/**
 * returns true ONCE when 'button' is held for 'time' frames
 * @param button The button's ID
 * @param time How much frames button must be held, between 1 and 254.
 * @return true when 'button' is held for 'time' frames
 */
bool M5Stack::held(uint8_t button, uint8_t time){
    if(btn_states[button] == (time+1))
        return true;
    else
        return false;
}

/**
 * returns true every 'period' frames when 'button' is held
 * @param button The button's ID
 * @param period How much frames button must be held, between 1 and 254.
 * @return true if the button is held for the given time
 */
bool M5Stack::repeat(uint8_t button, uint8_t period) {
    if (period <= 1) {
        if ((btn_states[button] != 0xFF) && (btn_states[button]))
            return true;
    } else {
        if ((btn_states[button] != 0xFF) && ((btn_states[button] % period) == 1))
            return true;
    }
    return false;
}

/**
 * 
 * @param button The button's ID
 * @return The number of frames during which the button has been held.
 */
uint8_t M5Stack::timeHeld(uint8_t button){
    if(btn_states[button] != 0xFF)
        return btn_states[button];
    else
        return 0;
}

void M5Stack::tone(double freq) {
    ledcWriteTone(TONE_PIN_CHANNEL, freq);
}

void M5Stack::noTone() {
    ledcWriteTone(TONE_PIN_CHANNEL, 0);
    digitalWrite(SPEAKER_PIN, 0);
}

M5Stack m5;
