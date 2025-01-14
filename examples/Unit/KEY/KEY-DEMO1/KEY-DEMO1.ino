/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit Key
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 * FastLED@^3.9.10: https://github.com/FastLED/FastLED
 */

#include <FastLED.h>
#include <M5Stack.h>

uint8_t ledColor = 0;

#define KEY_PIN  36
#define DATA_PIN 26

CRGB LED[1];

void setup()
{
    M5.begin();
    M5.Lcd.setTextSize(3);
    M5.Lcd.print("\n UNIT-KEY Example\n\n    Key State:");
    /* Init key pin */
    pinMode(KEY_PIN, INPUT_PULLUP);
    /* Init RGB led */
    FastLED.addLeds<SK6812, DATA_PIN, GRB>(LED, 1);
    LED[0] = CRGB::Blue;
    FastLED.setBrightness(0);
}

void loop()
{
    /* If Key was pressed */
    if (!digitalRead(KEY_PIN)) {
        M5.Lcd.setCursor(75, 130);
        M5.Lcd.print((" Pressed "));
        FastLED.setBrightness(255);
        FastLED.show();
        /* Hold until the key released */
        while (!digitalRead(KEY_PIN));
    } else {
        M5.Lcd.setCursor(75, 130);
        M5.Lcd.println(("Released"));
        FastLED.setBrightness(0);
        FastLED.show();
    }
    delay(100);
}