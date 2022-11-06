/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/gray
*
* Describe: Fan.  风扇
* Date: 2021/8/16
*******************************************************************************
  Please connect to Port B(26), Adjust the speed of FAN Unit through PWM.
  请连接端口B(26),通过PWM调节风扇单元的转速。
*/
#include <M5Stack.h>

#define motor_pin 26

int freq       = 10000;
int ledChannel = 0;
int resolution = 10;
void setup() {
    M5.begin();             // Init M5Stack.  初始化M5Stack
    M5.Power.begin();       // Init power  初始化电源模块
    M5.lcd.setTextSize(2);  // Set the text size to 2.  设置文字大小为2
    M5.Lcd.setCursor(140,
                     10);  // Set the cursor at (140,10). 将光标设置在(140,10)处
    M5.Lcd.println("Fan");
    ledcSetup(
        ledChannel, freq,
        resolution);  // Sets the frequency and number of counts corresponding
                      // to the channel.  设置通道对应的频率和计数位数
    ledcAttachPin(
        motor_pin,
        ledChannel);  // Binds the specified channel to the specified I/O port
                      // for output.  将指定通道绑定到指定 IO 口上以实现输出
}

void loop() {
    ledcWrite(ledChannel, 512);  // Output PWM.  输出PWM
    delay(1000);
    ledcWrite(ledChannel, 0);
    delay(1000);
}
