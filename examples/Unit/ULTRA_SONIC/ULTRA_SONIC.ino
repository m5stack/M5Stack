/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/gray
*
* describe: Ultrasonic.  超声波测距传感器
* date：2021/8/19
*******************************************************************************
  Please connect to Port A,Display the distance measured by ultrasonic
  请连接端口A,显示超声波测量的距离
*/

#include <M5Stack.h>

void setup() {
  M5.begin(); //Init M5Stack.  初始化M5Stack
  M5.Power.begin(); //Init power  初始化电源模块
  Wire.begin(21, 22);
  M5.Lcd.setCursor(105, 0, 4);  //Set the cursor at (105,0) and set the font to a 4 point font.  将光标设置在(105,0)处,且设置字体为4号字体
  M5.Lcd.print("Ultrasonic\nDistance:");
}

float readEUS()
{
  uint32_t data;
  Wire.beginTransmission(0x57); //Transfer data to 0x57.  将数据传输到0x57
  Wire.write(0x01);
  Wire.endTransmission(); //Stop data transmission with the Ultrasonic Unit.  停止与Ultrasonic Unit的数据传输
  delay(120);
  Wire.requestFrom(0x57,3); //Request 3 bytes from Ultrasonic Unit.  向Ultrasonic Unit请求3个字节。
  data  = Wire.read();data <<= 8;
  data |= Wire.read();data <<= 8;
  data |= Wire.read();
  return float(data) / 1000;
}

void loop() {
  static float newvalue = 0;
  newvalue = readEUS();
  if(( newvalue < 1500 )&&( newvalue > 20 )){
    M5.Lcd.setCursor(105,27);
    M5.Lcd.printf("%.2fmm",newvalue);
  }
  delay(100);
}
