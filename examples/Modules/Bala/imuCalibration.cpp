/*
 * @Author: Sorzn
 * @Date: 2020-01-06 11:00:49
 * @LastEditTime : 2020-01-06 15:15:40
 * @Description: M5Stack project
 * @FilePath: /home/sakabin/Arduino/libraries/M5Stack/examples/Modules/Bala/imuAuto.cpp
 */


#include "imuCalibration.h"

static float gyroXOffset = 0;
static float gyroYOffset = 0;
static float gyroZOffset = 0;

static float accX = 0;
static float accY = 0;
static float accZ = 0;

static float gyroX = 0;
static float gyroY = 0;
static float gyroZ = 0;

static float angleAccX = 0;
static float angleGyroX = 0;
static float angleX = 0;

static uint32_t preInterval = 0;

void imu_CalcInit() {
  M5.IMU.Init();
  M5.IMU.setGyroFsr(M5.IMU.GFS_1000DPS);
}

void imu_setOffsetX(float x) {
  gyroXOffset = x;
}

float imu_getOffsetX() {
  return gyroXOffset;
}

void imu_calcGyroOffsets() {
  float x = 0, y = 0, z = 0;
  float x_total = 0, y_total = 0, z_total = 0;
  
  for(int i = 0; i < 3000; i++) {
    M5.IMU.getGyroData(&x, &y, &z);
    x_total += x;
  }
  
  gyroXOffset = x_total / 3000;
}

void imu_update() {
  float interval;
  if(preInterval == 0) preInterval = millis();
  
  M5.IMU.getGyroData(&gyroX, &gyroY, &gyroZ);
  M5.IMU.getAccelData(&accX, &accY, &accZ);

  angleAccX = atan2(accY, sqrt(accZ * accZ + accX * accX)) * 360 / 2.0 / PI;

  gyroX -= gyroXOffset;

  interval = (millis() - preInterval) * 0.001;
  
  preInterval = millis();

  angleX = (0.98 * (angleX + gyroX * interval)) + (0.02 * angleAccX);
}

float imu_getAngleX() {
  #ifdef M5STACK_MPU6886 || M5STACK_200Q:
  return 0 - angleX;
  #else:
  return angleX;
  #endif
}
