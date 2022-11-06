#ifndef __DISPLAY_CH_H_
#define __DISPLAY_CH_H_
#include <M5Stack.h>

#include "Fonts/ASC16.h"
#include "Fonts/HZK16.h"

typedef enum { DontUsedHzk16, InternalHzk16, ExternalHzk16 } Hzk16Types;

class DisplayCh {
   public:
    DisplayCh();
    /**************************************************************************
    **
    ** GBK character support
    **
    **************************************************************************/
    void loadHzk16(Hzk16Types hzkTypes   = InternalHzk16,
                   const char* HZK16Path = "/HZK16",
                   const char* ASC16Path = "/ASC16");
    void disableHzk16();

    void setTextColor(uint16_t c);
    void setTextColor(uint16_t c, uint16_t b);
    void setCursor(int16_t x, int16_t y);
    void setCursor(int16_t x, int16_t y, uint8_t font);
    void setTextSize(uint8_t size);
    void fillScreen(uint32_t color);

    // Highlight the text (Once set to be true, the text background will not be
    // transparent any more)
    inline void highlight(bool isHighlight) {
        highlighted = isHighlight;
    }
    // Set highlight color
    inline void setHighlightColor(uint16_t color) {
        highlightcolor = color;
        istransparent  = false;
    }

    void writeHzk(char* c);

   private:
    inline void setTransparentBgColor(bool isTransparent) {
        istransparent = isTransparent;
    }
    inline bool isTransparentBg() {
        return istransparent;
    }
    bool initHzk16(boolean use, const char* HZK16Path = nullptr,
                   const char* ASC16Path = nullptr);
    inline bool isHzk16Used() {
        return hzk16Used;
    }
    void setTextWrap(boolean wrap);
    void writeHzkAsc(const char c);
    void writeHzkGbk(const char* c);

   private:
    uint8_t hzkBufCount, hzkBuf[2];
    boolean hzk16Used, istransparent, highlighted;
    Hzk16Types hzk16Type;          // Use of HZK16 and ASC16 font.
    File Asc16File, Hzk16File,     // Font file
        *pAsc16File, *pHzk16File;  // Font file pointer

    uint8_t *pAscCharMatrix, *pGbkCharMatrix;
    uint16_t highlightcolor, ascCharWidth, ascCharHeigth, gbkCharWidth,
        gbkCharHeight;
    uint32_t textcolor, textbgcolor;
    int32_t cursor_x, cursor_y;
    uint8_t textfont, textsize;
    uint32_t _width, _height;  // Display w/h as modified by current rotation
    boolean textwrap;          // If set, 'wrap' text at right edge of display
};

#endif
