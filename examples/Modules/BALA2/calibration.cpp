#define M5STACK_MPU6886
#include <Preferences.h>

#include "M5Stack.h"

Preferences preferences;

void GetGyroOffset(uint16_t times, int16_t* x_offset, int16_t* y_offset,
                   int16_t* z_offset) {
    int64_t x = 0, y = 0, z = 0;
    int16_t gyro_x, gyro_y, gyro_z;
    for (size_t i = 0; i < times; i++) {
        M5.IMU.getGyroAdc(&gyro_x, &gyro_y, &gyro_z);
        x += gyro_x;
        y += gyro_y;
        z += gyro_z;
    }

    *x_offset = x / times;
    *y_offset = y / times;
    *z_offset = z / times;
}

void calibrationInit() {
    preferences.begin("Bala2Cal", false);
}

void calibrationGet(int16_t* gyro_x_offset, int16_t* gyro_y_offset,
                    int16_t* gyro_z_offset, float* angle_center) {
    *gyro_x_offset = preferences.getInt("gryo_x", 0);
    *gyro_y_offset = preferences.getInt("gryo_y", 0);
    *gyro_z_offset = preferences.getInt("gryo_z", 0);

    *angle_center = preferences.getFloat("angle", 0.0);
}

void calibrationGryo() {
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextFont(2);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.printf("Start gryo calibration\r\n");
    delay(1000);

    M5.Lcd.printf("Please keep BALA2 still for 2 seconds\r\n");

    int16_t x_offset = 0;
    int16_t y_offset = 0;
    int16_t z_offset = 0;
    GetGyroOffset(2000, &x_offset, &y_offset, &z_offset);
    M5.Lcd.printf("Finish calibration !!!\r\n");

    preferences.putInt("gryo_x", x_offset);
    preferences.putInt("gryo_y", y_offset);
    preferences.putInt("gryo_z", z_offset);

    delay(1000);
}

void calibrationSaveCenterAngle(float angle) {
    preferences.putFloat("angle", angle);
}