/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/gray
*
* describe：Character library.  字库
* date：2021/7/28
*******************************************************************************

-----PLEASE SEE THE README----
------请在使用前看README文件----*/

#include <M5Stack.h>
#include "display_ch.h"
#include "str.h"

DisplayCh displaych;
void setup() {
  M5.begin(); //Init M5Stack.  初始化M5Stack
  M5.Power.begin(); //Init power  初始化电源模块
  displaych.loadHzk16();  //Load the Chinese character library (be sure to load before using the Chinese character library).  加载汉字库(务必在使用汉字库前加载)
  displaych.setTextColor(WHITE, BLACK); //Set the text color to white and the text background color to black (mandatory).  设置文字颜色为白色,文字背景颜色为黑色(必加)
  // Set text with red highlight color
  displaych.setHighlightColor(RED); //Set the text highlight color to red.  设置文字高亮颜色为红色
  displaych.setTextSize(1); //Set text size to 1.  设置字号大小为1 Set text size to 1(必加)
}

void loop() {
  displaych.setCursor(0,0,1); //Set the cursor at (0,0) and the size to 1(mandatory).  将光标设置在(0,0)处,并设置字号为1(必加)
  displaych.writeHzk(AscStr); //Display the contents of AscStr here (which can be changed in str.h).  在此处显示AscStr中的内容(可在str.h中更改)
  delay(1000);  //delay 1000ms.  延迟1000ms
  displaych.setCursor(0,45);
  displaych.writeHzk(GbkStr); //Display the contents of GbkStr here (which can be changed in str.h).  在此处显示GbkStr中的内容(可在str.h中更改)
  delay(1000);
  //Highlight the text.  高亮显示文本
  displaych.highlight(true);  //Turn on highlight.  开启高亮显示
  displaych.setCursor(0,65);
  displaych.writeHzk(GbkStr);
  delay(1000);
  displaych.fillScreen(BLACK);  //Fill the screen with black color, equivalent to empty the screen.  填充屏幕颜色为黑色,等效于清空屏幕
  displaych.highlight(false); //Turn off highlight.  关闭高亮显示
  delay(500);
}