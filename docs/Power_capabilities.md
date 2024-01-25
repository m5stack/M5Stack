## Power management capabilities of M5Stack Core / Basic / Gray

Document v0.1

The power management capabilities are provided by the IP5306 power controller, specifically the IP5306_I2C version of the controller,
that can be accessed on the I2C bus at address 0x75.  Access is possible using the `POWER` class,
located in `Power.h` and `Power.cpp`, automatically included with `#include "M5Stack.h"`.

### Capabilities

- **Can the M5Stack Core turn itself off?** Yes, using `powerOFF()`, which shuts off the LED backlight, the WiFi radio, the
     Bluetooth stack, and then asks the chip to remove the power before putting the ESP into deep sleep mode.  If called when the device is externally powered,
     these actions (including the deep sleep mode) will give the appearance and effect of being powered off.  Pressing the power/reset button turns it back on.
- **Can it wake itself back up after a pre-determined delay or at a specific time?**
     No, since it doesn't have a real-time clock chip (RTC), but ESP32 sleep modes may provide a suitable alternative.
- **Can it tell if it is running on external versus battery power?**
- **Can it tell if the battery is being charged or discharged?**
- **Can it tell if a battery is present or absent?** *possibly, but not formally*
- **Can it tell the state of charge of the battery?** Yes, using `getBatteryLevel()`, which returns one of: 25, 50, 75, 100 (representing a percentage), or -1 (if unable to read)
- **Can it read its battery voltage?** Not possible to read the voltage level directly, only the approximate percentage of 25/50/75/100.
- **Can communication with the IP5306 power controller be tested and confirmed?** Yes, with `canControl()`
- **What other capabilities?** *(to be answered -- translation of Chinese-language datasheet is difficult to understand)*
  - *auto shutoff settings - voltage low for how long causes a shutoff*
  - *inhibit the power button, report it as an event?*
  - *setting the charging voltage and current*
  - *distinguish between a software/hardware/watchdog/poweron reset*
  - *maintain any of the settings across power cycles and/or complete loss of all power*
