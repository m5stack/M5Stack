/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit Weight
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5GFX@^0.2.3: https://github.com/m5stack/M5GFX
 * M5Unified@^0.2.2: https://github.com/m5stack/M5Unified
 * HX711: https://github.com/bogde/HX711
 */

#include <M5Unified.h>
#include <M5GFX.h>
#include "HX711.h"

M5GFX display;
M5Canvas canvas(&display);

// HX711 related pin Settings.  HX711 相关引脚设置
#define LOADCELL_DOUT_PIN 36
#define LOADCELL_SCK_PIN  26

HX711 scale;

void setup()
{
    M5.begin();        // Init M5Stack.  初始化M5Stack
    M5.Power.begin();  // Init power  初始化电源模块
    display.begin();
    canvas.setColorDepth(1);  // mono color
    canvas.createSprite(display.width(), display.height());
    canvas.setTextDatum(MC_DATUM);
    canvas.setPaletteColor(1, GREEN);

    canvas.drawString("Calibration sensor....", 160, 80);
    canvas.pushSprite(0, 0);
    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    // The scale value is the adc value corresponding to 1g
    scale.set_scale(61.2f);  // set scale
    scale.tare();            // auto set offset
}

void loop()
{
    canvas.fillSprite(BLACK);
    canvas.setTextSize(1);
    canvas.drawString("Connect the Weight Unit to PortB(G26,G36)", 160, 40);
    canvas.drawString("Click Btn A for Calibration", 160, 80);
    int weight = scale.get_units(5);
    canvas.setTextSize(3);
    canvas.drawString("Weight:" + String(weight) + "g", 160, 150);
    M5.update();
    if (M5.BtnA.wasPressed()) {
        scale.set_scale(61.2f);
        scale.tare();
        canvas.drawString("Calibration!", 160, 180);
    }
    canvas.pushSprite(0, 0);
}