#include "bala.h"

#include "M5Stack.h"

Bala::Bala() {
    wheel_left_encoder  = 0;
    wheel_right_encoder = 0;
    i2c_mutex           = NULL;
}

void Bala::ClearEncoder() {
    SetEncoder(0, 0);
    wheel_left_encoder  = 0;
    wheel_right_encoder = 0;
}

void Bala::GetEncoder(int32_t* wheel_left, int32_t* wheel_right) {
    UpdateEncoder();
    *wheel_left  = wheel_left_encoder;
    *wheel_right = wheel_right_encoder;
}

void Bala::SetEncoder(int32_t wheel_left, int32_t wheel_right) {
    uint8_t data_out[8];
    data_out[0] = (uint8_t)(wheel_left >> 24);
    data_out[1] = (uint8_t)(wheel_left >> 16);
    data_out[2] = (uint8_t)(wheel_left >> 8);
    data_out[3] = (uint8_t)(wheel_left >> 0);

    data_out[4] = (uint8_t)(wheel_right >> 24);
    data_out[5] = (uint8_t)(wheel_right >> 16);
    data_out[6] = (uint8_t)(wheel_right >> 8);
    data_out[7] = (uint8_t)(wheel_right >> 0);
    if (i2c_mutex != NULL) {
        xSemaphoreTake(i2c_mutex, portMAX_DELAY);
    }
    M5.I2C.writeBytes(0x3A, 0x10, data_out, 8);
    if (i2c_mutex != NULL) {
        xSemaphoreGive(i2c_mutex);
    }
}

void Bala::UpdateEncoder() {
    uint8_t data_in[8];
    if (i2c_mutex != NULL) {
        xSemaphoreTake(i2c_mutex, portMAX_DELAY);
    }
    M5.I2C.readBytes(0x3A, 0x10, 8, data_in);
    if (i2c_mutex != NULL) {
        xSemaphoreGive(i2c_mutex);
    }

    wheel_left_encoder = (data_in[0] << 24) | (data_in[1] << 16) |
                         (data_in[2] << 8) | data_in[3];
    wheel_right_encoder = (data_in[4] << 24) | (data_in[5] << 16) |
                          (data_in[6] << 8) | data_in[7];
}

void Bala::SetSpeed(int16_t wheel_left, int16_t wheel_right) {
    uint8_t data_out[4];

    data_out[0] = (int8_t)(wheel_left >> 8);
    data_out[1] = (int8_t)(wheel_left >> 0);
    data_out[2] = (int8_t)(wheel_right >> 8);
    data_out[3] = (int8_t)(wheel_right >> 0);

    if (i2c_mutex != NULL) {
        xSemaphoreTake(i2c_mutex, portMAX_DELAY);
    }
    M5.I2C.writeBytes(0x3A, 0x00, data_out, 4);
    if (i2c_mutex != NULL) {
        xSemaphoreGive(i2c_mutex);
    }
}

void Bala::SetMutex(SemaphoreHandle_t* mutex) {
    i2c_mutex = *mutex;
}

void Bala::SetServoAngle(uint8_t pos, uint8_t angle) {
    if (pos < 1) {
        pos = 1;
    } else if (pos > 8) {
        pos = 8;
    }

    pos = pos - 1;

    if (i2c_mutex != NULL) {
        xSemaphoreTake(i2c_mutex, portMAX_DELAY);
    }
    M5.I2C.writeBytes(0x3A, 0x20 | pos, &angle, 1);
    if (i2c_mutex != NULL) {
        xSemaphoreGive(i2c_mutex);
    }
}

void Bala::SetServoPulse(uint8_t pos, uint16_t width) {
    if (pos < 1) {
        pos = 1;
    } else if (pos > 8) {
        pos = 8;
    }

    pos = (pos - 1) << 1;
    uint8_t buff_out[2];
    buff_out[0] = width >> 8;
    buff_out[1] = width & 0xff;

    if (i2c_mutex != NULL) {
        xSemaphoreTake(i2c_mutex, portMAX_DELAY);
    }
    M5.I2C.writeBytes(0x3A, 0x30 | pos, buff_out, 2);
    if (i2c_mutex != NULL) {
        xSemaphoreGive(i2c_mutex);
    }
}
