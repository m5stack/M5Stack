/******************************************************************************
 * M5Snake : Input management from Gameboy faces                              *
 * ---------------------------------------------                              *
 * Management of input coming from the face Gameboy                           *
 * Author: Olivier Staquet                                                    *
 * Last version available on https://github.com/ostaquet/M5Snake              *
 *****************************************************************************/
#ifndef _GAMEBOYINPUT_H_
#define _GAMEBOYINPUT_H_

#include <Arduino.h>
#include "Wire.h"

#define GAMEBOY_KEY_NONE        0x00
#define GAMEBOY_KEY_RELEASED    0xFF
#define GAMEBOY_KEY_START       0x7F
#define GAMEBOY_KEY_SELECT      0xBF
#define GAMEBOY_KEY_A           0xEF
#define GAMEBOY_KEY_B           0xDF
#define GAMEBOY_KEY_UP          0xFE
#define GAMEBOY_KEY_DOWN        0xFD
#define GAMEBOY_KEY_LEFT        0xFB
#define GAMEBOY_KEY_RIGHT       0xF7

class GameboyInputClass {
  public:
    // Initialize
    void begin(uint8_t _i2c_address = 0x08, uint8_t _pin_int_face = 5);

    // Check if button pressed and return which one
    uint8_t getActivity();

  private:
    // I2C address
    uint8_t i2c_address = 0x00;
    // PIN for interrupt of the Face
    uint8_t pin_int_face = 0x00;
};

extern GameboyInputClass GameboyInput;

#endif // _GAMEBOYINPUT_H_
