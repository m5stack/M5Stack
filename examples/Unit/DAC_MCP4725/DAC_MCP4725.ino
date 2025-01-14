/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit DAC
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 * Adafruit_MCP4725: https://github.com/adafruit/Adafruit_MCP4725
 */

#include <Adafruit_MCP4725.h>
#include <M5Stack.h>

#define DAC_ADDR \
    (0x60)  // For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR
            // pin tied to VCC) For MCP4725A0 the address is 0x60 or 0x61 For
            // MCP4725A2 the address is 0x64 or 0x65

Adafruit_MCP4725 dac;

void setup(void)
{
    M5.begin(true, false, false);  // Init M5Stack.  初始化M5Stack
    M5.Power.begin();              // Init power  初始化电源模块
    M5.lcd.setTextSize(2);         // Set the text size to 2.  设置文字大小为2
    M5.Lcd.print("     DAC MCP4725 demo.");
    dac.begin(0x60);  // Setups the hardware address and checks the DAC was
                      // found.  设置硬件地址并检查是否找到DAC
    dac.setVoltage(2048, false);
}

void loop(void)
{
    M5.Lcd.setCursor(100, 60);
    M5.Lcd.print("1.2V");
    dac.setVoltage(1024, false);  // Set the voltage to 1.2V and retain the current
                                  // voltage output after power off or reset.
                                  // 设置电压为1.2v,关闭断电或复位后保留当前电压输出
    delay(1000);
    M5.Lcd.fillRect(100, 60, 120, 40, BLACK);  // Draw a black rectangle at (100,60) with a width of 120 and a
                                               // height of 40.  在(100,60)处绘制一个宽为120,高为40的黑色矩形
    M5.Lcd.print("2.4V");
    dac.setVoltage(2048, false);  // 2.4v
    delay(1000);
    M5.Lcd.fillRect(100, 60, 120, 40, BLACK);
}