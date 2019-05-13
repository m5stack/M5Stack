/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADS1100
        This code is designed to work with the ADS1100_I2CADC I2C Mini Module available from ControlEverything.com.
        https://www.controleverything.com/content/Analog-Digital-Converters?sku=ADS1100_I2CADC#tabs-0-product_tabset-2
*/
/**************************************************************************/

#if ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Wire.h>

/**************************************************************************
    I2C ADDRESS/BITS
**************************************************************************/
    #define ADS1100_DEFAULT_ADDRESS         (0x48)    // 1001 000 (ADDR = GND)

/**************************************************************************
    CONVERSION DELAY (in mS)
**************************************************************************/
    #define ADS1100_CONVERSIONDELAY         (100)

/**************************************************************************
    CONFIG REGISTER
**************************************************************************/
    #define ADS1100_REG_CONFIG_OS_MASK      (0x80)      // Conversion
    #define ADS1100_REG_CONFIG_OS_NOEFFECT  (0x00)      // Write: Bit = 0 No effect
    #define ADS1100_REG_CONFIG_OS_SINGLE    (0x80)      // Write: Bit = 1 Begin a conversion (default)
    #define ADS1100_REG_CONFIG_OS_BUSY      (0x00)      // Read: Bit = 0 Device is not performing a conversion
    #define ADS1100_REG_CONFIG_OS_NOTBUSY   (0x80)      // Read: Bit = 1 Device is busy performing a conversion

    #define ADS1100_REG_CONFIG_MODE_MASK    (0x10)      // Device operating mode
    #define ADS1100_REG_CONFIG_MODE_CONTIN  (0x00)      // Continuous conversion mode (default)
    #define ADS1100_REG_CONFIG_MODE_SINGLE  (0x10)      // Single-conversion mode

    #define ADS1100_REG_CONFIG_DR_MASK      (0x0C)      // Data rate
    #define ADS1100_REG_CONFIG_DR_128SPS    (0x00)      // 128 samples per second
    #define ADS1100_REG_CONFIG_DR_32SPS     (0x04)      // 32 samples per second
    #define ADS1100_REG_CONFIG_DR_16SPS     (0x08)      // 16 samples per second
    #define ADS1100_REG_CONFIG_DR_8SPS      (0x0C)      // 8 samples per second (default)

    #define ADS1100_REG_CONFIG_PGA_MASK     (0x03)      // Programmable gain amplifier configuration
    #define ADS1100_REG_CONFIG_PGA_1        (0x00)      // Gain 1 (default)
    #define ADS1100_REG_CONFIG_PGA_2        (0x01)      // Gain 2
    #define ADS1100_REG_CONFIG_PGA_4        (0x02)      // Gain 4
    #define ADS1100_REG_CONFIG_PGA_8        (0x03)      // Gain 8

/**************************************************************************/


typedef enum
{
    OSMODE_SINGLE   = ADS1100_REG_CONFIG_OS_SINGLE,
    OSMODE_BUSY     = ADS1100_REG_CONFIG_OS_BUSY,
    OSMODE_NOTBUSY  = ADS1100_REG_CONFIG_OS_NOTBUSY
    
} adsOSMode_t;

typedef enum
{
    MODE_CONTIN     = ADS1100_REG_CONFIG_MODE_CONTIN,
    MODE_SINGLE     = ADS1100_REG_CONFIG_MODE_SINGLE
} adsMode_t;

typedef enum
{
    RATE_1288       = ADS1100_REG_CONFIG_DR_128SPS,
    RATE_32         = ADS1100_REG_CONFIG_DR_32SPS,
    RATE_16         = ADS1100_REG_CONFIG_DR_16SPS,
    RATE_8          = ADS1100_REG_CONFIG_DR_8SPS
} adsRate_t;

typedef enum
{
    GAIN_ONE        = ADS1100_REG_CONFIG_PGA_1,
    GAIN_TWO        = ADS1100_REG_CONFIG_PGA_2,
    GAIN_FOUR       = ADS1100_REG_CONFIG_PGA_4,
    GAIN_EIGHT      = ADS1100_REG_CONFIG_PGA_8
} adsGain_t;


class ADS1100
{
    protected:
        // Instance-specific properties
        uint8_t ads_conversionDelay;
        adsOSMode_t ads_osmode;
        adsMode_t ads_mode;
        adsRate_t ads_rate;
        adsGain_t ads_gain;

    public:
        uint8_t ads_i2cAddress;
        void getAddr_ADS1100(uint8_t i2cAddress);
        void begin(void);
        int16_t  Measure_Differential();
        void setOSMode(adsOSMode_t osmode);
        adsOSMode_t getOSMode(void);
        void setMode(adsMode_t mode);
        adsMode_t getMode(void);
        void setRate(adsRate_t rate);
        adsRate_t getRate(void);
        void setGain(adsGain_t gain);
        adsGain_t getGain(void);

    private:
};