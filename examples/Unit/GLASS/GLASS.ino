/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/Glass%20Unit
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/Glass%20Unit
*
* Describe: Glass.
* Date: 2023/2/23
*******************************************************************************
Connect the Unit_Glass to Port A and press the buttons on the left/right side of
the Unit Glass to subtract/add to the counter.
将Unit_Glass连接到Port A,按Unit Glass左/右侧的按键来对计数器进行减/加.
*/
#include <M5Stack.h>
#include <UNIT_GLASS.h>

UNIT_GLASS Glass;
String string_buffer;

void setup() {
    M5.begin(1, 0, 1, 0);
    M5.Lcd.setTextSize(3);
    M5.Lcd.setTextColor(0x867D);
    M5.Lcd.printf(" Unit Glass Demo\n\n\n");
    M5.Lcd.printf(" Please connect\n\n your Unit-Glass\n\n");
    M5.Lcd.printf(" to PORT-A\n\n <--");

    /* Unit-Glass init */
    Glass.begin(&Wire, GLASS_ADDR, 21, 22, 100000UL);
    Glass.invert(0);
    Glass.color_invert(0);
    Glass.clear();

    /* Draw title on Unit-Glass */
    string_buffer = "M5STACK";
    Glass.set_string_buffer(string_buffer.c_str());
    Glass.draw_string(36, 0, 16, 1);
    delay(20);  // Ensure complete character display. 保证字符显示完全
    string_buffer = "2023-02-24";
    Glass.set_string_buffer(string_buffer.c_str());
    Glass.draw_string(24, 24, 16, 1);
    Glass.show();
}

void loop() {
    /* When key on Unit-Glass was pressed, update conut and print it out */
    static int count = 0;
    if (Glass.getKeyA() == 0) {
        Glass.setBuzzer(1500, 128);
        Glass.enable_buzz();
        delay(50);
        Glass.disable_buzz();
        count++;
    } else if (Glass.getKeyB() == 0) {
        Glass.setBuzzer(2300, 128);
        Glass.enable_buzz();
        delay(50);
        Glass.disable_buzz();
        count--;
    }

    string_buffer = "Count: ";
    string_buffer += count;
    string_buffer += "      ";
    Glass.set_string_buffer(string_buffer.c_str());
    Glass.draw_string(36, 48, 16, 1);
    Glass.show();
    delay(20);
}
