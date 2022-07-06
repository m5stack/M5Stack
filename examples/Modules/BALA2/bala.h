#ifndef _BALA_H__
#define _BALA_H__

#include "Arduino.h"
#include "freertos/FreeRTOS.h"

class Bala {
   public:
    int32_t wheel_left_encoder, wheel_right_encoder;

   public:
    Bala();
    void ClearEncoder();
    void GetEncoder(int32_t* wheel_left, int32_t* wheel_right);
    void SetEncoder(int32_t wheel_left, int32_t wheel_right);
    void UpdateEncoder();

    // -1023 ~ 1024
    void SetSpeed(int16_t wheel_left, int16_t wheel_right);

    // 500 ~ 2500 -> 0 ~ 180
    void SetServoAngle(uint8_t pos, uint8_t angle);

    // 500 ~ 2500
    void SetServoPulse(uint8_t pos, uint16_t width);

    // used i2c lock
    void SetMutex(SemaphoreHandle_t* mutex);

   private:
    SemaphoreHandle_t i2c_mutex;
};
#endif