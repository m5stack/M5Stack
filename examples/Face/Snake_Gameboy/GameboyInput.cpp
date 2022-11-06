/******************************************************************************
 * M5Snake : Input management from Gameboy faces                              *
 * ---------------------------------------------                              *
 * Management of input coming from the face Gameboy                           *
 * Author: Olivier Staquet                                                    *
 * Last version available on https://github.com/ostaquet/M5Snake              *
 *****************************************************************************/
#include "GameboyInput.h"

/**
 * Initialize
 */
void GameboyInputClass::begin(uint8_t _i2c_address, uint8_t _pin_int_face) {
    // Store local info
    i2c_address  = _i2c_address;
    pin_int_face = _pin_int_face;

    // Prepare the detection of activity
    pinMode(pin_int_face, INPUT_PULLUP);

    // Init the I2C
    Wire.begin();
}

/**
 * Check if button pressed and return which one
 */
uint8_t GameboyInputClass::getActivity() {
    // Check if there is activity on interrupt
    if (digitalRead(pin_int_face) == LOW) {
        // If yes, request 1 byte from the panel
        Wire.requestFrom(i2c_address, (uint8_t)1);

        // Check if data on the I2C is available
        while (Wire.available()) {
            // Receive one byte as character
            uint8_t key_val = Wire.read();
            if (key_val != 0x00) {
                return key_val;
            }
        }
    }

    // No activity to send back
    return GAMEBOY_KEY_NONE;
}

GameboyInputClass GameboyInput;
