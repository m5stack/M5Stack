/*
*******************************************************************************
*Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/gray
*
* describe：Power Management Example  电源管理
* date：2021/7/21
*******************************************************************************
*/
#include <M5Stack.h>

/* After M5Core is started or reset
the program in the setUp () function will be run, and this part will only be run once.
在 M5Core 启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。 */
void setup(){
  M5.begin(); //Init M5Core.  初始化 M5Core
  M5.Power.begin(); //Init Power module.  初始化电源
  M5.Lcd.setTextSize(2);  //Set the font size.  设置字体大小
  M5.Lcd.print("After 5 seconds, the program entered light sleep\n\n");  //Screen printingformatted string.  输出格式化字符串
  delay(5000);
  M5.Power.lightSleep(SLEEP_SEC(5));  //Sleep for 5 seconds, then continue the program.  睡眠5秒,结束后程序继续往下进行
  M5.Lcd.print("press ButtonA: shutdown,  use power button to turn  back on");  //Screen printingformatted string.  输出格式化字符串
}

/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop() {
  M5.update();  //Read the press state of the key.  读取按键 A, B, C 的状态
  if(M5.BtnA.wasPressed()) {  //Check if the key is pressed.  如果按键A被按下
    M5.Power.powerOFF();  //Turn off power.  关闭电源
  }
}
