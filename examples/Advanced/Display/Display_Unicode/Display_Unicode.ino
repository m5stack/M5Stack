/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/gray
*
* Describe: Display_Unicode.  Unicode显示
* Date: 2021/7/23
*******************************************************************************

-----PLEASE SEE THE README----
------请在使用前看README文件----*/
#define USE_M5_FONT_CREATOR
#include <M5Stack.h>

#include "CUF_24px.h"

void setup() {
    M5.begin();        // Init M5Stack.  初始化M5Stack
    M5.Power.begin();  // Init power  初始化电源模块
    M5.Lcd.setFreeFont(
        &unicode_24px);  // Set the GFX font to use.  设置要使用的GFX字体
    M5.Lcd.setTextDatum(TC_DATUM);  // Set text alignment to center-up
                                    // alignment. 设置文本对齐方式为居中向上对齐
}

void loop() {
    M5.Lcd.fillScreen(0);
    M5.Lcd.drawString("Hello world", 160, 60,
                      1);  // Hello world is displayed in font 1 on (1600.60).
    M5.Lcd.drawString("你好  世界", 160, 90,
                      1);  //在(160,60)处以字体1显示hello world
    M5.Lcd.drawString("Здравствуй  мир", 160, 120, 1);
    M5.Lcd.drawString("こんにちは  せかい", 160, 150, 1);
    delay(3000);

    M5.Lcd.fillScreen(
        0);  // Fill the screen with black (used to clear the screen) is
             // equivalent to clear ().  使屏幕充满黑色(用来清屏)等同于clear()
    M5.Lcd.setCursor(
        0, 30);  // Set the text cursor 0,30 position.  设置文本光标在(0,30)处
    M5.Lcd.printf(
        "☀☁☂☃☄★☆☇☈☉☊☋☌☍☎☏☐☑☒☓☔☕☖☗☘☙☚☛☜☝☞☟☠☡☢☣☤☥☦☧☨☩☪☫☬☭☮☯☸☹☺☻☼☽☾☿♀♁♂♃♄♅♆♇♈♉♊♋♌♍"
        "♎♏♐♑♒♓♔♕♖♗♘♙♚♛♜♝♞♟♠♡♢♣♤♥♦♧♨♩♪♫♬♭♮♯♰♱♲♳♴♵♶♷♸♹♺♻♼♽♾♿⚀⚁⚂⚃⚄⚅⚆⚇⚈⚉⚊⚋⚌⚍⚎⚏⚐⚑⚒⚓"
        "⚔⚕⚖⚗⚘⚙⚚⚛⚜⚝⚞⚟A⚠⚡⚢⚣⚤⚥⚦⚧⚨⚩⚪⚫⚬⚭⚮⚯B⚰⚱⚲⚳⚴⚵⚶⚷⚸⚹⚺⚻⚼⚽⚾⚿C⛀⛁⛂⛃⛄⛅⛆⛇⛈⛉⛊⛋⛌⛍⛎⛏D⛐⛑⛒⛓⛔⛕"
        "⛖⛗⛘⛙⛚⛛⛜⛝⛞⛟E⛠⛡⛢");
    delay(3000);

    M5.Lcd.fillScreen(0);
    M5.Lcd.setCursor(0, 30);
    M5.Lcd.printf(
        "⛣⛤⛥⛦⛧⛨⛩⛪⛫⛬⛭⛮⛯F⛰⛱⛲⛳⛴⛵⛶⛷⛸⛹⛺⛻⛼⛽⛾⛿✀✁✂✃✄✅✆✇✈✉✊✋✌✍✎✏✐✑✒✓✔✕✖✗✘✙✚✛✜✝✞"
        "✟"
        "✠"
        "✡"
        "✢"
        "✣"
        "✤"
        "✥"
        "✦✧✨✩✪✫✬✭✮✯✰✱✲✳✴✵✶✷✸✹✺✻✼✽✾✿❀❁❂❃❄❅❆❇❈❉❊❋❌❍❎❏❐❑❒❓❔❕❖❗❘❙❚❛❜❝❞❟❠❡❢❣❤❥❦❧❨❩❪❫"
        "❬❭❮❯❰❱❲❳❴❵❶❷❸❹❺❻❼❽❾❿➀➁➂➃➄➅➆➇➈➉➊➋➌➍➎➏➐➑➒➓➔➕➖➗➘➙➚➛➜➝➞➟A➠➡➢➣➤➥➦➧➨➩➪➫➬➭➮➯B"
        "➰➱➲➳➴➵➶➷➸➹➺➻➼➽➾➿");
    delay(3000);
}
