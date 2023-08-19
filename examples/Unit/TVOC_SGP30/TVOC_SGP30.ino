/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/tvoc
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/tvoc
*
* Describe: TVOC/eCO2.
* Date: 2021/8/26
*******************************************************************************
  Description: The screen will display TVOC and CO2.  屏幕将显示TVOC和CO2。
  Note: SGP30 needs 15 seconds to initialize calibration after power on.
SGP30开机后需要15秒进行初始校准。
*/

#include <M5Stack.h>

#include "Adafruit_SGP30.h"

Adafruit_SGP30 sgp;
long last_millis = 0;

void setup() {
    M5.begin(true, false, true, true);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(80, 0);
    M5.Lcd.println("TVOC TEST");
    if (!sgp.begin()) {  // Init the sensor. 初始化传感器
        M5.Lcd.println("Sensor not found");
        while (1)
            ;
    }
    M5.Lcd.setCursor(0, 80);
    M5.Lcd.println("\nInitialization...");
}

void loop() {
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
