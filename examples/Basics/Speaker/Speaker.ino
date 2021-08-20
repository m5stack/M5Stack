/*
*******************************************************************************
*Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/gray
*
* describe：Speaker example.  喇叭示例
* date：2021/7/21
*******************************************************************************
*/
#include <M5Stack.h>

// Set the frequency of the speaker
//喇叭相关频率设置
#define NOTE_D0 -1
#define NOTE_D1 294
#define NOTE_D2 330
#define NOTE_D3 350
#define NOTE_D4 393
#define NOTE_D5 441
#define NOTE_D6 495
#define NOTE_D7 556

#define NOTE_DL1 147
#define NOTE_DL2 165
#define NOTE_DL3 175
#define NOTE_DL4 196
#define NOTE_DL5 221
#define NOTE_DL6 248
#define NOTE_DL7 278

#define NOTE_DH1 589
#define NOTE_DH2 661
#define NOTE_DH3 700
#define NOTE_DH4 786
#define NOTE_DH5 882
#define NOTE_DH6 990
#define NOTE_DH7 112

/* After M5Core is started or reset
the program in the setUp () function will be run, and this part will only be run once.
在 M5Core 启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。 */
void setup() {
  M5.begin(); //Init M5Core.  初始化 M5Core
  M5.Power.begin(); //Init Power module.  初始化电源
  M5.lcd.setTextSize(2);  //Set the text size to 2.  设置文字大小为2
  M5.Lcd.println("M5Stack Speaker test"); //Screen printingformatted string.  输出格式化字符串
}

/* After the program in setup() runs, it runs the program in loop()
The loop() function is an infinite loop in which the program runs repeatedly
在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop() {
  M5.update();   //Read the press state of the key.  读取按键 A, B, C 的状态

  if(M5.BtnA.wasPressed()) {  //Check if the key is pressed.  如果按键A被按下
    M5.Lcd.println("A wasPressed");
    M5.Speaker.tone(NOTE_DH2, 200); //Set the speaker to ring at 661Hz for 200ms.  设定喇叭以661Hz频率响200ms
  }else if(M5.BtnB.wasPressed()){
    M5.Lcd.println("B wasPressed");
    M5.Speaker.tone(NOTE_DH7); //Set the horn to continuously sound at 112Hz.  设定喇叭以112Hz频率持续响
  }else if(M5.BtnC.wasPressed()){
    M5.Lcd.println("C wasPressed");
    M5.Speaker.end(); // Turn off the speaker.  关闭喇叭
  }
}
