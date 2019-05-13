#include "display_ch.h"
//#define _ASC16_ 
//#define _HZK16_ 
DisplayCh::DisplayCh(){
  // Set default highlight color
  highlightcolor = RED;
  highlighted = false;

  // HZK16 is not used by default
  hzk16Used = false;
  hzk16Type = DontUsedHzk16;
  //pHzk16File = nullptr;
  //pAsc16File = nullptr;

  textwrap = true;  

  _width  = 320;
  _height = 240;
}
/***************************************************************************************

** Function name:           setTextWrap

** Description:             Define if text should wrap at end of line

***************************************************************************************/

void DisplayCh::setTextWrap(boolean w) { textwrap = w; }
/***************************************************************************************

** Function name:           setTextSize

** Description:             Set the text size multiplier

***************************************************************************************/

void DisplayCh::setTextSize(uint8_t s) {

  if (s > 7)

    s = 7; // Limit the maximum size multiplier so byte variables can be used

           // for rendering

  textsize = (s > 0) ? s : 1; // Don't allow font size 0


  // Calculate the text width and height.

  // It's 8 pixels wide and 16 pixels high in for ASCII characters,

  // and 16 pixels both wide and high for GBK characters in HZK16 mode.

  ascCharWidth = 8 * textsize;

  ascCharHeigth = 16 * textsize;

  gbkCharWidth = ascCharHeigth;

  gbkCharHeight = gbkCharWidth;

}


/***************************************************************************************

** Function name:           setCursor

** Description:             Set the text cursor x,y position

***************************************************************************************/

void DisplayCh::setCursor(int16_t x, int16_t y) {

  cursor_x = x;

  cursor_y = y;

}



/***************************************************************************************

** Function name:           setCursor

** Description:             Set the text cursor x,y position and font

***************************************************************************************/

void DisplayCh::setCursor(int16_t x, int16_t y, uint8_t font) {

  textfont = font;

  cursor_x = x;

  cursor_y = y;

}
/***************************************************************************************

** Function name:           setTextColor

** Description:             Set the font foreground colour (background is

*transparent)

***************************************************************************************/

void DisplayCh::setTextColor(uint16_t c) {

  // For 'transparent' background, we'll set the bg

  // to the same as fg instead of using a flag

  textcolor = textbgcolor = c;

}

/***************************************************************************************

** Function name:           fillScreen

** Description:             Clear the screen to defined colour

***************************************************************************************/

void DisplayCh::fillScreen(uint32_t color) {

  M5.Lcd.fillRect(0, 0, _width, _height, color);

}

/***************************************************************************************

** Function name:           setTextColor

** Description:             Set the font foreground and background colour

***************************************************************************************/

void DisplayCh::setTextColor(uint16_t c, uint16_t b) {

  textcolor = c;

  textbgcolor = b;

}

/***************************************************************************************

** Function name:           loadHzk16

** Description:             loadHzk16 fonts

** Params:

**  HZK16Path: HZK16 file path on TF card. e.g. /HZK16, means file on the

*root dir of TF card. * ASC16Path: ASC16 file path on TF card.

***************************************************************************************/

void DisplayCh::loadHzk16(Hzk16Types hzkTypes,const char *HZK16Path, const char *ASC16Path) {

  if (hzk16Used)

    return;


//Hzk16Types hzkTypes = InternalHzk16
if(hzkTypes == InternalHzk16){
//#if defined(_ASC16_) && defined(_HZK16_)

  pAscCharMatrix = (uint8_t *)&ASC16[0];

  pGbkCharMatrix = (uint8_t *)&HZK16[0];

  Serial.println("HZK16 path: Internal");

  Serial.println("ASC16 path: Internal");

  hzk16Used = initHzk16(true, nullptr, nullptr);
}
else
{
  pAscCharMatrix = NULL;

  pGbkCharMatrix = NULL;



  Serial.print("HZK16 path: /SD");

  Serial.println(HZK16Path);

  Serial.print("ASC16 path: /SD");

  Serial.println(ASC16Path);

  hzk16Used = initHzk16(true, HZK16Path, ASC16Path);
}



  Serial.print("HZK16 init result: ");

  Serial.println(isHzk16Used());

}

/***************************************************************************************

** Function name:           disableHzk16

** Description:             disable Hzk16 fonts

** Params:

***************************************************************************************/

void DisplayCh::disableHzk16() {

  if (hzk16Used) {

    hzk16Used = initHzk16(false);

  }

}

bool DisplayCh::initHzk16(boolean use, const char *HZK16Path,
                        const char *ASC16Path) {

  bool result = false;

  if (use == false) { // Do not use HZK16 and ASC16 fonts

    hzk16Type = DontUsedHzk16;

    Serial.println("Use default font.");

  } else if (pAscCharMatrix == NULL ||

             pGbkCharMatrix ==

                 NULL) { // Use external HZK16 and ASC16 font on TF card.



    // Check if HZK16 and ASC16 files exist on TF card.

    if (SD.exists(HZK16Path) && SD.exists(ASC16Path)) { // Exists

      hzk16Type = ExternalHzk16;

      Serial.println("Use external HZK16 and ASC16 font.");

    } else { // Not exists

      hzk16Type = DontUsedHzk16;

      Serial.println("External font file HZK16/ASC16 lost, use default font.");

    }

  } else { // Use internal HZK16 and ASC16 fonts

    hzk16Type = InternalHzk16;

    Serial.println("Use internal HZK16 and ASC16 font.");

  }



  switch (hzk16Type) {

  case ExternalHzk16: {

    if (pHzk16File == NULL) {

      Hzk16File = SD.open(HZK16Path);

      pHzk16File = &Hzk16File;

    }

    if (pAsc16File == NULL) {

      Asc16File = SD.open(ASC16Path);

      pAsc16File = &Asc16File;

    }

    hzkBufCount = 0;

    break;

  }

  case InternalHzk16: {

    if (pAscCharMatrix == NULL || pGbkCharMatrix == NULL) {

      hzk16Type = DontUsedHzk16;

    }



    if (pHzk16File != NULL) {

      pHzk16File->close();

      pHzk16File = NULL;

    }

    if (pAsc16File != NULL) {

      pAsc16File->close();

      pAsc16File = NULL;

    }

    hzkBufCount = 0;

    break;

  }

  case DontUsedHzk16: {

    if (pHzk16File != NULL) {

      pHzk16File->close();

      pHzk16File = NULL;

    }

    if (pAsc16File != NULL) {

      pAsc16File->close();

      pAsc16File = NULL;

    }

    break;

  }

  }

  return hzk16Type != DontUsedHzk16;

}
void DisplayCh::writeHzkAsc(const char c) {

  if (c == '\n') {

    cursor_x = 0;

    cursor_y += ascCharHeigth;

  } else if (c != '\r') {

    uint32_t offset;

    uint8_t mask;

    uint16_t posX = cursor_x, posY = cursor_y;

    uint8_t charMatrix[16];

    uint8_t *pCharMatrix;



    offset = (uint32_t)c * 16;



    if (hzk16Type == ExternalHzk16) {

      pAsc16File->seek(offset, SeekSet);

      pAsc16File->readBytes((char *)&charMatrix[0], 16);

      pCharMatrix = &charMatrix[0];

    } else {

      if (pAscCharMatrix == NULL) {

        return;

      }

      pCharMatrix = pAscCharMatrix + offset;

    }



    // startWrite();



    if (highlighted) {

      M5.Lcd.fillRect(cursor_x, cursor_y, ascCharWidth, ascCharHeigth, highlightcolor);

    } else if (istransparent == false) {

      M5.Lcd.fillRect(cursor_x, cursor_y, ascCharWidth, ascCharHeigth, textbgcolor);

    }



    for (uint8_t row = 0; row < 16; row++) {

      mask = 0x80;

      posX = cursor_x;

      for (uint8_t col = 0; col < 8; col++) {

        if ((*pCharMatrix & mask) != 0) {

          if (textsize == 1) {

            M5.Lcd.drawPixel(posX, posY, textcolor);

          } else {

            M5.Lcd.fillRect(posX, posY, textsize, textsize, textcolor);

          }

        }

        posX += textsize;

        mask >>= 1;

      }

      posY += textsize;

      pCharMatrix++;

    }



    // endWrite();



    cursor_x += ascCharWidth;

    if (textwrap && ((cursor_x + ascCharWidth) > _width)) {

      cursor_x = 0;

      cursor_y += ascCharHeigth;

    }

  }

}

void DisplayCh::writeHzk(char* c){

  char *ret  = c;
  if(ret == NULL) return;
  while(*ret != '\0'){
    while(*ret <= 0xA0){
      if(*ret == '\0') return;
      writeHzkAsc(*ret);
      ret++;  
    }
    writeHzkGbk(ret);
    ret++;
    ret++;
  }
}


void DisplayCh::writeHzkGbk(const char *c) {

  uint32_t offset;

  uint8_t mask;

  uint16_t posX = cursor_x, posY = cursor_y;

  uint8_t charMatrix[32];

  uint8_t *pCharMatrix;



  offset = (uint32_t)(94 * (uint32_t)(c[0] - 0xA1) + (uint32_t)(c[1] - 0xA1)) * 32;

  if (hzk16Type == ExternalHzk16) {

    pHzk16File->seek(offset, SeekSet);

    pHzk16File->readBytes((char *)&charMatrix[0], 32);

    pCharMatrix = &charMatrix[0];

  } else {

    if (pGbkCharMatrix == NULL) {

      return;

    }

    pCharMatrix = pGbkCharMatrix + offset;

  }



  // startWrite();



  if (highlighted) {

    M5.Lcd.fillRect(cursor_x, cursor_y, gbkCharWidth, gbkCharHeight, highlightcolor);

  } else if (istransparent == false) {

    M5.Lcd.fillRect(cursor_x, cursor_y, gbkCharWidth, gbkCharHeight, textbgcolor);

  }



  for (uint8_t row = 0; row < 16; row++) {

    posX = cursor_x;

    mask = 0x80;

    for (uint8_t col = 0; col < 8; col++) {

      if ((*pCharMatrix & mask) != 0) {

        if (textsize == 1) {

          M5.Lcd.drawPixel(posX, posY, textcolor);

        } else {

          M5.Lcd.fillRect(posX, posY, textsize, textsize, textcolor);

        }

      }

      if ((*(pCharMatrix + 1) & mask) != 0) {

        if (textsize == 1) {

          M5.Lcd.drawPixel(posX + ascCharWidth, posY, textcolor);

        } else {

          M5.Lcd.fillRect(posX + ascCharWidth, posY, textsize, textsize, textcolor);

        }

      }

      mask >>= 1;

      posX += textsize;

    }

    posY += textsize;

    pCharMatrix += 2;

  }

  // endWrite();

  cursor_x += gbkCharWidth;

  if (textwrap && ((cursor_x + gbkCharWidth) > _width)) {

    cursor_x = 0;

    cursor_y += gbkCharHeight;

  }

}
