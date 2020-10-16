#define M5STACK_MPU6886 

#include "imu_filter.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "M5Stack.h"
#include "MadgwickAHRS.h"


static volatile float angle;
SemaphoreHandle_t angle_lock = NULL;
static void ImuUpdateTask(void *arg);

int16_t gryo_x_offset;
int16_t gryo_y_offset;
int16_t gryo_z_offset;

static float acc_x_2, acc_y_2, acc_z_2;
// maybe not need
typedef struct {
  float a1;
  float a2;

  float b0;
  float b1;
  float b2;

  float y1;
  float y2;

  float x1;
  float x2;

} Butter_t;

float ButterFilter2(Butter_t* butter, float input) {
  float output;
  output = butter->b0 * input - butter->a2 * butter->y2 + butter->b1 * butter->x1 + butter->b2 * butter->x2 - butter->a1 * butter->y1 ;
  butter->y2 = butter->y1;
  butter->y1 = output;
  butter->x2 = butter->x1;
  butter->x1 = input;
  return output;
}

float getAngle() {
  float angle_out;
  
  if (angle_lock == NULL) {
    return 0;
  }

  xSemaphoreTake(angle_lock, portMAX_DELAY);
  angle_out = angle;
  xSemaphoreGive(angle_lock);
  return angle_out;
}

void ImuTaskStart(int16_t x_offset, int16_t y_offset, int16_t z_offset, SemaphoreHandle_t *i2c_lock) {
  gryo_x_offset = 0 - x_offset;
  gryo_y_offset = 0 - y_offset;
  gryo_z_offset = 0 - z_offset;
  xTaskCreatePinnedToCore(ImuUpdateTask, "imu_task", 4 * 1024, i2c_lock, 5, NULL, 1);
}

void ImuUpdateTask(void *arg) {
  uint32_t last_ticks = xTaskGetTickCount();

  uint8_t* fifo_buff = NULL;
  uint16_t fifo_count = 0;
  uint16_t data_number = 0;
  fifo_buff = (uint8_t *)malloc(sizeof(uint8_t) * 1024);

  float acc_x = 0, acc_y = 0, acc_z = 0; 
  float gyro_x, gyro_y, gyro_z;
  float yaw_ahrs, pitch_ahrs, roll_ahrs;

  ImuData_t* imu_data;
  SemaphoreHandle_t i2c_lock = *(SemaphoreHandle_t*)arg;
  angle_lock = xSemaphoreCreateRecursiveMutex();

  bool fast_to_normal_angle = true;

  Butter_t butter_acc_x;
  Butter_t butter_acc_y;
  Butter_t butter_acc_z;

  memset(&butter_acc_x, 0, sizeof(Butter_t));
  memset(&butter_acc_y, 0, sizeof(Butter_t));
  memset(&butter_acc_z, 0, sizeof(Butter_t));

  // 500hz in, filter 50hz, low pass, butterworth
  butter_acc_x.a1 = -1.142980f;
  butter_acc_x.a2 = 0.412801f;
  butter_acc_x.b0 = 0.067455f;
  butter_acc_x.b1 = 0.134910f;
  butter_acc_x.b2 = 0.067455f;

  memcpy(&butter_acc_y, &butter_acc_x, sizeof(Butter_t));
  memcpy(&butter_acc_z, &butter_acc_x, sizeof(Butter_t));

  // Read from fifo to ensure that data will not be lost
  // IMU data freq is 500HZ
  xSemaphoreTake(i2c_lock, portMAX_DELAY);
  M5.IMU.setFIFOEnable(true);
  M5.IMU.RestFIFO();
  xSemaphoreGive(i2c_lock);

  // make sure angle fast to normal
  MadgwickAHRSetBeta(10);

  for(;;) {
    xSemaphoreTake(i2c_lock, portMAX_DELAY);
    fifo_count = M5.IMU.ReadFIFOCount();
    M5.IMU.ReadFIFOBuff(fifo_buff, fifo_count);
    xSemaphoreGive(i2c_lock);

    imu_data = (ImuData_t *)fifo_buff;
    data_number = fifo_count / 14;

    for(uint8_t i = 0; i < data_number; i++) {
      acc_x = ACC_8G_RES * (int16_t)(imu_data[i].value.acc_x_l | (imu_data[i].value.acc_x_h << 8));
      acc_y = ACC_8G_RES * (int16_t)(imu_data[i].value.acc_y_l | (imu_data[i].value.acc_y_h << 8));
      acc_z = ACC_8G_RES * (int16_t)(imu_data[i].value.acc_z_l | (imu_data[i].value.acc_z_h << 8));
      gyro_x = GYRO_2000DPS_RES * ((int16_t)(imu_data[i].value.gyro_x_l | (imu_data[i].value.gyro_x_h << 8)) + gryo_x_offset);
      gyro_y = GYRO_2000DPS_RES * ((int16_t)(imu_data[i].value.gyro_y_l | (imu_data[i].value.gyro_y_h << 8)) + gryo_y_offset);
      gyro_z = GYRO_2000DPS_RES * ((int16_t)(imu_data[i].value.gyro_z_l | (imu_data[i].value.gyro_z_h << 8)) + gryo_z_offset);
      
      // Don't know if this is necessary
      acc_x_2 = ButterFilter2(&butter_acc_x, acc_x);
      acc_y_2 = ButterFilter2(&butter_acc_y, acc_y);
      acc_z_2 = ButterFilter2(&butter_acc_z, acc_z);
      MadgwickAHRSupdateIMU(gyro_x * DEG_TO_RAD, gyro_y * DEG_TO_RAD, gyro_z * DEG_TO_RAD, acc_x_2, acc_y_2, acc_z_2, &pitch_ahrs, &roll_ahrs, &yaw_ahrs);
      
      // MadgwickAHRSupdateIMU(gyro_x * DEG_TO_RAD, gyro_y * DEG_TO_RAD, gyro_z * DEG_TO_RAD, acc_x, acc_y, acc_z, &pitch_ahrs, &roll_ahrs, &yaw_ahrs);
    }

    if (fast_to_normal_angle) {
      int16_t roll = atan2(acc_y, sqrt(acc_z * acc_z + acc_x * acc_x)) * 360.0 / 2.0 / PI;
      
      if(fabs(roll - roll_ahrs) < 1 && roll != roll_ahrs) {
        fast_to_normal_angle = false;
        MadgwickAHRSetBeta(0.1);
      }
    }

    xSemaphoreTake(angle_lock, portMAX_DELAY);
    if (!fast_to_normal_angle) {
      angle = roll_ahrs; 
    }
    xSemaphoreGive(angle_lock);
    vTaskDelayUntil(&last_ticks, pdMS_TO_TICKS(5));
  }
}
