#pragma once

#include "Arduino.h"

#define ADS115_ADDR 0x48
#define EEPROM_ADDR 0x51

#define ADS1115_RA_CONVERSION   0x00
#define ADS1115_RA_CONFIG       0x01

#define ADS1115_PGA_6144        0x00
#define ADS1115_PGA_4096        0x01
#define ADS1115_PGA_2048        0x02 // default
#define ADS1115_PGA_1024        0x03
#define ADS1115_PGA_512         0x04
#define ADS1115_PGA_256         0x05

#define ADS1115_MV_6144         0.187500F
#define ADS1115_MV_4096         0.125000F
#define ADS1115_MV_2048         0.062500F // default
#define ADS1115_MV_1024         0.031250F
#define ADS1115_MV_512          0.015625F
#define ADS1115_MV_256          0.007813F

#define ADS1115_RATE_8          0x00
#define ADS1115_RATE_16         0x01
#define ADS1115_RATE_32         0x02
#define ADS1115_RATE_64         0x03
#define ADS1115_RATE_128        0x04 // default
#define ADS1115_RATE_250        0x05
#define ADS1115_RATE_475        0x06
#define ADS1115_RATE_860        0x07

#define ADS1115_MUX_P0N1        0x00 // ammeter only support

#define ADS1115_COMP_MODE_HYSTERESIS    0x00 // default
#define ADS1115_COMP_MODE_WINDOW        0x01

#define ADS1115_MODE_CONTINUOUS     0x00
#define ADS1115_MODE_SINGLESHOT     0x01 // default

#define AMMETER_PRESSURE_COEFFICIENT 0.05

#define AMMETER_PAG_6144_CAL_ADDR   208
#define AMMETER_PAG_4096_CAL_ADDR   216
#define AMMETER_PAG_2048_CAL_ADDR   224
#define AMMETER_PAG_1024_CAL_ADDR   232
#define AMMETER_PAG_512_CAL_ADDR    240
#define AMMETER_PAG_256_CAL_ADDR    248

#define AMMETER_MEASURING_DIR -1

typedef enum {
  PAG_6144 = ADS1115_PGA_6144,
  PAG_4096 = ADS1115_PGA_4096,
  PAG_2048 = ADS1115_PGA_2048, // default
  PAG_1024 = ADS1115_PGA_1024,
  PAG_512 = ADS1115_PGA_512,
  PAG_256 = ADS1115_PGA_256,
} ammeterGain_t;

typedef enum {
  RATE_8 = ADS1115_RATE_8,   
  RATE_16 = ADS1115_RATE_16,  
  RATE_32 = ADS1115_RATE_32,  
  RATE_64 = ADS1115_RATE_64,  
  RATE_128 = ADS1115_RATE_128, // default
  RATE_250 = ADS1115_RATE_250, 
  RATE_475 = ADS1115_RATE_475, 
  RATE_860 = ADS1115_RATE_860, 
} ammeterRate_t;

typedef enum {
  SINGLESHOT = ADS1115_MODE_SINGLESHOT,
  CONTINUOUS = ADS1115_MODE_CONTINUOUS,
} ammeterMode_t;

class Ammeter {
private:
  void i2cBegin();
  bool i2cReadBytes(uint8_t addr, uint8_t reg_addr, uint8_t* buff, uint16_t len);
  bool i2cWriteBytes(uint8_t addr, uint8_t reg_addr, uint8_t* buff, uint16_t len);
  bool i2cReadU16(uint8_t addr, uint8_t reg_addr, uint16_t* value);
  bool i2cWriteU16(uint8_t addr, uint8_t reg_addr, uint16_t value);

  float getResolution(ammeterGain_t gain);
  uint16_t getCoverTime(ammeterRate_t rate);
  uint8_t getPGAEEEPROMAddr(ammeterGain_t gain);
  
  uint8_t _ads1115_addr;
  uint8_t _eeprom_addr;

  float pga_256_calibration_5v_value = 10187;
  float pga_2048_calibration_25v_value = 6368;

public:
  ammeterGain_t _gain;
  ammeterRate_t _rate;
  ammeterMode_t _mode;

  float resolution;
  uint16_t cover_time;
  int16_t adc_raw;
  float calibration_factor;


public:
  Ammeter(uint8_t ads1115_addr=ADS115_ADDR, uint8_t eeprom_addr=EEPROM_ADDR);

  void setGain(ammeterGain_t gain);
  void setRate(ammeterRate_t rate);
  void setMode(ammeterMode_t mode);

  float getCurrent(bool calibration = true);
  int16_t getConversion(uint16_t timeout = 125);
  int16_t getAdcRaw();

  bool isInConversion();
  void startSingleConversion();

  bool EEPORMWrite(uint8_t address, uint8_t* buff, uint8_t len);
  bool EEPORMRead(uint8_t address, uint8_t* buff, uint8_t len);

  bool saveCalibration2EEPROM(ammeterGain_t gain, int16_t hope, int16_t actual);
  bool readCalibrationFromEEPROM(ammeterGain_t gain, int16_t* hope, int16_t* actual);
};
