#pragma once

void calibrationGryo();
void calibrationInit();
void calibrationSaveCenterAngle(float angle);
void calibrationGet(int16_t* gyro_x_offset, int16_t* gyro_y_offset, int16_t* gyro_z_offset, float *angle_center);