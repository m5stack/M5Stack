#include "TFTTerminal.h"

TFTTerminal::TFTTerminal(TFT_eSprite *dis_buff_ptr) {
    _dis_buff_ptr = dis_buff_ptr;
    memset(discharbuff, 0, 55 * 60);
}

TFTTerminal::~TFTTerminal() {
}

void TFTTerminal::setcolor(uint16_t color, uint16_t bk_color) {
    _color   = color;
    _bkcolor = bk_color;
}

void TFTTerminal::setGeometry(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
    _win_x_pos = x;
    _win_y_pos = y;
    _win_w     = w;
    _win_h     = h;

    _line_x_limit = _win_w / _font_x_size;
    _line_y_limit = _win_h / _font_y_size;
}

void TFTTerminal::setFontsize(uint8_t size) {
    size         = (size == 0) ? 1 : size;
    _font_x_size = 5 * size;
    _font_y_size = 6 * size;
    _fontSize    = size;

    _line_x_limit = _win_w / _font_x_size;
    _line_y_limit = _win_h / _font_y_size;
}

size_t TFTTerminal::write(uint8_t chardata) {
    bool flush_page_flag = false;
    uint8_t dis_y_pos    = 0;

    if ((chardata == '\r') || (chardata == '\n')) {
        xpos = 0;
        ypos++;
        ypos = ypos % 60;
        memset(discharbuff[ypos % 60], 0, 55);
        return 1;
    } else if (xpos >= _line_x_limit) {
        xpos = 0;
        ypos++;
        ypos = ypos % 60;
        memset(discharbuff[ypos % 60], 0, 55);
    }

    discharbuff[ypos][xpos] = chardata;
    xpos++;

    if ((dispos <= ypos) && ((ypos - dispos) > _line_y_limit)) {
        dispos          = ypos - _line_y_limit;
        flush_page_flag = true;
    } else if ((dispos <= ypos) && ((ypos - dispos) <= _line_y_limit)) {
        dis_y_pos       = ypos - dispos;
        flush_page_flag = false;
    } else if ((dispos > ypos) && ((60 - (dispos - ypos)) > _line_y_limit)) {
        dispos          = 60 - (_line_y_limit - ypos);
        flush_page_flag = true;
    } else if ((dispos > ypos) && ((60 - (dispos - ypos)) > _line_y_limit)) {
        dis_y_pos       = 60 - (dispos - ypos);
        flush_page_flag = false;
    }

    dispos = dispos % 60;

    _dis_buff_ptr->setTextColor(_color);
    _dis_buff_ptr->setTextSize(_fontSize);

    if (flush_page_flag) {
        _dis_buff_ptr->fillSprite(_bkcolor);

        for (size_t i = 0; i < _line_y_limit; i++) {
            _dis_buff_ptr->drawString((char *)discharbuff[(dispos + i) % 60], 0,
                                      i * _font_y_size);
        }
    } else {
        _dis_buff_ptr->drawChar(chardata, (xpos - 1) * _font_x_size,
                                dis_y_pos * _font_y_size);
    }
    _dis_buff_ptr->pushSprite(_win_x_pos, _win_y_pos);

    return 1;
}

size_t TFTTerminal::write(const uint8_t *buffer, size_t size) {
    while ((size != 0) && (*buffer != '\0')) {
        if ((*buffer == '\r') || (*buffer == '\n')) {
            xpos = 0;
            ypos++;
            ypos = ypos % 60;
            memset(discharbuff[ypos % 60], 0, 55);
            buffer++;
            size--;
            continue;
        } else if (xpos >= _line_x_limit) {
            xpos = 0;
            ypos++;
            ypos = ypos % 60;
            memset(discharbuff[ypos % 60], 0, 55);
        }
        discharbuff[ypos][xpos] = *buffer;
        xpos++;
        buffer++;
        size--;
    }

    if ((dispos <= ypos) && ((ypos - dispos) > _line_y_limit)) {
        dispos = ypos - _line_y_limit;
    } else if ((dispos > ypos) && ((60 - (dispos - ypos)) > _line_y_limit)) {
        dispos = 60 - (_line_y_limit - ypos);
    }

    dispos = dispos % 60;

    _dis_buff_ptr->setTextColor(_color);
    _dis_buff_ptr->setTextSize(_fontSize);
    _dis_buff_ptr->fillSprite(_bkcolor);
    //_dis_buff_ptr->fillRect(_win_x_pos, _win_y_pos, _win_w, _win_h, _bkcolor);
    for (size_t i = 0; i < _line_y_limit; i++) {
        _dis_buff_ptr->drawString((char *)discharbuff[(dispos + i) % 60], 0,
                                  i * _font_y_size);
    }
    _dis_buff_ptr->pushSprite(_win_x_pos, _win_y_pos);
    return 1;
}
