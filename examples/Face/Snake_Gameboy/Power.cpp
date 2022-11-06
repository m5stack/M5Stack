/******************************************************************************
 * M5Snake : Power management                                                 *
 * --------------------------                                                 *
 * Management of the charge of the battery to avoid over charging             *
 * Author: Olivier Staquet                                                    *
 * Last version available on https://github.com/ostaquet/M5Snake              *
 *****************************************************************************/
#include "Power.h"

/**
 * Initialize
 */
void PowerClass::begin() {
    // Initialize the power management module (based on IP5306 chip)
    M5.Power.begin();

    // Check if the control can be operated
    canControl = M5.Power.canControl();
}

/**
 * Adapt charging mode to avoid excessive charging
 */
void PowerClass::adaptChargeMode() {
    // If power management not available, ignore the routine
    if (!canControl) {
        return;
    }

    // Disable the charging if the battery is fully charged
    if (M5.Power.isChargeFull()) {
        M5.Power.setCharge(false);
    } else {
        M5.Power.setCharge(true);
    }

    // Define the shutdown time at 64s
    M5.Power.setLowPowerShutdownTime(M5.Power.ShutdownTime::SHUTDOWN_64S);
}

/**
 * Return battery level (0-100%)
 * (-1 if cannot communicate with the controller)
 */
int8_t PowerClass::getBatteryLevel() {
    return M5.Power.getBatteryLevel();
}

PowerClass Power;
