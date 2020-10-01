#ifndef _POINTANDZONE_H_
#define _POINTANDZONE_H_


#include <Arduino.h>
#include <M5Display.h>

#define DISPLAY_WIDTH	320
#define DISPLAY_HEIGHT	240
#define TFT				M5Display::instance

class Zone;

class Point {
  public:
	Point(int16_t x_ = -1, int16_t y_ = -1);
	bool operator ==(const Point& p);
	bool operator !=(const Point& p);
	operator char*();
	void set(int16_t x_ = -1, int16_t y_ = -1);
    bool in(Zone& z);
    bool Equals(const Point& p);
    bool valid();
    uint16_t distanceTo(const Point& p);
    void rotate(uint8_t m);
    int16_t x, y;
  private:
    char _text[12];
};

class Zone {
  public:
	Zone(int16_t x_, int16_t y_, int16_t w_, int16_t h_, bool rot1_ = false);
	void set(int16_t x_, int16_t y_, int16_t w_, int16_t h_, bool rot1_ = false);
    bool contains(const Point &p);
    bool contains(int16_t x, int16_t y);
    void rotate(uint8_t m);
    int16_t x, y, w, h;
    // If rot1 is true, the zone specified in rotation 1 coordinates
    // (stays in same place on physical screen, regardless of rotation.)
    bool rot1;
};

#endif /* _POINTANDZONE_H_ */