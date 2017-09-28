#ifndef _MPU9250_H_ 
#define _MPU9250_H_ 
#include <Arduino.h>
#include <Wire.h>
#include <math.h>

#define MPU9250_ADDRESS_AD0_LOW  0x68
#define MPU9250_ADDRESS_AD0_HIGH 0x69
#define MAG_ADDRESS              0x0C

#define GYRO_FULL_SCALE_250_DPS  0x00
#define GYRO_FULL_SCALE_500_DPS  0x08
#define GYRO_FULL_SCALE_1000_DPS 0x10
#define GYRO_FULL_SCALE_2000_DPS 0x18

#define ACC_FULL_SCALE_2_G       0x00
#define ACC_FULL_SCALE_4_G       0x08
#define ACC_FULL_SCALE_8_G       0x10
#define ACC_FULL_SCALE_16_G      0x18

#define AK8963_RA_HXL                   0x03
#define AK8963_RA_CNTL1                 0x0A
#define AK8963_RA_ASAX                  0x10

#define AK8963_MODE_POWERDOWN           0x0
#define AK8963_MODE_SINGLE              0x1
#define AK8963_MODE_CONTINUOUS_8HZ      0x2
#define AK8963_MODE_EXTERNAL            0x4
#define AK8963_MODE_CONTINUOUS_100HZ    0x6
#define AK8963_MODE_SELFTEST            0x8
#define AK8963_MODE_FUSEROM             0xF

class MPU9250 {
  public:
  int16_t magXOffset, magYOffset, magZOffset;

  // MPU9250(uint8_t address = (uint8_t) MPU9250_ADDRESS_AD0_HIGH):
  MPU9250(uint8_t address = (uint8_t) MPU9250_ADDRESS_AD0_LOW);
  void setWire(TwoWire *wire);

  void beginAccel();
  void accelUpdate();
  float accelX();
  float accelY();
  float accelZ();
  float accelSqrt();

  void beginMag(uint8_t mode = AK8963_MODE_CONTINUOUS_8HZ);
  void magUpdate();
  int16_t magX();
  int16_t magY();
  int16_t magZ();
  void magSetMode(uint8_t mode);
  float magHorizDirection();

  private:
  TwoWire* myWire;
  uint8_t address;
  uint8_t accelBuf[14];
  uint8_t magBuf[7];
  uint8_t magXAdjust, magYAdjust, magZAdjust;
  float accelGet(uint8_t highIndex, uint8_t lowIndex);
  int16_t magGet(uint8_t highIndex, uint8_t lowIndex);
  void magReadAdjustValues();
  void I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data);
  void I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data);
};

#endif
