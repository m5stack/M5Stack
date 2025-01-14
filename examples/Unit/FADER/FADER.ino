/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit Fader
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 * FastLED@^3.9.10: https://github.com/FastLED/FastLED
 */

#include "FastLED.h"
#include "M5Stack.h"

// How many leds in your strip?
#define NUM_LEDS   (14)
#define INPUT_PINS (36)
#define DATA_PIN   (26)

// Define the array of leds
CRGB leds[NUM_LEDS];

uint8_t beginHue   = 0;
uint8_t deltaHue   = 30;
uint8_t brightness = 100;
uint16_t rawADC    = 0;

void setup()
{
    M5.begin();
    M5.Lcd.setTextDatum(MC_DATUM);
    M5.Lcd.drawString("FADER UNIT TEST", 160, 60, 4);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    delay(1000);
    pinMode(36, INPUT);
    fill_rainbow(leds, NUM_LEDS, beginHue, deltaHue);
}

void loop()
{
    rawADC     = analogRead(INPUT_PINS);        // Read ADC value 读取ADC数值
    brightness = map(rawADC, 0, 4095, 0, 255);  // The mapping ADC value is the brightness value
                                                // range  映射ADC值为亮度值范围
    FastLED.setBrightness(brightness);          // Adjust the brightness of the FADER
                                                // LED 调整FADER LED灯亮度
    FastLED.show();
    Serial.printf("%d\r\n", rawADC);
    M5.Lcd.fillRect(0, 120, 320, 100, BLACK);
    M5.Lcd.drawString("value: " + String(rawADC), 160, 160, 4);
    delay(100);
}