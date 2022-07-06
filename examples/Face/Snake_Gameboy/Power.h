/******************************************************************************
 * M5Snake : Power management                                                 *
 * --------------------------                                                 *
 * Management of the charge of the battery to avoid over charging             *
 * Author: Olivier Staquet                                                    *
 * Last version available on https://github.com/ostaquet/M5Snake              *
 *****************************************************************************/
#ifndef _POWER_H_
#define _POWER_H_

#include <Arduino.h>
#include <M5Stack.h>

class PowerClass {
   public:
    // Initialize
    void begin();

    // Adjust charge mode
    void adaptChargeMode();

    // Return battery level (0-100%)
    // (-1 if cannot communicate with the controller)
    int8_t getBatteryLevel();

   private:
    // Power control available
    bool canControl = false;
};

extern PowerClass Power;

#endif  // _POWER_H_
