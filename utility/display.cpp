#include "display.h"

#define SPI_DC_HIGH()           digitalWrite(_dc, HIGH)
#define SPI_DC_LOW()            digitalWrite(_dc, LOW)
#define SPI_CS_HIGH()           digitalWrite(_cs, HIGH)
#define SPI_CS_LOW()            digitalWrite(_cs, LOW)
#define SPI_MOSI_HIGH()         digitalWrite(_mosi, HIGH)
#define SPI_MOSI_LOW()          digitalWrite(_mosi, LOW)
#define SPI_SCK_HIGH()          digitalWrite(_sclk, HIGH)
#define SPI_SCK_LOW()           digitalWrite(_sclk, LOW)
#define LCD_RST_HIGH()          digitalWrite(_rst, HIGH)
#define LCD_RST_LOW()          	digitalWrite(_rst, LOW)

static SPISettings spi_settings = SPISettings(SPI_CLOCK_DIV64, MSBFIRST, SPI_MODE0);

void M5STACK_TFTLCD::spiWrite(uint8_t byte)
{
	if(_sclk < 0){
		SPI.transfer(byte);
	} else {
		SPI_SCK_LOW();
		// Fast SPI bitbang swiped from LPD8806 library
		for(uint8_t bit = 0x80; bit; bit >>= 1) {
			if(byte & bit) {
				SPI_MOSI_HIGH();
			} else {
				SPI_MOSI_LOW();
			}
			SPI_SCK_HIGH();
			// delayMicroseconds(1);
			SPI_SCK_LOW();
		}
	}
}

void M5STACK_TFTLCD::spiWrite16(uint16_t dat)
{
	SPI_DC_HIGH();
	if(_sclk < 0){
		SPI.transfer16(dat);
	} else {
		spiWrite(dat>>8);		//	start byte RS=1,RW=0----Write a GRAM data
		spiWrite(dat);
	}
}

void M5STACK_TFTLCD::startWrite(void)
{
    SPI.beginTransaction(spi_settings);
    SPI_CS_LOW();
}

void M5STACK_TFTLCD::endWrite(void)
{
    SPI_CS_HIGH();
    SPI.endTransaction();
}

void M5STACK_TFTLCD::writeCommand(uint8_t index)
{
	SPI_DC_LOW();
	spiWrite(index);	//upper eight bits
	SPI_DC_HIGH();
}

void M5STACK_TFTLCD::LCDWriteReg(uint16_t index,uint16_t congfigtemp)
{
	writeCommand(index);
	spiWrite(congfigtemp);
}

void M5STACK_TFTLCD::LCDSetWindow(uint16_t xstat, uint16_t xend, uint16_t ystat, uint16_t yend)
{
	//HX8352-C
	xstat += 10;
	xend += 10;
	ystat += 144;
	yend += 144;

	//Set GRAM Area
	LCDWriteReg(0x02, xstat>>8);
	LCDWriteReg(0x03, xstat&0xff);     //Column Start
	LCDWriteReg(0x04, xend>>8);
	LCDWriteReg(0x05, xend&0xff);     //Column End

	LCDWriteReg(0x06, ystat>>8);
	LCDWriteReg(0x07, (ystat&0xff));     //Row Start
	LCDWriteReg(0x08, (yend>>8));
	LCDWriteReg(0x09, (yend&0xff));     //Row End
	writeCommand(0x22);
}

//-----------------------------------------------------------------------
M5STACK_TFTLCD::M5STACK_TFTLCD(int8_t _CS, int8_t _DC, int8_t _RST) : Adafruit_GFX(SPILCD_W, SPILCD_H) 
{
    _cs   = _CS;
    _dc   = _DC;
    _rst  = _RST;
    _sclk  = -1;
    _mosi  = -1;
    _miso  = -1;
}

M5STACK_TFTLCD::M5STACK_TFTLCD(int8_t _CS, int8_t _DC, int8_t _RST, int8_t _MOSI, int8_t _MISO, int8_t _SCLK) : Adafruit_GFX(SPILCD_W, SPILCD_H) 
{
	_cs   = _CS;
    _dc   = _DC;
    _rst  = _RST;
    _sclk  = _SCLK;
    _mosi  = _MOSI;
    _miso  = _MISO;
}

void M5STACK_TFTLCD::begin()
{
	pinMode(LCD_LED_PIN, OUTPUT);
	digitalWrite(LCD_LED_PIN, HIGH);
	// analogWrite(LCD_LED_PIN, 100);

	pinMode(_rst, OUTPUT);
	pinMode(_sclk, OUTPUT);
	pinMode(_dc, OUTPUT);
	pinMode(_cs, OUTPUT);
	pinMode(_mosi, OUTPUT);

	digitalWrite(_rst, HIGH);
	digitalWrite(_dc, HIGH);
	digitalWrite(_cs, HIGH);
	digitalWrite(_mosi, HIGH);
	digitalWrite(_sclk, HIGH); // Stop line floating

	if(_sclk < 0) {
		SPI.begin();
		// SPI.beginTransaction(spi_settings);
		// SPI.beginTransaction(SPISettings(SPI_CLOCK_DIV64, MSBFIRST, SPI_MODE0));
	}

	// toggle RST low to reset
	reset();
	delay(10);
	startWrite();
	LCDWriteReg(0xE8,0x48); // EQVCI_M1=0x00
	LCDWriteReg(0xE9,0x38); // EQGND_M1=0x1C
	LCDWriteReg(0xEA,0x00); // Command page 0
	LCDWriteReg(0xEB,0x00); // SUB_SEL=0xF6
	// Power saving for HX8357-A
	LCDWriteReg(0xEC,0x3C); // Command page 0
	LCDWriteReg(0xED,0xC4); // GENON=0x00

	LCDWriteReg(0xF1,0x01); // EQVCI_M0=0x1C
	LCDWriteReg(0xF2,0x00); // EQGND_M0=0x1C
	LCDWriteReg(0x60,0x08);
	LCDWriteReg(0x27,0xA3); // For GRAM read/write speed
	LCDWriteReg(0x29,0x01);
	LCDWriteReg(0x2B,0x02);
	LCDWriteReg(0x2E,0x76); //For Gate timing, prevent the display abnormal in RGB I/F
	LCDWriteReg(0x36,0x09);

	//SET PANEL
	LCDWriteReg(0x3a,0x55);
	LCDWriteReg(0x36,0x01);     //REV_P, SM_P, GS_P, BGR_P, SS_P
	LCDWriteReg(0x16,0x90);//MX, MY, RGB mode

	// Gamma
	LCDWriteReg(0x40,0x00);
	LCDWriteReg(0x41,0x01);
	LCDWriteReg(0x42,0x01);
	LCDWriteReg(0x43,0x12);
	LCDWriteReg(0x44,0x10);
	LCDWriteReg(0x45,0x24);
	LCDWriteReg(0x46,0x05);
	LCDWriteReg(0x47,0x5B);
	LCDWriteReg(0x48,0x03);
	LCDWriteReg(0x49,0x11);
	LCDWriteReg(0x4A,0x17);
	LCDWriteReg(0x4B,0x18);
	LCDWriteReg(0x4C,0x19);
	LCDWriteReg(0x50,0x1B);
	LCDWriteReg(0x51,0x2F);
	LCDWriteReg(0x52,0x2D);
	LCDWriteReg(0x53,0x3E);
	LCDWriteReg(0x54,0x3E);
	LCDWriteReg(0x55,0x3F);
	LCDWriteReg(0x56,0x30);
	LCDWriteReg(0x57,0x6E);
	LCDWriteReg(0x58,0x06);
	LCDWriteReg(0x59,0x07);
	LCDWriteReg(0x5A,0x08);
	LCDWriteReg(0x5B,0x0E);
	LCDWriteReg(0x5C,0x1C);
	LCDWriteReg(0x5D,0xCC);

	// Power Setting
	LCDWriteReg(0xE2,0x03);
	LCDWriteReg(0x1B,0x1D);
	LCDWriteReg(0x1A,0x01);
	LCDWriteReg(0x24,0x37); // Set VCOMH voltage, VHH=0x64
	LCDWriteReg(0x25,0x4F); // Set VCOML voltage, VML=0x71
	LCDWriteReg(0x23,0x6A); // Set VCOM offset, VMF=0x52	  ////////////60
	// Power on Setting
	LCDWriteReg(0x18,0x3A); // OSC_EN=1, Start to Oscillate
	LCDWriteReg(0x19,0x01); // AP=011
	LCDWriteReg(0x01,0x00); // Normal display(Exit Deep standby mode)
	LCDWriteReg(0x1F,0x88); // Exit standby mode and Step-up circuit 1 enable
	// GAS_EN=1, VCOMG=0, PON=0, DK=0, XDK=0, DDVDH_TRI=0, STB=0
	delay(5);
	LCDWriteReg(0x1F,0x80); // Step-up circuit 2 enable
	// GAS_EN=1, VCOMG=0, PON=1, DK=0, XDK=0, DDVDH_TRI=0, STB=0
	delay(5);
	LCDWriteReg(0x1F,0x90);
	delay(5);
	LCDWriteReg(0x1F,0xD0);
	// GAS_EN=1, VCOMG=1, PON=1, DK=0, XDK=1, DDVDH_TRI=0, STB=0
	delay(5);
	// Display ON Setting
	LCDWriteReg(0x17,0x05); // GON=1, DTE=1, D[1:0]=10
	delay(50);
	LCDWriteReg(0x28,0x38); // GON=1, DTE=1, D[1:0]=11
	LCDWriteReg(0x28,0x3C); // 16-bit/pixel

	// Set GRAM area 320x240
	LCDWriteReg(0x02,0x00);
	LCDWriteReg(0x03,0x00);	//Column Start
	LCDWriteReg(0x04,0x00);
	LCDWriteReg(0x05,0xAF);	//Column End
	LCDWriteReg(0x06,0x00);
	LCDWriteReg(0x07,0x90);	//Row Start
	LCDWriteReg(0x08,0x01);
	LCDWriteReg(0x09,0x40);	//Row End

	writeCommand(0x22);
	endWrite();
}

void M5STACK_TFTLCD::drawPixel(int16_t x, int16_t y, uint16_t color)
{
	if((x>219) || (y>175)) {
		return;
	}
	startWrite();
	LCDSetWindow(x,x,y,y);
	spiWrite16(color);
	endWrite();
}

void M5STACK_TFTLCD::writePixel(int16_t x, int16_t y, uint16_t color) 
{
	if((x>219) || (y>175)) {
		return;
	}
	LCDSetWindow(x,x,y,y);
	spiWrite16(color);
}

void M5STACK_TFTLCD::fillScreen(uint16_t color)
{
	uint16_t x,y;
	startWrite();
	LCDSetWindow(0,SPILCD_W-1,0,SPILCD_H-1);
	for(x=0;x<SPILCD_W;x++) {
		for(y=0;y<SPILCD_H;y++) {
			spiWrite16(color);
		}
	}
	endWrite();
}

void M5STACK_TFTLCD::writeFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
	if(y+h>176) {h = 175-x;}
	LCDSetWindow(x,x,y,y+h);
	for(uint8_t i=0; i<h; i++) {
		spiWrite16(color);
	}
}

void M5STACK_TFTLCD::writeFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
	if(x+w>220) {w = 219-x;}
	LCDSetWindow(x,x+w,y,y);
	for(uint8_t i=0; i<w; i++) {
		spiWrite16(color);
	}
}

void M5STACK_TFTLCD::drawPicture(int x, int y,uint16_t pic_H, uint16_t pic_V, const uint8_t* pic)
{
	unsigned long i;
	unsigned int j;
	startWrite();
	if(x>=0) {
		LCDSetWindow(x, (x+pic_H-1>219)?219:x+pic_H-1, y, y+pic_V-1);
		for(int i=0; i<pic_V; i++) {
			for(int j=0; j<pic_H*2; j++) {
				if(j/2+x < 220)
					spiWrite(pic[(i*pic_H*2)+j]);
			}
		}
	} else if(x<0) {
		LCDSetWindow(0, x+pic_H-1, y, y+pic_V-1);
		// int i,j;
		for(int i=0; i<pic_V; i++) {
			for(int j=0; j<(pic_H+x)*2; j++) {
				spiWrite(pic[(i*pic_V*2)+(2*abs(x))+j]);
			}
		}
	}
	endWrite();
}

#define HTONL(v)	( ((v) << 24) | (((v) >> 24) & 255) | (((v) << 8) & 0xff0000) | (((v) >> 8) & 0xff00) )
#define HTONS(v)	( (((v) << 8) & 0xff00) | (((v) >> 8) & 255) )

void M5STACK_TFTLCD::drawPicture(const char* filename)
{
  uint8_t buf[700];
  uint16_t tmp;
  File fsFile;

  if(SD.exists(filename)) {
    fsFile = SD.open(filename, "r");
    if(fsFile) {
        Serial.printf("open the %s, size:%d \r\n",filename, fsFile.size());
        fsFile.read(buf, 14);
        Serial.printf("bfType :0x%x \r\n", ((BitMapFileHeader*)buf)->bfType);
        fsFile.read(buf, 40);
        Serial.printf("biSize:%d, Width:%d, Height:%d, biBitCount%d\r\n", \
        ((BitMapInfoHeader*)buf)->biSize,   ((BitMapInfoHeader*)buf)->biWidth, \
        ((BitMapInfoHeader*)buf)->biHeight, ((BitMapInfoHeader*)buf)->biBitCount);

        // BitMapInfoHeader
        // fsFile.read(buf, 32);

		int bmp_bit_count = ((BitMapInfoHeader*)buf)->biBitCount;
        uint16_t r,g,b;
		uint16_t rev_color;
		bool rev_bit=1;

		startWrite();
        LCDSetWindow(0, 219, 0, 175);
		endWrite();

		if(bmp_bit_count == 16) {
			for(int i=0; i<176; i++) {
				int sizen = fsFile.read(buf, 440);
				// Serial.printf("i:%d, sizen:%d\r\n", i, sizen);
				startWrite();
				// LCDSetWindow(0, 219, 176-i, 176-i);
				for(int j=0; j<220; j++) {
					spiWrite(buf[j*2+1]);
					spiWrite(buf[j*2]);
				}
				endWrite();
			}
		} else if(bmp_bit_count == 24) {
			for(int i=0; i<176; i++) {
				int sizen = fsFile.read(buf, 660);
				// Serial.printf("i:%d, sizen:%d\r\n", i, sizen);
				startWrite();
				// LCDSetWindow(0, 219, 176-i, 176-i);
				// LCDSetWindow(0, 219, i, i);
				for(int j=0; j<220; j++) {
					// rev_color = Combine16Bit_565(buf[j*3+2], buf[j*3+1], buf[j*3]);
					// if(rev_bit) spiWrite(buf[j-1]);
					// else spiWrite(buf[j+1]);
					// rev_bit = !rev_bit;
					// rev_color = buf[j*2];
					// sw16(rev_color);
					// HTONS(rev_color);
					// spiWrite16(rev_color);
					// Serial.printf("%x ", buf[j]);
					rev_color  = (((uint16_t)buf[j*3+2] >> 3) << 11) ;
					rev_color |= (((uint16_t)buf[j*3+1] >> 2) << 5) ;
					rev_color |= ((uint16_t)buf[j*3] >> 3) ;

					// spiWrite(buf[j*2+1]);
					spiWrite16(rev_color);
				}
				endWrite();
				// drawPicture(0, 176-i, 219, 1, buf);
			}
		}
        fsFile.close();
    }
  }
}

void M5STACK_TFTLCD::ProgressBar(int x, int y, int w, int h, uint8_t val)
{
	drawRect(x, y, w, h, 0x09F1);
	fillRect(x+1, y+1, w*(((float)val)/100.0), h-1, 0x09F1);
}

void M5STACK_TFTLCD::reset(void)
{
	LCD_RST_HIGH();
	delay(50);
	LCD_RST_LOW();
	delay(10);
	LCD_RST_HIGH();
	delay(10);
}

void M5STACK_TFTLCD::buttonSet(uint8_t button_id, const char* str)
{
	#define X_OFFSET 40
	#define Y_POS 165
	switch (button_id) {
		case 0:
			buttons[0].initButton((this), X_OFFSET, Y_POS, 44, 30, GRAY, WHITE, GRAY, str, 1);
			buttons[0].drawButton(0);
			break;

		case 1:
			buttons[1].initButton((this), X_OFFSET + 68,  Y_POS, 44, 30, GRAY, WHITE, GRAY, str, 1);
			buttons[1].drawButton(0);
			break;

		case 2:
			buttons[2].initButton((this), X_OFFSET + 136, Y_POS, 44, 30, GRAY, WHITE, GRAY, str, 1);
			buttons[2].drawButton(0);
			break;
	}
}

void M5STACK_TFTLCD::buttonEnable(uint8_t button_id)
{
	buttons[button_id].enable();
}

void M5STACK_TFTLCD::buttonDisable(uint8_t button_id)
{
	buttons[button_id].disable();
	// fillRect(0, 150, 219, 30, WHITE);
}

void M5STACK_TFTLCD::buttonUpdate() 
{
	#define BUTTON_A_PIN        39
	#define BUTTON_B_PIN        38
	#define BUTTON_C_PIN        37
	const uint8_t button_table[3] = {BUTTON_A_PIN, BUTTON_B_PIN, BUTTON_C_PIN};
	for(uint8_t i=0; i<3; i++) {
		if(buttons[i].state() == true) {
			if(buttons[i].preLevel() != !digitalRead(button_table[i])) {
				buttons[i].drawButton(!digitalRead(button_table[i]));
			}
		}
	}
}

void M5STACK_TFTLCD::drawTitle(char *str, int color)
{
	setFont(&FreeSansOblique9pt7b);
	fillRect(0, 0, 230, 22, color);
	setCursor(4, 16);
	setTextColor(WHITE);
	setTextSize(1);
	printf(str);
}

// code for the GFX button UI element

TFTLCD_Button::TFTLCD_Button(void) {
  _gfx = 0;
}

void TFTLCD_Button::initButton(
 Adafruit_GFX *gfx, int16_t x, int16_t y, uint8_t w, uint8_t h,
 uint16_t outline, uint16_t fill, uint16_t textcolor,
 const char *label, uint8_t textsize)
{
  _x            = x;
  _y            = y;
  _w            = w;
  _h            = h;
  _outlinecolor = outline;
  _fillcolor    = fill;
  _textcolor    = textcolor;
  _textsize     = textsize;
  _gfx          = gfx;
  strncpy(_label, label, 9);
  _label[9] = 0;
  enable();
}

void TFTLCD_Button::drawButton(boolean inverted) {
	uint16_t x, y;
	uint16_t fill, outline, text;
  
	_pre_level = inverted;
	fill    = _fillcolor;
	outline = _outlinecolor;
	text    = _textcolor;

	x = _x;
	_gfx->setFont(NULL);
	_gfx->fillRect(_x-_w/2-2, 147, _w+4, 28, _fillcolor);

  if(!inverted) {
		// _gfx->fillRect(_x-_w/2-1, _y+10-_h/2-1, _w+2, _h+3, _fillcolor);
		y = _y;
    // fill    = _fillcolor;
    // outline = _outlinecolor;
    // text    = _textcolor;
  } else {
		// _gfx->fillRect(_x-_w/2-1, _y-_h/2-1, _w+2, _h+3, _fillcolor);
		y = _y+12;
    // fill    = _textcolor;
    // outline = _outlinecolor;
    // text    = _fillcolor;
    // x = _x+15;
  }

  // _gfx->fillRoundRect(x - (_w/2), y - (_h/2), _w, _h, min(_w,_h)/4, fill);
  _gfx->drawRoundRect(x - (_w/2),   y - (_h/2),   _w,   _h, 7, outline);
  _gfx->drawRoundRect(x - (_w/2)-1, y - (_h/2)-1, _w+2, _h, 8, outline);
  // _gfx->drawRoundRect(x - (_w/2),   y - (_h/2),   _w, _h, min(_w,_h)/4, outline);
  // _gfx->drawRoundRect(x - (_w/2)-1, y - (_h/2)-1, _w+2, _h, min(_w,_h)/4, outline);

  _gfx->setCursor(x - strlen(_label)*3*_textsize, y-4*_textsize);
  _gfx->setTextColor(text);
  _gfx->setTextSize(_textsize);
  _gfx->print(_label);
}

boolean TFTLCD_Button::contains(int16_t x, int16_t y) {
  if ((x < (_x - _w/2)) || (x > (_x + _w/2))) return false;
  if ((y < (_y - _h/2)) || (y > (_y + _h/2))) return false;
  return true;
}

void TFTLCD_Button::press(boolean p) {
  laststate = currstate;
  currstate = p;
}

boolean TFTLCD_Button::isPressed() { return currstate; }
boolean TFTLCD_Button::justPressed() { return (currstate && !laststate); }
boolean TFTLCD_Button::justReleased() { return (!currstate && laststate); }
boolean TFTLCD_Button::state() { return _action; }
boolean TFTLCD_Button::preLevel() { return _pre_level; }
void TFTLCD_Button::enable() { _action = 1; }
void TFTLCD_Button::disable() { _action = 0; }
