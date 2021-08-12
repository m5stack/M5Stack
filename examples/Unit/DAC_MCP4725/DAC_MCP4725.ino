/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/gray
*
* describe：DAC_MCP4725.  数模转换
* date：2021/8/11
*******************************************************************************
  Please connect to Port A(22、21),Use DAC Unit DAC to output 0 ~ 3.3V voltage with an accuracy of 12 bits.
  请连接端口A(22、21),使用DAC Unit DAC输出12位精度的0 ~ 3.3V电压。
*/

#include <M5Stack.h>
#include <Adafruit_MCP4725.h>
#define DAC_ADDR
Adafruit_MCP4725 dac;

void setup(void) {
    M5.begin(true, false, false);
    M5.Power.begin();
    Serial.begin(115200);
    Serial.println("Hello!");
    M5.Lcd.setTextFont(4);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawString("DAC MCP4725 demo.",25, 100, 4);
    // For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
    // For MCP4725A0 the address is 0x60 or 0x61
    // For MCP4725A2 the address is 0x64 or 0x65
    dac.begin(0x60);
        
    Serial.println("Generating a triangle wave");
    dac.setVoltage(2048, false);

}

void loop(void) {
    // 12bit value , false mean not write EEPROM
    dac.setVoltage(1024, false);
    delay(1000);
    dac.setVoltage(2048, false);
    delay(1000);
}