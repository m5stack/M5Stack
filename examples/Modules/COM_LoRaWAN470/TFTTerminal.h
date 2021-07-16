#ifndef _TFTTERMINAL_H_
#define _TFTTERMINAL_H_

#include <M5Stack.h>
#include <Print.h>

class TFTTerminal : public Print
{
private:
    TFT_eSprite *disptr;
    char discharbuff[60][55];
    uint32_t xpos = 0,ypos = 0, dispos = 0;
    TFT_eSprite* _dis_buff_ptr = NULL;
    uint16_t _bkcolor = TFT_BLACK;
    uint16_t _color = TFT_GREEN;

    uint16_t _win_x_pos = 0,_win_y_pos = 0,_win_w = 320,_win_h = 240;
    uint16_t _font_x_size = 6,_font_y_size = 8;
    uint8_t  _fontSize = 0;
    uint16_t _line_x_limit = 53,_line_y_limit = 30;

public:
    TFTTerminal(TFT_eSprite *dis_buff_ptr);
    ~TFTTerminal();
    
    void setcolor( uint16_t color, uint16_t bk_color );
    void setGeometry(uint16_t x, uint16_t y, uint16_t w, uint16_t h );
    void setFontsize(uint8_t size);

    size_t write(uint8_t) ;
    size_t write(const uint8_t *buffer, size_t size);
};


#endif
