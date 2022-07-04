/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/gray
*
* Describe: I2C Scanner.  I2C探测
* Date: 2021/7/26
*******************************************************************************
*/
/*
This program scans the addresses 1-127 continuosly and shows the devices found
on the TFT. The M5Stack has three internal I2C devices at address 0x68, 0x75 and
0x10. 该程序连续扫描地址 1-127 并显示在 TFT 上发现的设备。 M5Stack 在地址0x10,
0x68, 0x75 处有三个内部 I2C 设备。
*/
#include <M5Stack.h>

void setup() {
    M5.begin();        // Init M5Stack(Initial I2C is also included).
                       // 初始化M5Stack(初始化I2C也包含在内)
    M5.Power.begin();  // Init power  初始化电源模块
    M5.Lcd.setTextColor(
        YELLOW);  // Set the font color to yellow.  设置字体颜色为黄色
    M5.Lcd.setTextSize(2);  // Set the font size to 2.  设置字体大小为2
    M5.Lcd.println("M5StackFire I2C Tester");  // Print a string on the screen.
                                               // 在屏幕上打印字符串
    delay(3000);
    M5.Lcd.fillScreen(
        BLACK);  // Make the screen full of black (equivalent to clear() to
                 // clear the screen).  使屏幕充满黑色(等效clear()清屏)
}

int textColor = YELLOW;

void loop() {
    int address;
    int error;
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.println("scanning Address [HEX]");
    for (address = 1; address < 127; address++) {
        Wire.beginTransmission(
            address);  // Data transmission to the specified device address
                       // starts.   开始向指定的设备地址进行传输数据
        error = Wire.endTransmission(); /*Stop data transmission with the slave.
                  停止与从机的数据传输 0: success.  成功 1: The amount of data
                  exceeds the transmission buffer capacity limit.
                  数据量超过传送缓存容纳限制 return value:              2:
                  Received NACK when sending address.  传送地址时收到 NACK 3:
                  Received NACK when transmitting data.  传送数据时收到 NACK
                                             4: Other errors.  其它错误 */
        if (error == 0) {
            M5.Lcd.print(address, HEX);
            M5.Lcd.print(" ");
        } else
            M5.Lcd.print(".");

        delay(10);
    }

    if (textColor == YELLOW) {
        textColor = CYAN;
    } else
        textColor = YELLOW;
    M5.Lcd.setTextColor(textColor,
                        BLACK);  // Set the foreground color of the text to
                                 // textColor and the background color to BLACK.
                                 // 设置文字的前景色为textColor背景色为BLACK
}
