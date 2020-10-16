#ifndef __IMU__H_
#define __IMU__H_

#include "Arduino.h"

#define ACC_8G_RES (0.000244140625f)
#define GYRO_2000DPS_RES (0.06103515625f)

typedef union {
  uint8_t raw[14];
  struct {
    uint8_t acc_x_h;
    uint8_t acc_x_l;
    uint8_t acc_y_h;
    uint8_t acc_y_l;
    uint8_t acc_z_h;
    uint8_t acc_z_l;
    uint8_t temp_h;
    uint8_t temp_l;
    uint8_t gyro_x_h;
    uint8_t gyro_x_l;
    uint8_t gyro_y_h;
    uint8_t gyro_y_l;
    uint8_t gyro_z_h;
    uint8_t gyro_z_l;
  } value;
} ImuData_t;


float getAngle();
void ImuTaskStart(int16_t x_offset, int16_t y_offset, int16_t z_offset, SemaphoreHandle_t* i2c_lock);

#endif