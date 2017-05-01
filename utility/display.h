#ifndef DISPLAY_H
#define	DISPLAY_H

#include <SPI.h>
#include <Adafruit_GFX.h>
#include "utility/config.h"

#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeMonoBoldOblique9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSerif9pt7b.h>
#include <Fonts/FreeSerifItalic9pt7b.h>
#include <Fonts/FreeSansOblique9pt7b.h>
#include <Fonts/FreeMonoOblique9pt7b.h>
#include <Fonts/FreeSansBoldOblique9pt7b.h>
#include <Fonts/FreeSerifBold9pt7b.h>
#include <Fonts/FreeSerifBoldItalic9pt7b.h>
#include <Fonts/FreeSansBold9pt7b.h>

#define WHITE						0xFFFF
#define BLACK						0x0000
#define BLUE						0x001F
#define BRED						0XF81F
#define GRED						0XFFE0
#define GBLUE						0X07FF
#define RED							0xF800
#define MAGENTA					0xF81F
#define GREEN						0x07E0
#define CYAN						0x7FFF
#define YELLOW					0xFFE0
#define BROWN						0XBC40
#define BRRED						0XFC07
#define GRAY						0X8430

#ifndef _swap_int16_t
#define _swap_int16_t(a, b) { int16_t t = a; a = b; b = t; }
#endif

class TFTLCD_Button {

 public:
  TFTLCD_Button(void);
  void initButton(Adafruit_GFX *gfx, int16_t x, int16_t y,
   uint8_t w, uint8_t h, uint16_t outline, uint16_t fill,
   uint16_t textcolor, char *label, uint8_t textsize);
  void drawButton(boolean inverted = false);
  boolean contains(int16_t x, int16_t y);

  void press(boolean p);
  boolean isPressed();
  boolean justPressed();
  boolean justReleased();
  boolean state();
  bool preLevel();
  void disable();
  void enable();

 private:
  Adafruit_GFX *_gfx;
  int16_t _x, _y;
  uint16_t _w, _h;
  uint8_t _textsize;
  uint16_t _outlinecolor, _fillcolor, _textcolor;
  char _label[10];
  bool _action;
  bool _pre_level;

  boolean currstate, laststate;
};

class M5STACK_TFTLCD : public Adafruit_GFX {

 public:
  M5STACK_TFTLCD(int8_t _CS = SPI_CS_PIN, int8_t _DC = LCD_RS_PIN, int8_t _RST = LCD_RST_PIN);
  M5STACK_TFTLCD(int8_t _CS, int8_t _DC, int8_t _RST, int8_t _MOSI, int8_t _MISO, int8_t _SCLK);

  void     begin(void);
  void     drawPixel(int16_t x, int16_t y, uint16_t color);
  void     writePixel(int16_t x, int16_t y, uint16_t color);
  void     writeFastHLine(int16_t x0, int16_t y0, int16_t w, uint16_t color);
  void     writeFastVLine(int16_t x0, int16_t y0, int16_t h, uint16_t color);
  // void     writeFillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t c);
  void     fillScreen(uint16_t color);
  void     reset(void);
  void     drawPicture(int x, int y,uint16_t pic_H, uint16_t pic_V, const uint8_t* pic);
  void     drawPicture();
  void     ProgressBar(int x, int y, int w, int h, uint8_t val);

  // LCD screen button
  TFTLCD_Button buttons[3];
  void     buttonSet(uint8_t button_id, char* str);
  void     buttonEnable(uint8_t button_id);
  void     buttonDisable(uint8_t button_id);
  void     buttonUpdate();

  // GUI menu
  void     drawTitle(char *str, int color);
     
 private:
  int8_t      _cs, _dc, _rst, _sclk, _mosi, _miso;
  uint32_t    _freq;
  void spiWrite(uint8_t byte);
  void spiWrite16(uint16_t dat);
  void writeCommand(uint8_t Index);
  void startWrite(void);
  void endWrite(void);
  void LCDWriteReg(uint16_t index,uint16_t congfigtemp);
  void LCDSetWindow(uint16_t xstat, uint16_t xend, uint16_t ystat, uint16_t yend);
};



#endif
