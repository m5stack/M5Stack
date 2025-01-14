/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit Vibrator
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <M5Stack.h>

#define motor_pin 26
int freq       = 10000;
int ledChannel = 0;
int resolution = 10;
void setup()
{
    M5.begin();                 // Init M5Stack.  初始化M5Stack
    M5.Power.begin();           // Init power  初始化电源模块
    M5.lcd.setTextSize(2);      // Set the text size to 2.  设置文字大小为2
    M5.Lcd.setCursor(110, 10);  // Set the cursor at (110, 10).  将光标设置在(110, 10)处
    M5.Lcd.println("Vibrator");
    ledcSetup(ledChannel, freq,
              resolution);  // Sets the frequency and number of counts corresponding
                            // to the channel.  设置通道对应的频率和计数位数
    ledcAttachPin(motor_pin,
                  ledChannel);  // Binds the specified channel to the specified I/O port
                                // for output.  将指定通道绑定到指定 IO 口上以实现输出
}

void loop()
{
    ledcWrite(ledChannel, 512);  // Output PWM.  输出PWM
    delay(1000);
    ledcWrite(ledChannel, 0);
    delay(1000);
}
