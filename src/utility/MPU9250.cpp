#include "MPU9250.h"

MPU9250::MPU9250(uint8_t address):
  address(address),
  magXOffset(0),
  magYOffset(0),
  magZOffset(0) {
}

void MPU9250::I2Cread(uint8_t Address, uint8_t Register, uint8_t Nbytes, uint8_t* Data) {
  myWire->beginTransmission(Address);
  myWire->write(Register);
  myWire->endTransmission();

  myWire->requestFrom(Address, Nbytes);
  uint8_t index=0;
  while (myWire->available())
    Data[index++]=myWire->read();
}

void MPU9250::I2CwriteByte(uint8_t Address, uint8_t Register, uint8_t Data) {
  myWire->beginTransmission(Address);
  myWire->write(Register);
  myWire->write(Data);
  myWire->endTransmission();
}

void MPU9250::setWire(TwoWire* wire) {
  myWire = wire;
}

void MPU9250::beginAccel() {
  delay(40);

  I2CwriteByte(address, 27, GYRO_FULL_SCALE_2000_DPS);
  I2CwriteByte(address, 28, ACC_FULL_SCALE_16_G);
  delay(10);
}

void MPU9250::magReadAdjustValues() {
  magSetMode(AK8963_MODE_POWERDOWN);
  magSetMode(AK8963_MODE_FUSEROM);
  uint8_t buff[3];
  I2Cread(MAG_ADDRESS, AK8963_RA_ASAX, 3, buff);
  magXAdjust = buff[0];
  magYAdjust = buff[1];
  magZAdjust = buff[2];
}

void MPU9250::beginMag(uint8_t mode) {
  delay(10);

  // trun on magnetometor
  I2CwriteByte(address, 0x37, 0x02);
  delay(10);

  magReadAdjustValues();
  magSetMode(AK8963_MODE_POWERDOWN);
  magSetMode(mode);
}

void MPU9250::magSetMode(uint8_t mode) {
  I2CwriteByte(MAG_ADDRESS, AK8963_RA_CNTL1, mode);
  delay(10);
}

const float Pi = 3.14159;

float MPU9250::magHorizDirection() {
  return atan2((float) magX(), (float) magY()) * 180 / Pi;
}

void MPU9250::magUpdate() {
  I2Cread(MAG_ADDRESS, AK8963_RA_HXL, 7, magBuf);
}

int16_t MPU9250::magGet(uint8_t highIndex, uint8_t lowIndex) {
  return (((int16_t)magBuf[highIndex]) << 8) | magBuf[lowIndex];
}

uint16_t adjustMagValue(int16_t value, uint8_t adjust) {
  return (value * ((((adjust - 128) * 0.5) / 128) + 1));
}

int16_t MPU9250::magX() {
  return adjustMagValue(magGet(1, 0), magXAdjust) + magXOffset;
}

int16_t MPU9250::magY() {
  return adjustMagValue(magGet(3, 2), magYAdjust) + magYOffset;
}

int16_t MPU9250::magZ() {
  return adjustMagValue(magGet(5, 4), magZAdjust) + magZOffset;
}

void MPU9250::accelUpdate() {
  I2Cread(address, 0x3B, 14, accelBuf);
}

float MPU9250::accelGet(uint8_t highIndex, uint8_t lowIndex) {
  int16_t v = -(accelBuf[highIndex]<<8 | accelBuf[lowIndex]);
  return ((float)v) * 16.0/32768.0;
}

float MPU9250::accelX() {
  return accelGet(0, 1);
}

float MPU9250::accelY() {
  return accelGet(2, 3);
}

float MPU9250::accelZ() {
  return accelGet(4, 5);
}

float MPU9250::accelSqrt() {
  return sqrt(pow(accelGet(0, 1), 2) +
              pow(accelGet(2, 3), 2) +
              pow(accelGet(4, 5), 2));
}
