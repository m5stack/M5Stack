/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit RGB LED
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 * FastLED@^3.9.10: https://github.com/FastLED/FastLED
 */

#include <M5Stack.h>
#include "FastLED.h"

#define Neopixel_PIN (21)
#define NUM_LEDS     (30)

CRGB leds[NUM_LEDS];
uint8_t gHue                              = 0;
static TaskHandle_t FastLEDshowTaskHandle = 0;
static TaskHandle_t userTaskHandle        = 0;

void setup()
{
    M5.begin();
    M5.Power.begin();

    M5.Lcd.clear(BLACK);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(40, 0);
    M5.Lcd.println("Neopixel Example");
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setCursor(0, 25);
    M5.Lcd.println("Display rainbow effect");

    // Neopixel initialization
    FastLED.addLeds<WS2811, Neopixel_PIN, GRB>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(10);
    xTaskCreatePinnedToCore(FastLEDshowTask, "FastLEDshowTask", 2048, NULL, 2, NULL, 1);
}

void loop()
{
}

void FastLEDshowESP32()
{
    if (userTaskHandle == 0) {
        userTaskHandle = xTaskGetCurrentTaskHandle();
        xTaskNotifyGive(FastLEDshowTaskHandle);
        const TickType_t xMaxBlockTime = pdMS_TO_TICKS(200);
        ulTaskNotifyTake(pdTRUE, xMaxBlockTime);
        userTaskHandle = 0;
    }
}

void FastLEDshowTask(void *pvParameters)
{
    for (;;) {
        fill_rainbow(leds, NUM_LEDS, gHue, 7);  // rainbow effect
        FastLED.show();                         // must be executed for neopixel becoming effective
        EVERY_N_MILLISECONDS(20)
        {
            gHue++;
        }
    }
}
