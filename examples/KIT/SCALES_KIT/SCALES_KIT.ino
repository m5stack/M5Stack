/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/app/scales_kit
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/app/scales_kit
*
* Describe: SCALES KIT WEIGHT UNIT EXAMPLE.
* Date: 2022/02/23
*******************************************************************************
  Connect WEIGHT UNIT to port B (G26/36), calibration instructions: press button
A to remove the tare weight when there is no load, press button B, switch the
standard weight value left and right and put down the corresponding weight,
confirm for calibration. 将WEIGHT UNIT连接至端口B（G26/36）， 校准说明:
无负重情况下按下按键A去处皮重，
按下按键B，左右切换标准重量值并放下对应重量砝码，comfirm进行校准。 Libraries:
  - [HX711](https://github.com/bogde/HX711)

*/

#include <M5GFX.h>
#include <M5Stack.h>

#include "HX711.h"

M5GFX display;
M5Canvas canvas(&display);

// HX711 related pin Settings.  HX711 相关引脚设置
#define LOADCELL_DOUT_PIN 36
#define LOADCELL_SCK_PIN  26

HX711 scale;

void setup() {
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

    scale.set_gain();
    // The scale value is the adc value corresponding to 1g
    scale.set_scale(27.61f);  // set scale
    scale.tare();             // auto set offset
}

char info[100];

void loop() {
    canvas.fillSprite(BLACK);
    canvas.setTextSize(1);
    canvas.drawString("Connect the Weight Unit to PortB(G26,G36)", 160, 40);
    canvas.drawString("Click Btn A for Tare deduction", 160, 55);
    canvas.drawString("Click Btn B Switch to Calibration mode", 160, 70);
    float weight = scale.get_units(10) / 1000.0;
    // float weight = scale.get_units(10) / 1.0;
    canvas.setTextSize(3);
    if (weight >= 0) {
        Serial.printf("Weight: %.2f", weight);
        sprintf(info, "Weight: %.2f", weight);
        canvas.drawString(String(info) + "kg", 160, 150);
        // canvas.drawString(String(info) + "g", 160, 150);
    } else {
        canvas.drawString("Weight: 0kg", 160, 150);
        // canvas.drawString("Weight: 0g", 160, 150);
    }
    M5.update();
    if (M5.BtnA.wasPressed()) {
        scale.tare();
        canvas.drawString("0g Calibration!", 160, 180);
    }
    if (M5.BtnB.wasPressed()) {
        long kg = 5;
        while (1) {
            M5.update();
            canvas.fillSprite(BLACK);
            canvas.setTextSize(1);
            canvas.drawString("Connect the Weight Unit to PortB(G26,G36)", 160,
                              40);
            canvas.drawString("Click Btn A/C to change kg value", 160, 55);
            canvas.drawString("Click Btn B Calibration Comfirm", 160, 70);
            canvas.setTextSize(3);
            canvas.drawString("Calibration:" + String(kg) + "kg", 160, 150);
            canvas.drawString("comfirm", 160, 200);
            canvas.fillTriangle(40, 200, 60, 220, 60, 180, 1);
            canvas.fillTriangle(280, 200, 260, 220, 260, 180, 1);
            canvas.pushSprite(0, 0);
            if (M5.BtnA.isPressed()) {
                kg--;
            }
            if (M5.BtnC.isPressed()) {
                kg++;
            }
            if (M5.BtnB.wasPressed()) {
                break;
            }
            delay(10);
        }
        long kg_adc = scale.read_average(20);
        kg_adc      = kg_adc - scale.get_offset();
        scale.set_scale(kg_adc / (kg * 1000.0));
        // canvas.drawString(String(kg) + "kg Calibration: " + String(kg_adc /
        // (kg * 1000.0)), 160, 180);
        canvas.drawString("Set Scale: " + String(kg_adc / (kg * 1000.0)), 160,
                          180);
        canvas.pushSprite(0, 0);
        delay(1000);
    }
    canvas.pushSprite(0, 0);
}