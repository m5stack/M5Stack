/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit Button
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <M5Stack.h>

int last_value = 0;
int cur_value  = 0;

void setup()
{
    M5.begin();                   // Init M5Stack.  初始化M5Stack
    M5.Power.begin();             // Init power  初始化电源模块
    pinMode(36, INPUT);           // set pin mode to input.设置引脚模式为输入模式
    M5.Lcd.setTextColor(YELLOW);  // Set the font color to yellow.  设置字体颜色为黄色
    M5.Lcd.setTextSize(2);        // Setting the Font size.  设置字号大小
    M5.Lcd.setCursor(80, 0);      // Set the cursor position to (80,0).  将光标位置设置为(80,0)
    M5.Lcd.println("Button example");
    M5.Lcd.setTextColor(WHITE);
}

void loop()
{
    cur_value = digitalRead(36);  // read the value of BUTTON.  读取36号引脚的值
    M5.Lcd.setCursor(80, 25);
    M5.Lcd.print("Button");
    M5.Lcd.setCursor(0, 45);
    M5.Lcd.print("Value: ");
    M5.Lcd.setCursor(0, 85);
    M5.Lcd.print("State: ");
    if (cur_value != last_value) {
        M5.Lcd.fillRect(85, 45, 75, 85,
                        BLACK);  // Draw a black rectangle 75 by 85 at (85,45).
                                 // 在(85,45)处绘制宽75,高85的黑色矩形
        if (cur_value == 0) {
            M5.Lcd.setCursor(95, 45);
            M5.Lcd.print("0");  // display the status
            M5.Lcd.setCursor(95, 85);
            M5.Lcd.print("pre");
        } else {
            M5.Lcd.setCursor(95, 45);
            M5.Lcd.print("1");  // display the status
            M5.Lcd.setCursor(95, 85);
            M5.Lcd.print("rel");
        }
        last_value = cur_value;
    }
}
