/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit TVOC/eCO2
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 * Adafruit_SGP30: https://github.com/adafruit/Adafruit_SGP30
 */

#include <M5Stack.h>
#include "Adafruit_SGP30.h"

Adafruit_SGP30 sgp;
long last_millis = 0;

void setup()
{
    M5.begin(true, false, true, true);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(80, 0);
    M5.Lcd.println("TVOC TEST");
    if (!sgp.begin()) {  // Init the sensor. 初始化传感器
        M5.Lcd.println("Sensor not found");
        while (1);
    }
    M5.Lcd.setCursor(0, 80);
    M5.Lcd.println("\nInitialization...");
}

void loop()
{
    static int i = 15;
    while (i > 0) {
        if (millis() - last_millis > 1000) {
            last_millis = millis();
            i--;
            M5.Lcd.fillRect(20, 120, 60, 30, BLACK);
            M5.Lcd.drawNumber(i, 20, 120, 2);
        }
    }
    M5.Lcd.fillRect(0, 80, 90, 100, BLACK);

    if (!sgp.IAQmeasure()) {  // Commands the sensor to take a single eCO2/VOC
                              // measurement.  命令传感器进行一次eCO2/VOC测量
        Serial.println("Measurement failed");
        return;
    }
    M5.Lcd.fillRect(100, 40, 220, 90, TFT_BLACK);
    M5.Lcd.setCursor(0, 50);
    M5.Lcd.printf("TVOC:%d ppb\n", sgp.TVOC);
    M5.Lcd.printf("eCO2:%d ppm\n", sgp.eCO2);
    delay(500);
}
