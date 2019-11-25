/*
  if use PlatformIO, need add "board_build.partitions = no_ota.csv" to config
  Set tools->partition scheme->'Large APP'
  Pls modify <m5stack path>/src/utility/In_eSPI_Setup.h, At the end of the file add "#define USE_M5_FONT_CREATOR"
  if want get customize unicode, try use https://github.com/m5stack/FontCreator, create font file
  
  if add "#define USE_M5_FONT_CREATOR" and want to use standard gfx font, need modify font file
    #ifdef USE_M5_FONT_CREATOR
    0, 0, 
    #endif
  like:
    const GFXfont FreeMono12pt7b PROGMEM = {
      (uint8_t  *)FreeMono12pt7bBitmaps,
      (GFXglyph *)FreeMono12pt7bGlyphs,
      0x20, 0x7E, 24,
      #ifdef USE_M5_FONT_CREATOR
      0, 0, 
      #endif
    };
*/
#include <M5Stack.h>
#include "CUF_24px.h"

void setup()
{
    M5.begin();

    M5.Lcd.setTextColor(TFT_WHITE);
    M5.Lcd.setFreeFont(&CUF_24px);


    M5.Lcd.setTextDatum(TC_DATUM);
}

void loop()
{
    M5.Lcd.fillScreen(0);
    M5.Lcd.drawString("Hello world", 160, 60, 1);
    M5.Lcd.drawString("你好  世界", 160, 90, 1);
    M5.Lcd.drawString("Здравствуй  мир", 160, 120, 1);
    M5.Lcd.drawString("こんにちは  せかい", 160, 150, 1);
    delay(3000);

    M5.Lcd.fillScreen(0);
    M5.Lcd.setCursor(0, 30);
    M5.Lcd.printf("☀☁☂☃☄★☆☇☈☉☊☋☌☍☎☏☐☑☒☓☔☕☖☗☘☙☚☛☜☝☞☟☠☡☢☣☤☥☦☧☨☩☪☫☬☭☮☯☸☹☺☻☼☽☾☿♀♁♂♃♄♅♆♇♈♉♊♋♌♍♎♏♐♑♒♓♔♕♖♗♘♙♚♛♜♝♞♟♠♡♢♣♤♥♦♧♨♩♪♫♬♭♮♯♰♱♲♳♴♵♶♷♸♹♺♻♼♽♾♿⚀⚁⚂⚃⚄⚅⚆⚇⚈⚉⚊⚋⚌⚍⚎⚏⚐⚑⚒⚓⚔⚕⚖⚗⚘⚙⚚⚛⚜⚝⚞⚟A⚠⚡⚢⚣⚤⚥⚦⚧⚨⚩⚪⚫⚬⚭⚮⚯B⚰⚱⚲⚳⚴⚵⚶⚷⚸⚹⚺⚻⚼⚽⚾⚿C⛀⛁⛂⛃⛄⛅⛆⛇⛈⛉⛊⛋⛌⛍⛎⛏D⛐⛑⛒⛓⛔⛕⛖⛗⛘⛙⛚⛛⛜⛝⛞⛟E⛠⛡⛢");
    delay(3000);

    M5.Lcd.fillScreen(0);
    M5.Lcd.setCursor(0, 30);
    M5.Lcd.printf("⛣⛤⛥⛦⛧⛨⛩⛪⛫⛬⛭⛮⛯F⛰⛱⛲⛳⛴⛵⛶⛷⛸⛹⛺⛻⛼⛽⛾⛿✀✁✂✃✄✅✆✇✈✉✊✋✌✍✎✏✐✑✒✓✔✕✖✗✘✙✚✛✜✝✞✟✠✡✢✣✤✥✦✧✨✩✪✫✬✭✮✯✰✱✲✳✴✵✶✷✸✹✺✻✼✽✾✿❀❁❂❃❄❅❆❇❈❉❊❋❌❍❎❏❐❑❒❓❔❕❖❗❘❙❚❛❜❝❞❟❠❡❢❣❤❥❦❧❨❩❪❫❬❭❮❯❰❱❲❳❴❵❶❷❸❹❺❻❼❽❾❿➀➁➂➃➄➅➆➇➈➉➊➋➌➍➎➏➐➑➒➓➔➕➖➗➘➙➚➛➜➝➞➟A➠➡➢➣➤➥➦➧➨➩➪➫➬➭➮➯B➰➱➲➳➴➵➶➷➸➹➺➻➼➽➾➿");
    delay(3000);
    
}
