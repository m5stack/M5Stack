/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/gray
*
* Describe: Button example.  按键示例
* Date: 2022/7/1
*******************************************************************************
    Press button A/B/C to display the corresponding output on the screen
    按下按键A/B/C，在屏幕上显示相应输出
*/
#include <M5Stack.h>
/* After M5Core is started or reset
  the program in the setUp () function will be run, and this part will only be
  run once. 在 M5Core
  启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。 */
void setup() {
    M5.begin();        // Init M5Core.  初始化 M5Core
    M5.Power.begin();  // Init Power module.  初始化电源模块
    M5.Lcd.setTextColor(
        YELLOW);  // Set the font color to yellow.  设置字体颜色为黄色
    M5.Lcd.setTextSize(2);     // Set the font size.  设置字体大小为2
    M5.Lcd.setCursor(65, 10);  // Move the cursor position to (65, 10).
                               // 移动光标位置到 (65, 10) 处
    M5.Lcd.println(
        "Button example");  // The screen prints the formatted string and wraps
                            // the line.  输出格式化字符串并换行
    M5.Lcd.setCursor(3, 35);
    M5.Lcd.println("Press button B for 700ms");
    M5.Lcd.println("to clear screen.");
    M5.Lcd.setTextColor(RED);
}

/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop() {
    M5.update();  // Read the press state of the key.  读取按键 A, B, C 的状态
    if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000, 200)) {
        M5.Lcd.print('A');
    } else if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000, 200)) {
        M5.Lcd.print('B');
    } else if (M5.BtnC.wasReleased() || M5.BtnC.pressedFor(1000, 200)) {
        M5.Lcd.print('C');
    } else if (M5.BtnB.wasReleasefor(700)) {
        M5.Lcd.clear(WHITE);  // Clear the screen and set white to the
                              // background color.  清空屏幕并将白色设置为底色
        M5.Lcd.setCursor(0, 0);
    }
}
