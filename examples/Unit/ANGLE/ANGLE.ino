/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit Angle
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <M5Stack.h>

int sensorPin        = 36;   // set the input pin for the potentiometer.  设置角度计的输入引脚
int last_sensorValue = 100;  // Stores the value last read by the sensor.  存储传感器上次读取到的值
int cur_sensorValue  = 0;    // Stores the value currently read by the sensor.
                             // 存储传感器当前读取到的值

void setup()
{
    M5.begin();                 // Init M5Stack.  初始化M5Stack
    M5.Power.begin();           // Init power  初始化电源模块
    pinMode(sensorPin, INPUT);  // Sets the specified pin to input mode. 设置指定引脚为输入模式
    dacWrite(25, 0);
    M5.Lcd.setTextSize(2);  // Set the font size to 2.  设置字体大小为2
    M5.Lcd.print("the value of ANGLE: ");
}

void loop()
{
    cur_sensorValue = analogRead(sensorPin);             // read the value from the sensor.  读取当前传感器的值
    M5.Lcd.setCursor(0, 25);                             // Place the cursor at (0,25).  将光标固定在(0,25)
    if (abs(cur_sensorValue - last_sensorValue) > 10) {  // debaunce
        M5.Lcd.fillRect(0, 25, 100, 25, BLACK);
        M5.Lcd.print(cur_sensorValue);
        last_sensorValue = cur_sensorValue;
    }
    delay(50);
}
