/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/unit/sonic.io
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/unit/sonic.io
*
* describe: Ultrasonic.  超声波测距传感器 IO版本
* date：2021/11/17
*******************************************************************************
  Please connect to Port A,Display the distance measured by ultrasonic
  请连接端口B,显示超声波测量的距离
*/

#include <M5Stack.h>
#include <UNIT_SONIC.h>
#include <M5GFX.h>
M5GFX display;
M5Canvas canvas(&display);

SONIC_IO sensor;

void setup() {
  M5.begin(); //Init M5Stack.  初始化M5Stack
  // begin(uint8_t trig = 26, uint8_t echo = 36);
  sensor.begin();
  display.begin();
  canvas.setColorDepth(1);
  canvas.setFont(&fonts::Orbitron_Light_24);
  canvas.setTextSize(1);
  canvas.createSprite(display.width(), display.height());
  canvas.setPaletteColor(1, ORANGE);
}

int point = 0;
int last_point = 0;

void loop() {
  canvas.scroll(4);
  float Distance = sensor.getDistance();
  Serial.printf("Distance: %.2fmm\r\n",Distance);
  point = map((int)Distance,20, 4500, 10, 180);
  canvas.drawLine(0, point, 4, last_point, 1);
  last_point = point;
  canvas.fillRect(0,190,320,50,BLACK);
  canvas.drawString(String(Distance)+"mm", 80, 190);
  canvas.pushSprite(0,0);
}
