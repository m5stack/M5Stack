// Copyright (c) M5Stack. All rights reserved.

// Licensed under the MIT license. See LICENSE file in the project root for full license information.
/**
 * \par Copyright (C), 2016-2017, M5Stack
 * \class M5Stack
 * \brief   M5Stack library.
 * @file    M5Stack.h
 * @author  M5Stack
 * @version V0.2.4
 * @date    2018/10/29
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

    Power:
        M5.Power.setPowerBoostKeepOn()
        M5.Power.setCharge(uint8_t mode);
        M5.Power.setPowerBoostKeepOn(bool en);
        M5.Power.isChargeFull();
        M5.Power.setWakeupButton(uint8_t button);
        M5.Power.powerOFF();

        bool setPowerBoostOnOff(bool en);
        bool setPowerBoostSet(bool en);
        bool setPowerVin(bool en);
        bool setPowerWLEDSet(bool en);

    LCD:
        M5.lcd.setBrightness(uint8_t brightness);
        M5.Lcd.drawPixel(int16_t x, int16_t y, uint16_t color);
        M5.Lcd.drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
        M5.Lcd.fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
        M5.Lcd.fillScreen(uint16_t color);
        M5.Lcd.drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
        M5.Lcd.drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,uint16_t color);
        M5.Lcd.fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
        M5.Lcd.fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername,int16_t delta, uint16_t color);
        M5.Lcd.drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
        M5.Lcd.fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
        M5.Lcd.drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
        M5.Lcd.fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
        M5.Lcd.drawBitmap(int16_t x, int16_t y, const uint8_t bitmap[], int16_t w, int16_t h, uint16_t color);
        M5.Lcd.drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h),
        M5.Lcd.drawChar(uint16_t x, uint16_t y, char c, uint16_t color, uint16_t bg, uint8_t size);
        M5.Lcd.setCursor(uint16_t x0, uint16_t y0);
        M5.Lcd.setTextColor(uint16_t color);
        M5.Lcd.setTextColor(uint16_t color, uint16_t backgroundcolor);
        M5.Lcd.setTextSize(uint8_t size);
        M5.Lcd.setTextWrap(boolean w);
        M5.Lcd.printf();
        M5.Lcd.print();
        M5.Lcd.println();
        M5.Lcd.drawCentreString(const char *string, int dX, int poY, int font);
        M5.Lcd.drawRightString(const char *string, int dX, int poY, int font);
        M5.Lcd.drawJpg(const uint8_t *jpg_data, size_t jpg_len, uint16_t x, uint16_t y);
        M5.Lcd.drawJpgFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y);
        M5.Lcd.drawBmpFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y);

    Button:
        M5.BtnA/B/C.read();
        M5.BtnA/B/C.isPressed();
        M5.BtnA/B/C.isReleased();
        M5.BtnA/B/C.wasPressed();
        M5.BtnA/B/C.wasReleased();
        M5.BtnA/B/C.wasreleasedFor()
        M5.BtnA/B/C.pressedFor(uint32_t ms);
        M5.BtnA/B/C.releasedFor(uint32_t ms);
        M5.BtnA/B/C.lastChange();

    Speaker:
        M5.Speaker.tone(uint32_t freq);
        M5.Speaker.tone(freq, time);
        M5.Speaker.beep();
        M5.Speaker.setBeep(uint16_t frequency, uint16_t duration);
        M5.Speaker.mute();

 *
 * \par History:
 * <pre>
 * `<Author>`         `<Time>`        `<Version>`        `<Descr>`
 * Zibin Zheng         2017/07/14        0.0.1          Rebuild the new.
 * Bin                 2018/10/29        0.2.4          Add Button API
 * </pre>
 *
 */
// #define ESP32

#ifndef _M5STACK_H_
  #define _M5STACK_H_

  #if defined(ESP32)

    #include "gitTagVersion.h"
    #include <Arduino.h>
    #include <Wire.h>
    #include <SPI.h>
    #include "FS.h"
    #include "SD.h"

    #include "M5Display.h"
    #include "utility/Config.h"
    #include "utility/M5Button.h"
    #include "utility/Speaker.h"
    #include "utility/Power.h"
    #include "utility/CommUtil.h"

    #if defined(M5STACK_MPU6886) || defined(M5STACK_MPU9250) || defined(M5STACK_MPU6050)
    #include "utility/MPU6886.h"
    #elif defined M5STACK_200Q
    #include "utility/SH200Q.h"
    #endif

    class M5Stack
    {
      public:
        M5Stack();
        void begin(bool LCDEnable = true, bool SDEnable = true, bool SerialEnable = true, bool I2CEnable = false);
        void update();

        // Buttons (for things that involve all buttons)
        M5Buttons Buttons;

        // Button API
        #define DEBOUNCE_MS 10
        Button BtnA = Button(BUTTON_A_PIN, true, DEBOUNCE_MS, "hw",
                             3, 218, 102, 21, true, "BtnA");
        Button BtnB = Button(BUTTON_B_PIN, true, DEBOUNCE_MS, "hw",
                             109, 218, 102, 21, true, "BtnB");
        Button BtnC = Button(BUTTON_C_PIN, true, DEBOUNCE_MS, "hw",
                             215, 218, 102, 21, true, "BtnC");

        // SPEAKER
        SPEAKER Speaker;

        // LCD
        M5Display Lcd;

        //Power
        POWER Power;

        // UART
        // HardwareSerial Serial0 = HardwareSerial(0);
        // HardwareSerial Serial2 = HardwareSerial(2);

        #if defined(M5STACK_MPU6886) || defined(M5STACK_MPU9250) || defined(M5STACK_MPU6050)
        MPU6886 IMU = MPU6886();
        #elif defined M5STACK_200Q
        SH200Q IMU = SH200Q();
        #endif

        // I2C
        CommUtil I2C;

        /**
        * Function has been move to Power class.(for compatibility)
        * This name will be removed in a future release.
        */
        void setPowerBoostKeepOn(bool en) __attribute__((deprecated));
        void setWakeupButton(uint8_t button) __attribute__((deprecated));
        void powerOFF() __attribute__((deprecated));

      private:
          bool isInited;
    };

    extern M5Stack M5;
    #define m5 M5
    #define lcd Lcd
  #else
    #error “This library only supports boards with ESP32 processor.”
  #endif
#endif
