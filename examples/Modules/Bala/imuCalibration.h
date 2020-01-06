/*
 * @Author: Sorzn
 * @Date: 2020-01-06 11:36:48
 * @LastEditTime : 2020-01-06 16:54:52
 * @Description: M5Stack project
 * @FilePath: /home/sakabin/Arduino/libraries/M5Stack/examples/Modules/Bala/imuCalibration.h
 */
// #define M5STACK_MPU6050 
#define M5STACK_MPU6886 
// #define M5STACK_200Q
#include "M5Stack.h"

extern void imu_CalcInit();
extern void imu_calcGyroOffsets();
extern void imu_update();
extern float imu_getAngleX();
extern void imu_setOffsetX(float x);
extern float imu_getOffsetX();
