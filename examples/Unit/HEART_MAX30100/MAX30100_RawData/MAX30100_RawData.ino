/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit Heart
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 * Arduino-MAX30100: https://github.com/oxullo/Arduino-MAX30100
 */

#include <M5Stack.h>
#include "MAX30100.h"

#define SAMPLING_RATE   (MAX30100_SAMPRATE_100HZ)
#define IR_LED_CURRENT  (MAX30100_LED_CURR_24MA)
#define RED_LED_CURRENT (MAX30100_LED_CURR_27_1MA)
#define PULSE_WIDTH     (MAX30100_SPC_PW_1600US_16BITS)
#define HIGHRES_MODE    (true)

MAX30100 sensor;  // Instantiate a MAX30100 sensor class.  实例化一个MAX30100传感器类

void setup()
{
    M5.begin();        // Init M5Stack.  初始化M5Stack
    M5.Power.begin();  // Init power  初始化电源模块
    Serial.print("Initializing MAX30100..");

    while (!sensor.begin()) {  // Initialize the sensor.  初始化传感器
        M5.Lcd.setTextFont(4);
        M5.Lcd.setCursor(50, 100, 4);
        M5.Lcd.println("Sensor not found");
        delay(1000);
    }
    M5.Lcd.fillScreen(BLACK);
    // Set up the wanted parameters.  设置所需的参数
    sensor.setMode(MAX30100_MODE_SPO2_HR);
    sensor.setLedsCurrent(IR_LED_CURRENT, RED_LED_CURRENT);
    sensor.setLedsPulseWidth(PULSE_WIDTH);
    sensor.setSamplingRate(SAMPLING_RATE);
    sensor.setHighresModeEnabled(HIGHRES_MODE);
}

void loop()
{
    uint16_t ir, red;
    sensor.update();                          // 更新传感器读取到的数据
    while (sensor.getRawValues(&ir, &red)) {  // 如果获取到数据
        M5.Lcd.setTextFont(4);
        M5.Lcd.setCursor(100, 100, 4);
        M5.Lcd.printf("IR:%d               ", ir);
        M5.Lcd.setCursor(100, 130, 4);
        M5.Lcd.printf("RED:%d              ", red);
    }
}
