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

  class POWER {
    public:
      POWER();
      bool canControl();
      void begin();

      // -- SleepParam
      enum SleepTime
      {
        SLEEP_8S = 0,
        SLEEP_16S,
        SLEEP_32S,
        SLEEP_64S
      };
      
      // -- control for power
      bool setKeepLightLoad(bool en);
      bool setPowerBoostKeepOn(bool en);
      bool setAutoBootOnLoad(bool en);
      bool setLowPowerShutdown(bool en);
      bool setPowerBoostOnOff(bool en);
      bool setPowerBoostSet(bool en);
      bool setPowerVin(bool en);
      bool setPowerWLEDSet(bool en);
      bool setPowerBtnEn(bool en);
      bool setLowCurrentShutdownTime(SleepTime time);

      // -- control for battery
      bool setCharge(bool en);
      bool isChargeFull();
      bool isCharging();
      int8_t getBatteryLevel();
      bool batteryMode(bool en);

      // -- configuration for wakeup
      void setWakeupButton(uint8_t button);

      // -- get resson for startup
      bool isResetbyWatchdog();
      bool isResetbyDeepsleep();
      bool isResetbySoftware();
      bool isResetbyPowerSW();

      // -- sleep
      void deepSleep(uint64_t time_in_us=0);
      void lightSleep(uint64_t time_in_us=0);

      // -- power off
      void powerOFF();
      
      // -- software reset
      void reset();

    private:
      uint8_t _wakeupPin;
  };
#endif
