#ifndef _M5BUTTON_H_
#define _M5BUTTON_H_

class Gesture;

#include <Arduino.h>
#include "PointAndZone.h"
#include <M5Display.h>
#include <Free_Fonts.h>

#define TFT		M5Display::instance
#define EVENTS	M5Events::instance
#define BUTTONS	M5Buttons::instance

#define MAX_TAP				150
#define MAX_BETWEEN_TAP		200
#define GESTURE_MAXTIME		500
#define GESTURE_MINDIST		75

#define NUM_EVENTS			9
#define E_TOUCH				0x0001
#define E_RELEASE			0x0002
#define E_MOVE  			0x0004
#define E_GESTURE			0x0008
#define E_TAP				0x0010
#define E_DBLTAP			0x0020
#define E_DRAGGED			0x0040
#define E_PRESSED			0x0080
#define E_PRESSING			0x0100

#define E_ALL				0x0FFF
#define E_BTNONLY			0x1000

#define NODRAW				0x0120	// Special color value: transparent

#define BUTTON_FREEFONT		FSS9
#define BUTTON_TEXTFONT		1
#define BUTTON_TEXTSIZE		1
#define BUTTON_DATUM		MC_DATUM

struct ButtonColors {
	uint16_t bg;
	uint16_t text;
	uint16_t outline;
};

class Button;
	
class Event {
  public:
  	Event();
  	operator bool();
	const char* typeName();
	const char* objName();
	uint8_t finger;
	uint16_t type;
	Point from, to;
	uint16_t duration;
	Button* button;
	Gesture* gesture;
};

class Button : public Zone {
  public:
    static std::vector<Button*> instances;
	Button(
	  int16_t x_, int16_t y_, int16_t w_, int16_t h_,
	  bool rot1_ = false,
	  const char* name_ = "",
	  ButtonColors off_ = {NODRAW, NODRAW, NODRAW},
	  ButtonColors on_ = {NODRAW, NODRAW, NODRAW},
	  uint8_t datum_ = BUTTON_DATUM,
	  int16_t dx_ = 0,
	  int16_t dy_ = 0,
	  uint8_t r_ = 0xFF
	);
	Button(
	  uint8_t pin, uint8_t invert, uint32_t dbTime, String hw = "hw",
	  int16_t x_ = 0, int16_t y_ = 0, int16_t w_ = 0, int16_t h_ = 0,
	  bool rot1_ = false,
	  const char* name_ = "",
	  ButtonColors off_ = {NODRAW, NODRAW, NODRAW},
	  ButtonColors on_ = {NODRAW, NODRAW, NODRAW},
	  uint8_t datum_ = BUTTON_DATUM,
	  int16_t dx_ = 0,
	  int16_t dy_ = 0,
	  uint8_t r_ = 0xFF
	);
	~Button();
	operator bool();
	int16_t instanceIndex();
	bool read();
	bool setState(bool);
	bool isPressed();
	bool isReleased();
	bool wasPressed();
	bool wasReleased();
	bool pressedFor(uint32_t ms);
	bool pressedFor(uint32_t ms, uint32_t continuous_time);
	bool releasedFor(uint32_t ms);
	bool wasReleasefor(uint32_t ms);
	void addHandler(void (*fn)(Event&), uint16_t eventMask = E_ALL);
	void delHandlers(void (*fn)(Event&) = nullptr);
	uint32_t lastChange();
	uint8_t finger;
	bool changed;
	char name[16];
	Event event;
	uint8_t pin;
	uint32_t dbTime;
	bool invert;
  private:
  	friend class M5Buttons;
  	void init();
	bool _state, _tapWait, _pressing;
	uint32_t _time;
	uint32_t _lastChange, _lastLongPress, _pressTime, _hold_time;
	
  // visual stuff
  public:
	void draw(ButtonColors bc);
	void draw();
	void setLabel(const char* label_);
	void setFont(const GFXfont* freeFont_);
	void setFont(uint8_t textFont_ = 0);
	void setTextSize(uint8_t textSize_ = 0);
	ButtonColors off, on;
	Zone* drawZone;
	uint8_t datum, r;
	int16_t dx, dy;
	void (*drawFn)(Button* b, ButtonColors bc);
	char label[51];
	bool compat; // For TFT_eSPI_Button emulation		
  private:
	uint8_t _textFont;
	const GFXfont* _freeFont;
	uint8_t _textSize;
};

class M5Buttons {
  public:
    static M5Buttons* instance;
    static void drawFunction(Button* button, ButtonColors bc);
	M5Buttons();
    void setUnchanged();
	Button* which(Point& p);
	void draw();
	void setFont(const GFXfont* freeFont_);
	void setFont(uint8_t textFont_);
	void setTextSize(uint8_t textSize_);
	void (*drawFn)(Button* button, ButtonColors bc);
  private:
	uint8_t _textFont;
	const GFXfont* _freeFont;
	uint8_t _textSize;
};

class Gesture {
  public:
	static std::vector<Gesture*> instances;
  	Gesture(
  	  Zone& fromZone_,
  	  Zone& toZone_,
  	  const char* name_ = "",
  	  uint16_t maxTime_ = GESTURE_MAXTIME,
  	  uint16_t minDistance_ = GESTURE_MINDIST
  	);
  	~Gesture();
  	int16_t instanceIndex();
  	bool test(Event& e);
  	bool wasDetected();
  	void addHandler(void (*fn)(Event&), uint16_t eventMask = E_ALL);
  	void delHandlers(void (*fn)(Event&) = nullptr);
	Zone* fromZone;
	Zone* toZone;
  	uint16_t maxTime, minDistance;
  	char name[16];
	bool detected;
};

struct EventHandler {
	uint16_t eventMask;
	Button* button;
	Gesture* gesture;
	void (*fn)(Event&);
};

class M5Events {
  public:
    M5Events();
    static M5Events* instance;
    Event fireEvent(
      uint8_t finger,
      uint16_t type,
      Point& from,
      Point& to,
      uint16_t duration,
      Button* button,
      Gesture* gesture
    );
    void addHandler(
      void (*fn)(Event&),
      uint16_t eventMask = E_ALL,
      Button* button = nullptr,
      Gesture* gesture = nullptr
    );
    void delHandlers(
      void (*fn)(Event&),
      Button* button,
      Gesture* gesture
    );
	std::vector<EventHandler> _eventHandlers;
};

// TFT_eSPI_Button compatibility
class TFT_eSPI_Button : public Button {
  public:
	TFT_eSPI_Button();
	void initButton(
	  TFT_eSPI *gfx, 
	  int16_t x, int16_t y, uint16_t w, uint16_t h, 
	  uint16_t outline, uint16_t fill, uint16_t textcolor,
	  char *label,
	  uint8_t textsize_
	);
	void initButtonUL(
	  TFT_eSPI *gfx,
	  int16_t x_, int16_t y_, uint16_t w_, uint16_t h_, 
	  uint16_t outline, uint16_t fill, uint16_t textcolor, 
	  char *label,
	  uint8_t textsize_
	);
	void     setLabelDatum(int16_t x_delta, int16_t y_delta, uint8_t datum = MC_DATUM);
	void     drawButton(bool inverted = false, String long_name = "");
	bool     contains(int16_t x, int16_t y);
	void     press(bool p);
	bool     isPressed();
	bool     justPressed();
	bool     justReleased();
};

#endif /* _M5BUTTON_H_ */