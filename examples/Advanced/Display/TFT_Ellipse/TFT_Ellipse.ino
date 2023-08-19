/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/gray
*
* Describe: Ellipse drawing.  椭圆绘制
* Date: 2021/7/26
*******************************************************************************/

#include <M5Stack.h>

void setup(void) {
    M5.begin();        // Init M5Stack.  初始化M5Stack
    M5.Power.begin();  // Init power  初始化电源模块
}

void loop() {
    // Draw some random ellipses.  绘制一些随机形状的椭圆
    for (int i = 0; i < 40; i++) {
        int rx = random(60);
        int ry = random(60);
        int x  = rx + random(320 - rx - rx);
        int y  = ry + random(240 - ry - ry);
        M5.Lcd.fillEllipse(
            x, y, rx, ry,
            random(
                0xFFFF));  // At (x, y), draw a filled ellipse with random width
                           // and height of rx, ry with random color random
                           // (0xFFFF).
                           // 在(x,y)处以随机颜色random(0xFFFF),绘制一个随机宽高为rx,ry的填充椭圆
    }

    delay(2000);
    M5.Lcd.clear();  //清空屏幕

    for (int i = 0; i < 40; i++) {
        int rx = random(60);
        int ry = random(60);
        int x  = rx + random(320 - rx - rx);
        int y  = ry + random(240 - ry - ry);
        M5.Lcd.drawEllipse(
            x, y, rx, ry,
            random(
                0xFFFF));  // At (x, y), draw an elliptical wire frame with a
                           // random width and height of rx, ry in a random
                           // color (0xFFFF).
                           // 在(x,y)处以随机颜色random(0xFFFF),绘制一个随机宽高为rx,ry的椭圆线框
    }

    delay(2000);
}
