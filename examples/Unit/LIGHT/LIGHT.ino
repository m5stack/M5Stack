/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/gray
*
* Describe: Light.  环境光传感器
* Date: 2022/2/13
*******************************************************************************
  Please connect to Port B,Use the Light Unit screen to display the current
ambient lighting value 请连接端口 B ,使用Light Unit 屏幕显示当前环境光照值。
*/

#include <M5Stack.h>

TFT_eSprite img = TFT_eSprite(&M5.Lcd);

void setup() {
    M5.begin();        // Init M5Stack.  初始化M5Stack
    M5.Power.begin();  // Init power  初始化电源模块
    img.setColorDepth(8);
    img.createSprite(320, 240);  //创建一块320x240的画布
    img.setTextSize(2);
    pinMode(26, INPUT);  // Set pin 26 as input mode.  设置引脚26为输入模式
}

void loop() {
    static uint16_t digitalRead_value = 0, analogRead_value = 0;
    analogRead_value = analogRead(36);  // Store the analog quantity read from
                                        // pin 36. 将36号引脚读取到的模拟量存储
    digitalRead_value = digitalRead(
        26);  // Store the number read from pin 26. 将26号引脚读取到的数字量存储
    img.fillRect(0, 0, 320, 240, 0x00);
    img.drawString("UNIT_LIGHT EXAMPLE", 40, 0);
    img.setCursor(90, 30);
    img.printf("Analog:%d\n", analogRead_value);
    img.setCursor(90, 50);
    img.printf("Digital:%d\n", digitalRead_value);
    img.pushSprite(0, 0);  //把画布推送到屏幕(0,0)处
    delay(10);
}
