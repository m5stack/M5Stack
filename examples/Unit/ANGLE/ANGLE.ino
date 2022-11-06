/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/gray
*
* Describe: Angle.  角度计
* Date: 2021/8/9
*******************************************************************************
  Description: Read the Angle of the angometer and convert it to digital display
  读取角度计的角度，并转换为数字量显示
*/

#include <M5Stack.h>
int sensorPin =
    36;  // set the input pin for the potentiometer.  设置角度计的输入引脚

int last_sensorValue =
    100;  // Stores the value last read by the sensor.  存储传感器上次读取到的值
int cur_sensorValue = 0;  // Stores the value currently read by the sensor.
                          // 存储传感器当前读取到的值

void setup() {
    M5.begin();        // Init M5Stack.  初始化M5Stack
    M5.Power.begin();  // Init power  初始化电源模块
    pinMode(
        sensorPin,
        INPUT);  // Sets the specified pin to input mode. 设置指定引脚为输入模式
    dacWrite(25, 0);
    M5.Lcd.setTextSize(2);  // Set the font size to 2.  设置字体大小为2
    M5.Lcd.print("the value of ANGLE: ");
}

void loop() {
    cur_sensorValue = analogRead(
        sensorPin);  // read the value from the sensor.  读取当前传感器的值
    M5.Lcd.setCursor(0, 25);  // Place the cursor at (0,25).  将光标固定在(0,25)
    if (abs(cur_sensorValue - last_sensorValue) > 10) {  // debaunce
        M5.Lcd.fillRect(0, 25, 100, 25, BLACK);
        M5.Lcd.print(cur_sensorValue);
        last_sensorValue = cur_sensorValue;
    }
    delay(50);
}
