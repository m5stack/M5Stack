#include "PointAndZone.h"


// Point class

Point::Point(int16_t x_ /* = -1 */, int16_t y_ /* = -1 */) {
	x = x_;
	y = y_;
}

bool Point::operator ==(const Point& p) {
	return (Equals(p));
}
	
bool Point::operator !=(const Point& p) {
	return (!Equals(p));
}

Point::operator char*() {
	sprintf(_text, "(%d, %d)", x, y);
	return _text;
}

void Point::set(int16_t x_ /* = -1 */, int16_t y_ /* = -1 */) {
	x = x_;
	y = y_;
}

bool Point::Equals(const Point& p) {
	return (x == p.x && y == p.y);
}

bool Point::in(Zone& z) {
	return (z.contains(x, y));
}

bool Point::valid() {
	return !(x == -1 && y == -1);
}

uint16_t Point::distanceTo(const Point& p) {
	int16_t dx = x - p.x;
	int16_t dy = y - p.y;
	return sqrt(dx*dx + dy*dy);
}

void Point::rotate(uint8_t m) {
	int16_t normal_x = x;
	int16_t normal_y = y;
	int16_t inv_x = DISPLAY_WIDTH - 1 - x;
	int16_t inv_y = DISPLAY_HEIGHT - 1 - y;
	// inv_y can be negative for area below screen of M5Core2
	switch (m) {
	  case 0:
		x = inv_y;
		y = normal_x;
		break;
	  case 2:
	    x = normal_y;
	    y = inv_x;
	    break;
	  case 3:
	  	x = inv_x;
	  	y = inv_y;
	  	break;
	  // rotations 4-7 are mirrored
	  case 4:
	  	x = inv_y;
	  	y = inv_x;
	  	break;
	  case 5:
	  	x = normal_x;
	  	y = inv_y;
	  	break;
	  case 6:
	  	x = normal_y;
	  	y = normal_x;
	  	break;
	  case 7:
		x = inv_x;
		y = normal_y;
		break;
	}
}



// Zone class

Zone::Zone(int16_t x_, int16_t y_, int16_t w_, int16_t h_, bool rot1_ /* = false */) {
	set(x_, y_, w_, h_, rot1_);
}

void Zone::set(int16_t x_, int16_t y_, int16_t w_, int16_t h_, bool rot1_ /* = false */) {
	x = x_;
	y = y_;
	w = w_;
	h = h_;
	rot1 = rot1_;
}

bool Zone::contains(const Point &p) {
	return contains(p.x, p.y);
}

bool Zone::contains(int16_t x_, int16_t y_) {
	#ifdef TFT
		if (rot1 && TFT->rotation != 1) {
			Zone t = *this;
			t.rotate(TFT->rotation);
			return (y_ >= t.y && y_ <= t.y + t.h && x_ >= t.x && x_ <= t.x + t.w);
		}
	#endif
	return (y_ >= y && y_ <= y + h && x_ >= x && x_ <= x + w);
}

void Zone::rotate(uint8_t m) {
	int16_t normal_x = x;
	int16_t normal_y = y;
	int16_t inv_x = 319 - x - w;
	int16_t inv_y = 239 - y - h;	// negative for area outside of screen
	switch (m) {
	  case 0:
		x = inv_y;
		y = normal_x;
		break;
	  case 2:
	    x = normal_y;
	    y = inv_x;
	    break;
	  case 3:
	  	x = inv_x;
	  	y = inv_y;
	  	break;
	  // rotations 4-7 are mirrored
	  case 4:
	  	x = inv_y;
	  	y = inv_x;
	  	break;
	  case 5:
	  	x = normal_x;
	  	y = inv_y;
	  	break;
	  case 6:
	  	x = normal_y;
	  	y = normal_x;
	  	break;
	  case 7:
		x = inv_x;
		y = normal_y;
		break;
	}
	if (!(m % 2)) std::swap(w, h);
}