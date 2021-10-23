#pragma once

#include "Arduino.h"

#define ADS115_ADDR 0x49
#define EEPROM_ADDR 0x53

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

#define VOLTMETER_MEASURING_DIR -1

#define ADS1115_MUX_P0N1        0x00 // voltmeter only support

#define ADS1115_COMP_MODE_HYSTERESIS    0x00 // default
#define ADS1115_COMP_MODE_WINDOW        0x01

#define ADS1115_MODE_CONTINUOUS     0x00
#define ADS1115_MODE_SINGLESHOT     0x01 // default

#define VOLTMETER_PRESSURE_COEFFICIENT 0.015918958F

#define VOLTMETER_PAG_6144_CAL_ADDR   208
#define VOLTMETER_PAG_4096_CAL_ADDR   216
#define VOLTMETER_PAG_2048_CAL_ADDR   224
#define VOLTMETER_PAG_1024_CAL_ADDR   232
#define VOLTMETER_PAG_512_CAL_ADDR    240
#define VOLTMETER_PAG_256_CAL_ADDR    248

#define VOLTMETER_FILTER_NUMBER 10
typedef enum {
  PAG_6144 = ADS1115_PGA_6144,
  PAG_4096 = ADS1115_PGA_4096,
  PAG_2048 = ADS1115_PGA_2048, // default
  PAG_1024 = ADS1115_PGA_1024,
  PAG_512 = ADS1115_PGA_512,
  PAG_256 = ADS1115_PGA_256,
} voltmeterGain_t;

typedef enum {
  RATE_8 = ADS1115_RATE_8,   
  RATE_16 = ADS1115_RATE_16,  
  RATE_32 = ADS1115_RATE_32,  
  RATE_64 = ADS1115_RATE_64,  
  RATE_128 = ADS1115_RATE_128, // default
  RATE_250 = ADS1115_RATE_250, 
  RATE_475 = ADS1115_RATE_475, 
  RATE_860 = ADS1115_RATE_860, 
} voltmeterRate_t;

typedef enum {
  SINGLESHOT = ADS1115_MODE_SINGLESHOT,
  CONTINUOUS = ADS1115_MODE_CONTINUOUS,
} voltmeterMode_t;

class Voltmeter {
private:
  void i2cBegin();
  bool i2cReadBytes(uint8_t addr, uint8_t reg_addr, uint8_t* buff, uint16_t len);
  bool i2cWriteBytes(uint8_t addr, uint8_t reg_addr, uint8_t* buff, uint16_t len);
  bool i2cReadU16(uint8_t addr, uint8_t reg_addr, uint16_t* value);
  bool i2cWriteU16(uint8_t addr, uint8_t reg_addr, uint16_t value);

  float getResolution(voltmeterGain_t gain);
  uint16_t getCoverTime(voltmeterRate_t rate);
  uint8_t getPGAEEEPROMAddr(voltmeterGain_t gain);
  
  uint8_t _ads1115_addr;
  uint8_t _eeprom_addr;

public:
  voltmeterGain_t _gain;
  voltmeterRate_t _rate;
  voltmeterMode_t _mode;

  float resolution;
  uint16_t cover_time;
  int16_t adc_raw;
  float calibration_factor;


public:
  Voltmeter(uint8_t ads1115_addr=ADS115_ADDR, uint8_t eeprom_addr=EEPROM_ADDR);

  void setGain(voltmeterGain_t gain);
  void setRate(voltmeterRate_t rate);
  void setMode(voltmeterMode_t mode);

  float getVoltage(bool calibration = true);
  int16_t getConversion(uint16_t timeout = 125);
  int16_t getAdcRaw();

  bool isInConversion();
  void startSingleConversion();

  bool EEPORMWrite(uint8_t address, uint8_t* buff, uint8_t len);
  bool EEPORMRead(uint8_t address, uint8_t* buff, uint8_t len);

  void setCalibration(int8_t voltage, uint16_t actual);
  bool saveCalibration2EEPROM(voltmeterGain_t gain, int16_t hope, int16_t actual);
  bool readCalibrationFromEEPROM(voltmeterGain_t gain, int16_t* hope, int16_t* actual);
};
