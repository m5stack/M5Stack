
#ifndef __DISPLATCURRENTTIME_H
#define __DISPLATCURRENTTIME_H
#include "FastLED.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NUM_LEDS 192
extern CRGB leds[NUM_LEDS];
extern void displayCurrentTime(uint16_t time4Day);
extern void diaplayPoint();
extern void clearPoint();
extern void clearLeds();
#ifdef __cplusplus
}
#endif
#endif
