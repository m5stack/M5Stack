/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/oled
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/oled
*
* Describe: OLED.  显示屏
* Date: 2021/8/31
*******************************************************************************
  Please connect to Port A(22、21),Displays a string on the screen.
  请连接端口A(22、21),在屏幕上显示字符串。
*/
#include <M5UnitOLED.h>

M5UnitOLED display;

M5Canvas canvas(&display);

static constexpr char text[] =
    "Hello world ! こんにちは世界！ this is long long string sample. "
    "寿限無、寿限無、五劫の擦り切れ、海砂利水魚の、水行末・雲来末・風来末、喰う"
    "寝る処に住む処、藪ら柑子の藪柑子、パイポ・パイポ・パイポのシューリンガン、"
    "シューリンガンのグーリンダイ、グーリンダイのポンポコピーのポンポコナの、長"
    "久命の長助";
static constexpr size_t textlen = sizeof(text) / sizeof(text[0]);
int textpos                     = 0;
int scrollstep                  = 2;

void setup(void) {
    display.init();           // Initialize the display.  初始化显示屏
    display.setRotation(2);   // Rotating display.  旋转显示屏
    canvas.setColorDepth(1);  // Set the color depth.  设置色深
    canvas.setFont(&fonts::lgfxJapanMinchoP_32);  // Set the font.  设置字体
    canvas.setTextWrap(false);
    canvas.setTextSize(2);  // Set the font size.  设置字号
    canvas.createSprite(
        display.width() + 64,
        72);  // Create a canvas with a wide display width of +64 and a height
              // of 72.  创建一块宽显示屏宽度+64,高72的画布
}

void loop(void) {
    int32_t cursor_x = canvas.getCursorX() - scrollstep;
    if (cursor_x <= 0) {
        textpos  = 0;
        cursor_x = display.width();
    }

    canvas.setCursor(cursor_x, 0);  // Set the cursor position.  设置光标的位置
    canvas.scroll(-scrollstep, 0);  // Set the rolling.  设置滚动
    while (textpos < textlen && cursor_x <= display.width()) {
        canvas.print(text[textpos++]);
        cursor_x = canvas.getCursorX();
    }
    display.waitDisplay();
    canvas.pushSprite(
        &display, 0,
        (display.height() - canvas.height()) >>
            1);  // Displays the contents of the canvas.  显示画布上的内容
}