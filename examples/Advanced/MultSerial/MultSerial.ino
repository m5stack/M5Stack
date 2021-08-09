/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/gray
*
* describe：MultSerial.  多串口
* date：2021/8/5
******************************************************************************
*/
#include <M5Stack.h>

void setup() {
  M5.begin(); //Init M5Core.  初始化 M5Core
  // Serial2.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
  Serial2.begin(115200, SERIAL_8N1, 16, 17);  //Init serial port 2.  初始化串口2
}

void loop() {
 if(Serial.available()) { //If the serial port reads data.  如果串口读到数据
   int ch = Serial.read();    // Copy the data read from the serial port to the CH.  把串口读取到的数据复制给ch
   Serial2.write(ch); //Serial port 2 Outputs the CH content.  串口2输出ch的内容
   M5.Lcd.printf("Serial:%d\n",ch); //The screen prints the data received by serial port 2.  屏幕打印串口2收到的数据
 }

 if(Serial2.available()) {
   int ch = Serial2.read();
   Serial.write(ch);
   M5.Lcd.printf("Serial2:%d\n",ch);
 }
}
