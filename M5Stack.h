// Copyright (c) M5Stack. All rights reserved.

// Licensed under the MIT license. See LICENSE file in the project root for full license information.
/**
 * \par Copyright (C), 2012-2016, M5Stack
 * \class M5Stack
 * \brief   M5Stack library.
 * @file    M5Stack.h
 * @author  M5Stack
 * @version V1.0.1
 * @date    2016/04/07
 * @brief   Header for M5Stack.cpp module
 *
 * \par Description
 * This file is a drive for M5Stack core.
 *
 * \par Method List:
 *    
 *  System:
        M5.begin();
        M5.update();

    LCD:
        M5.lcd.setBrightness(uint8_t brightness);
        M5.lcd.drawPixel(int16_t x, int16_t y, uint16_t color);
        M5.lcd.drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
        M5.lcd.fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
        M5.lcd.fillScreen(uint16_t color);
        M5.lcd.drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
        M5.lcd.drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,uint16_t color);
        M5.lcd.fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
        M5.lcd.fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,int16_t delta, uint16_t color);
        M5.lcd.drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
        M5.lcd.fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
        M5.lcd.drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
        M5.lcd.fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
        M5.lcd.drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
        M5.lcd.drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h),
        M5.lcd.drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
        M5.lcd.setCursor(uint16_t x0, uint16_t y0);
        M5.lcd.setTextColor(uint16_t color);
        M5.lcd.setTextColor(uint16_t color, uint16_t backgroundcolor);
        M5.lcd.setTextSize(uint8_t size);
        M5.lcd.setTextWrap(boolean w);

    Button:
        M5.btnA/B/C.read();
        M5.btnA/B/C.isPressed();
        M5.btnA/B/C.isReleased();
        M5.btnA/B/C.wasPressed();
        M5.btnA/B/C.wasReleased();
        M5.btnA/B/C.pressedFor(uint32_t ms);
        M5.btnA/B/C.releasedFor(uint32_t ms);
        M5.btnA/B/C.lastChange();

    Speaker:
        M5.speaker.tone(freq);
        M5.speaker.tone(freq, time);
        M5.speaker.beep();
        M5.speaker.mute();

 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Zibin Zheng         2017/07/14        0.0.1          Rebuild the new.
 * </pre>
 *
 */

#ifndef _M5STACK_H_
#define _M5STACK_H_

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiMulti.h>
#include <Wire.h>
#include "FS.h"
#include "SD.h"
#include "esp32-hal-dac.h"
#include "utility/Display.h"
#include "utility/bmp_map.h"
#include "utility/config.h"
#include "utility/Button.h"

#ifdef ENABLE_DEBUG
#define M5PUTLOG(X)     Serial.printf(X)
#else
#define M5PUTLOG(X)
#endif

class M5Stack {

 public:
    void begin();
    void update();

    // Button API
    #define DEBOUNCE_MS 20
    #define PULLUP true
    #define INVERT true
    Button BtnA = Button(BUTTON_A_PIN, PULLUP, INVERT, DEBOUNCE_MS);    //Declare the button
    Button BtnB = Button(BUTTON_B_PIN, PULLUP, INVERT, DEBOUNCE_MS);
    Button BtnC = Button(BUTTON_C_PIN, PULLUP, INVERT, DEBOUNCE_MS);

    // Tone
    #define TONE_PIN_CHANNEL   0
    void tone(double freq);
    void noTone();

    // TFT;
    Adafruit_ILI9341 Lcd = Adafruit_ILI9341(TFT_CS_PIN, TFT_DC_PIN, TFT_RST_PIN);

 private:

};

extern M5Stack m5stack;
#define m5 m5stack
#define M5 m5stack
#define lcd Lcd

#endif
