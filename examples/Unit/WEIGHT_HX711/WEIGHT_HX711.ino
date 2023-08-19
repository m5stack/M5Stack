/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/gray
*
* Describe: Weight.
* Date: 2021/8/20
*******************************************************************************
  Please connect to Port B,Use WEIGHT Unit to read the analog value returned by
the pressure sensor, convert it into intuitive weight data and send it to
M5Core,Press ButtonA to calibrate 请连接端口B,使用WEIGHT
Unit读取压力传感器返回的模拟值，将其转换为直观的重量数据，发送到M5Core，按下按钮A进行校准
*/

#include <M5Stack.h>
#include <M5GFX.h>

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
    // The scale value is the adc value corresponding to 1g
    scale.set_scale(61.2f);  // set scale
    scale.tare();            // auto set offset
}

void loop() {
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