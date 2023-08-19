/*
*******************************************************************************
*Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/gray
*
* Describe: Power example.  电源管理示例
* Date: 2021/7/21
*******************************************************************************
*/
#include <M5Stack.h>

/* After M5Core is started or reset
the program in the setUp () function will be run, and this part will only be run
once. 在 M5Core
启动或者复位后，即会开始执行setup()函数中的程序，该部分只会执行一次。 */
void setup() {
    M5.begin();        // Init M5Core.  初始化 M5Core
    M5.Power.begin();  // Init Power module.  初始化电源模块
    M5.Power.setWakeupButton(BUTTON_A_PIN);  // Set the screen wake-up button to
                                             // A.  将屏幕唤醒的按键设置为A
    M5.Lcd.setBrightness(
        200);  // Set the screen brightness to 200 nits.  设置屏幕亮度为200尼特
    M5.lcd.setTextSize(2);  // Set the text size to 2.  设置文字大小为2
}

/* After the program in setup() runs, it runs the program in loop()
  The loop() function is an infinite loop in which the program runs repeatedly
  在setup()函数中的程序执行完后，会接着执行loop()函数中的程序
  loop()函数是一个死循环，其中的程序会不断的重复运行 */
void loop() {
    M5.update();  // Read the press state of the key.  读取按键 A, B, C 的状态
    bool c =
        M5.Power
            .isResetbyPowerSW();  // Determine if M5Core is started when powered
                                  // on.  确定M5Core是否在接通电源时启动
    bool d =
        M5.Power.isResetbyDeepsleep();  // Determine if M5Core starts after deep
                                        // sleep. 确定M5Core是否在深度睡眠后启动

    M5.Lcd.println(
        "<Sleep test>");  // The screen prints the formatted string and wraps
                          // the line.  输出格式化字符串并换行
    M5.Lcd.printf("power-on triggered at:%s%s\n\n", (c) ? ("POWER-SW") : (""),
                  (d) ? ("DeepSleep-end") : (""));

    M5.Lcd.printf("Go lightSleep (5s or press buttonA wake up)\n");
    delay(5000);  // delay 5000ms.  延迟5000ms
    /*Restart after 10 seconds of light sleep and continue from the next line
    Calling this function power button will disable the power button to restore
    Please call M5.Power.setPowerBoostKeepOn(false)*/
    /*轻度睡眠10秒后重新启动，程序从下一行继续执行
    调用此函数将禁用电源按钮，要恢复电源按钮
    请调用M5.Power.setPowerBoostKeepOn(false)*/
    M5.Power.lightSleep(SLEEP_SEC(10));
    M5.Lcd.printf("Go lightSleep (press buttonA wake up)\n");
    delay(5000);
    M5.Power.lightSleep(0);

    M5.Lcd.printf("resume.\n\nGo deepSleep (press buttonA wake up) ");
    delay(5000);
    /*After waking up from deep sleep for 0 seconds, the CPU will restart and
    the program will be executed from the beginning
    Calling this function will disable the power button to restore the power
    button Please call M5.Power.setPowerBoostKeepOn(false)*/
    /*深度睡眠0秒后唤醒,CPU将重新启动，程序将从头开始执行
    调用此函数将禁用电源按钮，要恢复电源按钮
    请调用M5.Power.setPowerBoostKeepOn(false)*/
    M5.Power.deepSleep(0);
}
