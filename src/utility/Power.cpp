/*----------------------------------------------------------------------*
 * M5Stack Bettery/Power Control Library v1.0                           *
 *                                                                      *
 * This work is licensed under the GNU Lesser General Public            *
 * License v2.1                                                         *
 * https://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html           *
 *----------------------------------------------------------------------*/
#include "Power.h"
#include "../M5Stack.h"
#include <rom/rtc.h>
#include <esp_sleep.h>

// ================ Power IC IP5306 ===================
#define IP5306_ADDR           (117) // 0x75
#define IP5306_REG_SYS_CTL0   (0x00)
#define IP5306_REG_READ0      (0x70)
#define IP5306_REG_READ1      (0x71)
#define IP5306_REG_READ3      (0x78)
#define CHARGE_FULL_BIT       (0x08)
#define BOOST_OUT_BIT         (0x02)
#define BOOT_ON_LOAD_BIT      (0x04)
#define LIGHT_LOAD_BIT        (0x20)
#define CHARGE_OUT_BIT        (0x10)
#define BOOST_ENABLE_BIT      (0x80)

extern M5Stack M5;

POWER::POWER()
{
}

static bool getI2CReg(uint8_t *result, uint8_t address, uint8_t reg)
{
    Wire.beginTransmission(address);
    Wire.write(reg);
    if (Wire.endTransmission(false) == 0 && Wire.requestFrom(address, (uint8_t)1)) {
        *result = Wire.read();
        return true;
    }
    return false;
}

static bool setI2CReg(uint8_t address, uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(address);
    Wire.write(reg);
    Wire.write(value);
    return (Wire.endTransmission() == 0);
}

/*
    default: false
    false: when the current is too small, ip5306 will automatically shut down
*/
bool POWER::setKeepLightLoad(bool en) {
    uint8_t data;
    if (getI2CReg(&data, IP5306_ADDR, IP5306_REG_SYS_CTL0)) {
        return setI2CReg(IP5306_ADDR, IP5306_REG_SYS_CTL0
                        ,!en ? (data |  LIGHT_LOAD_BIT)
                             : (data &(~LIGHT_LOAD_BIT)));
    }
    return false;
}

// true: Output normally open
bool POWER::setPowerBoostKeepOn(bool en) {
    uint8_t data;
    if (getI2CReg(&data, IP5306_ADDR, IP5306_REG_SYS_CTL0)) {
        return setI2CReg(IP5306_ADDR, IP5306_REG_SYS_CTL0
                        ,en ? (data |  BOOST_OUT_BIT)
                            : (data &(~BOOST_OUT_BIT)));
    }
    return false;
}

/*
    default: true
    true: If enough load is connected, the power will turn on.
*/
bool POWER::setAutoBootOnLoad(bool en) {
    uint8_t data;
    if (getI2CReg(&data, IP5306_ADDR, IP5306_REG_SYS_CTL0)) {
        return setI2CReg(IP5306_ADDR, IP5306_REG_SYS_CTL0
                        ,en ? (data |  BOOT_ON_LOAD_BIT)
                            : (data &(~BOOT_ON_LOAD_BIT)));
    }
    return false;
}

// if charge full,try set charge enable->disable->enable,can be recharged
bool POWER::setCharge(bool en){
    uint8_t data;
    if (getI2CReg(&data, IP5306_ADDR, IP5306_REG_SYS_CTL0)) {
        return setI2CReg(IP5306_ADDR, IP5306_REG_SYS_CTL0
                        ,en ? (data |  CHARGE_OUT_BIT)
                            : (data &(~CHARGE_OUT_BIT)));
    }
    return false;
}

// full return true, else return false
bool POWER::isChargeFull(){
    uint8_t data;
    return (getI2CReg(&data, IP5306_ADDR, IP5306_REG_READ1))
         ? (data & CHARGE_FULL_BIT)
         : false;
}

// test if ip5306 is an i2c version
bool POWER::canControl(){
    Wire.beginTransmission(IP5306_ADDR);
    Wire.write(IP5306_REG_READ0);
    return (Wire.endTransmission()==0);
}

//true:charge, false:discharge
bool POWER::isCharging(){
    uint8_t data;
    return (getI2CReg(&data, IP5306_ADDR, IP5306_REG_READ0))
         ? (data & CHARGE_FULL_BIT)
         : false;
}

// Return percentage * 100
int8_t POWER::getBatteryLevel() {
    uint8_t data;
    if (getI2CReg(&data, IP5306_ADDR, IP5306_REG_READ3)) {
        switch (data & 0xF0) {
        case 0x00: return 100;
        case 0x80: return 75;
        case 0xC0: return 50;
        case 0xE0: return 25;
        default:   return 0;
        }
    }
    return -1;
}

void POWER::setWakeupButton(uint8_t button) {
    _wakeupPin = button;
}

void POWER::reset() {

    esp_restart();
}

bool POWER::isResetbySoftware()
{
    RESET_REASON reset_reason=rtc_get_reset_reason(0);
    return( reset_reason==SW_RESET || 
            reset_reason==SW_CPU_RESET);
}

bool POWER::isResetbyWatchdog()
{
    RESET_REASON reset_reason=rtc_get_reset_reason(0);
    return( reset_reason==TG0WDT_SYS_RESET || 
            reset_reason==TG1WDT_SYS_RESET ||
            reset_reason==OWDT_RESET       ||            
            reset_reason==RTCWDT_CPU_RESET ||
            reset_reason==RTCWDT_RTC_RESET ||
            reset_reason==TGWDT_CPU_RESET );
}

bool POWER::isResetbyDeepsleep()
{
    RESET_REASON reset_reason=rtc_get_reset_reason(0);
    return( reset_reason==DEEPSLEEP_RESET);
}

bool POWER::isResetbyPowerSW()
{
    RESET_REASON reset_reason=rtc_get_reset_reason(0);
    return(reset_reason==POWERON_RESET);
}
/* caution: if use off()function on running battery ,
   M5stack can't restart from Power switch.
   (Switch pull up power has down by hard power down)*/
//bool POWER::batteryMode(bool en){
//
//    uint8_t data;
//    Wire.beginTransmission(IP5306_ADDR);
//    Wire.write(IP5306_REG_READ1);
//    Wire.endTransmission();
//
//    if(Wire.requestFrom(IP5306_ADDR, 1))
//    {
//        data = Wire.read();
//
//        Wire.beginTransmission(IP5306_ADDR);
//        Wire.write(IP5306_REG_READ1);
//        if (en) Wire.write(data |  BOOST_ENABLE_BIT);
//        else Wire.write(data &(~BOOST_ENABLE_BIT));
//        Wire.endTransmission();
//        return true;
//    }
//    return false;
//}

void POWER::deepSleep(uint64_t time_in_us){

    #ifdef M5STACK_FIRE
    // Keep power keep boost on
    setPowerBoostKeepOn(true);
    #endif

    // power off the Lcd
    M5.Lcd.setBrightness(0);
    M5.Lcd.sleep();

    // ESP32 into deep sleep
    esp_sleep_enable_ext0_wakeup((gpio_num_t)_wakeupPin , LOW);

    while(digitalRead(_wakeupPin) == LOW) {
        delay(10);
    }

    (time_in_us==0)?esp_deep_sleep_start():esp_deep_sleep(time_in_us);
}

void POWER::lightSleep(uint64_t time_in_us){

    #ifdef M5STACK_FIRE
    // Keep power keep boost on
    setPowerBoostKeepOn(true);
    #endif

    // power off the Lcd
    M5.Lcd.setBrightness(0);
    M5.Lcd.sleep();

    // ESP32 into deep sleep
    esp_sleep_enable_ext0_wakeup((gpio_num_t)_wakeupPin , LOW);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH,ESP_PD_OPTION_AUTO);

    while(digitalRead(_wakeupPin) == LOW) {
        delay(10);
    }
    if(time_in_us>0){
        esp_sleep_enable_timer_wakeup(time_in_us);
    }
    esp_light_sleep_start();

    // power on the Lcd
    M5.Lcd.wakeup();    
    M5.Lcd.setBrightness(200);
}


