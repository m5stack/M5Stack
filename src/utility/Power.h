/*----------------------------------------------------------------------*
 * M5Stack Bettery/Power Control Library v1.0                           *
 *                                                                      *
 * This work is licensed under the GNU Lesser General Public            *
 * License v2.1                                                         *
 * https://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html           *
 *----------------------------------------------------------------------*/
#ifndef Power_h
#define Power_h
#include <Arduino.h>
#include <Wire.h>

class POWER
{
 public:
    POWER();
    void begin();
    bool setCharge(bool en);
    bool setKeepLightLoad(bool en);
    bool setPowerBoostKeepOn(bool en);
    bool setAutoBootOnLoad(bool en);
    bool canControl();
    bool isChargeFull();
    bool isCharging();
    int8_t getBatteryLevel();

    void setWakeupButton(uint8_t button);
    //bool batteryMode(bool en);
    void deepSleep();
    void reset();


    bool setPowerBoostOnOff(bool en);
    bool setPowerBoostSet(bool en);
    bool setPowerVin(bool en);
    bool setPowerWLEDSet(bool en);

 private:
    uint8_t _wakeupPin;
};
#endif
