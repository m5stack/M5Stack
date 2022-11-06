/*
    Description: Provide three LEGO motor drive modes。Press button B to switch
   the mode, button A and C control parameter value increase and decrease
*/

#include <M5Stack.h>

#include "BaseX.h"

BASE_X base_x          = BASE_X();
int32_t position_point = 0;
int8_t mode            = NORMAL_MODE;
uint8_t max_speed      = 20;
int8_t speed_point     = 0;

void setup() {
    M5.begin(true, false, true, true);
    M5.Power.begin();
    for (uint8_t i = 1; i < 5; i++) {
        base_x.SetMode(i, NORMAL_MODE);
    }
}

void NormalMode() {
    if (M5.BtnA.wasPressed()) {
        for (size_t i = 1; i < 5; i++) {
            base_x.SetMotorSpeed(i, 125);
        }
    }

    if (M5.BtnC.wasPressed()) {
        for (size_t i = 1; i < 5; i++) {
            base_x.SetMotorSpeed(i, -125);
        }
    }

    if (M5.BtnB.wasPressed()) {
        for (size_t i = 1; i < 5; i++) {
            mode = POSITION_MODE;
            base_x.SetMotorSpeed(i, 0);
            base_x.SetEncoderValue(i, 0);
            base_x.SetPostionPIDMaxSpeed(i, max_speed);
            base_x.SetMode(i, mode);
        }
    }
}

void PositionMode() {
    if (M5.BtnA.wasPressed()) {
        position_point += 200;
        for (size_t i = 1; i < 5; i++) {
            base_x.SetPositionPoint(i, position_point);
        }
    }

    if (M5.BtnB.wasPressed()) {
        for (size_t i = 1; i < 5; i++) {
            mode = SPEED_MODE;
            base_x.SetMotorSpeed(i, 0);
            base_x.SetEncoderValue(i, 0);
            base_x.SetMode(i, mode);
        }
    }

    if (M5.BtnC.wasPressed()) {
        max_speed += 20;

        if (max_speed > 120) {
            max_speed = 20;
        }

        for (size_t i = 1; i < 5; i++) {
            base_x.SetPostionPIDMaxSpeed(i, max_speed);
        }
    }
}

void SpeedMode() {
    if (M5.BtnA.wasPressed()) {
        speed_point += 2;
        for (size_t i = 1; i < 5; i++) {
            base_x.SetSpeedPoint(i, speed_point);
        }
    }

    if (M5.BtnB.wasPressed()) {
        for (size_t i = 1; i < 5; i++) {
            mode = NORMAL_MODE;
            base_x.SetMotorSpeed(i, 0);
            base_x.SetEncoderValue(i, 0);
            base_x.SetMode(i, mode);
        }
    }

    if (M5.BtnC.wasPressed()) {
        speed_point -= 2;

        for (size_t i = 1; i < 5; i++) {
            base_x.SetSpeedPoint(i, speed_point);
        }
    }
}

void loop() {
    int32_t encoder[4];
    for (size_t i = 1; i < 5; i++) {
        encoder[i - 1] = base_x.GetEncoderValue(i);
    }

    M5.update();
    Serial.printf("encoder: 1: %d, 2: %d, 3: %d, 4: %d\r\n", encoder[0],
                  encoder[1], encoder[2], encoder[3]);
    if (mode == NORMAL_MODE) {
        NormalMode();
    } else if (mode == POSITION_MODE) {
        PositionMode();
    } else if (mode == SPEED_MODE) {
        SpeedMode();
    }

    delay(10);
}