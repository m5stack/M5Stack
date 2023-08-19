/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/gray
*
* Describe: Hall sensor.  霍尔传感器
* Date: 2021/7/26
*******************************************************************************
  The ESP32 MCU has build in Hall sensor.  ESP32 MCU内置霍尔传感器
  It is not very suscescible but if you bring a magnet close to the
  upper screen you will see the influence on the measurement.
  In this example we use some low pass filters to get rid of the noise.
*/
#include <M5Stack.h>

#define M5STACKFIRE_SPEAKER_PIN 34  // speaker DAC, only 8 Bit.  扬声器的管脚
#define HORIZONTAL_RESOLUTION   320
#define VERTICAL_RESOLUTION     240
#define POSITION_OFFSET_Y       20

uint16_t oldSignal[HORIZONTAL_RESOLUTION];
uint16_t adcBuffer[HORIZONTAL_RESOLUTION];

float ESP32_hallRead() {
    float value = 0;
    int count   = 400;
    // mean value filter.  数据低通滤波器
    for (int n = 0; n < count; n++) value += hallRead();
    return value / count * 10;
}

float HallOffset = 0;

void setup() {
    dacWrite(
        M5STACKFIRE_SPEAKER_PIN,
        0);  // make sure that the speaker is quite.  必须确保扬声器处于关闭状态
    M5.begin();             // Init M5Stack.  初始化M5Stack
    M5.Power.begin();       // Init power  初始化电源模块
    M5.Lcd.setTextSize(2);  // Set the font size to 2.  设置字体大小为2
    M5.Lcd.println("ESP32 Hall sensor:");  // Screen printout.  屏幕打印输出

    HallOffset =
        ESP32_hallRead();  // callibrate the output value to the magnetic field
                           // at start up.  在启动时将输出值校准到磁场
}

float LowPassFilteredValue = 0;
void showSignal() {
    int n;

    int oldx;
    int oldy;
    int oldSig;
    int x, y;

    for (n = 0; n < HORIZONTAL_RESOLUTION;
         n++)  //在水平分辨率内,每个像素点根据计算得到的磁力大小绘制
    {  // Within the horizontal resolution, each pixel is drawn according to the
       // calculated magnetic force
        x           = n;
        float value = ESP32_hallRead() - HallOffset;
        LowPassFilteredValue += (value - LowPassFilteredValue) * 0.05;
        M5.Lcd.setCursor(220, 1);
        M5.Lcd.print((int)LowPassFilteredValue);
        M5.Lcd.print("    ");
        y = map(value, -127, 127, VERTICAL_RESOLUTION, POSITION_OFFSET_Y);

        if (n > 0) {
            // delete old line element.  删除上次画的线
            M5.Lcd.drawLine(oldx, oldSig, x, oldSignal[n], BLACK);

            // draw new line element.  画新的线
            if (n < HORIZONTAL_RESOLUTION -
                        1)  // don't draw last element because it would generate
                            // artifacts.  不能绘制最后一个像素
            {
                M5.Lcd.drawLine(oldx, oldy, x, y, GREEN);
            }
        }
        oldx         = x;
        oldy         = y;
        oldSig       = oldSignal[n];
        oldSignal[n] = y;
    }
}

void loop(void) {
    showSignal();
}
