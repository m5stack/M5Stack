/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/gray
*
* describe: Weight.
* date：2021/8/20
*******************************************************************************
  Please connect to Port B,Use WEIGHT Unit to read the analog value returned by the pressure sensor,
  convert it into intuitive weight data and send it to M5Core,Press ButtonA to calibrate
  请连接端口B,使用WEIGHT Unit读取压力传感器返回的模拟值，将其转换为直观的重量数据，发送到M5Core，按下按钮A进行校准
*/

#include<M5Stack.h>
#include "HX711.h"

// HX711 related pin Settings.  HX711 相关引脚设置
#define LOADCELL_DOUT_PIN 36
#define LOADCELL_SCK_PIN 26

HX711 scale;
const long LOADCELL_OFFSET = 50682624;
const long LOADCELL_DIVIDER = 5895655;

void setup() {
  M5.begin(); //Init M5Stack.  初始化M5Stack
  M5.Power.begin(); //Init power  初始化电源模块
  M5.lcd.setTextSize(2);  //Set the text size to 2.  设置文字大小为2

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN); // Initialize library with data output pin, clock input pin and gain factor.  初始化库的数据输出引脚，时钟输入引脚和增益因子。
  scale.set_scale(LOADCELL_DIVIDER);  //set the SCALE value this value is used to convert the raw data to measure units.  设置SCALE值,该值用于将原始数据转换为度量单位
  scale.set_offset(LOADCELL_OFFSET);  //Set the tare weight.  设置皮重
  scale.set_scale(61.2f);  // this value is obtained by calibrating the scale with known weights; see the README for details.  这个值是通过校正已知权重的刻度而得到的
  scale.tare(); // reset the scale to 0.  将比例重置为0
  M5.Lcd.print("       Weight Unit\nConnect the Weight Unit to PortB");
  M5.Lcd.setCursor(20,210,1);
  M5.Lcd.print("Calibration");
}

void loop() {
  M5.update();
  if (M5.BtnA.wasPressed()) { //If button A is pressed.  如果按键A按下
    scale.set_offset(LOADCELL_OFFSET + scale.read());
    scale.set_scale(61.2f);
    scale.tare();
  }
  int weight = scale.get_units(5);
  M5.Lcd.fillRect(75, 80, 320, 80, BLACK);
  M5.Lcd.setCursor(75,80);
  M5.Lcd.printf("Weight:%1d g", weight);
}