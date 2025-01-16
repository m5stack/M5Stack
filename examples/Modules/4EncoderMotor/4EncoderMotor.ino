/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Module 4EncoderMotor
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5GFX@^0.2.3: https://github.com/m5stack/M5GFX
 * M5Unified@^0.2.2: https://github.com/m5stack/M5Unified
 * M5Module4EncoderMotor: https://github.com/m5stack/M5Module-4EncoderMotor
 */

#include "M5Unified.h"
#include "M5GFX.h"
#include "M5Module4EncoderMotor.h"

M5Module4EncoderMotor driver;

#define MAX_RECORD_SIZE (256)

float amp_record[MAX_RECORD_SIZE] = {0};
uint8_t record_index              = 0;
float amp_value                   = 0.0f;

uint8_t avg_filter_level = 20;

float avg_filter(float *data, int len)
{
    float sum = 0;
    float min = data[0];
    float max = data[0];
    for (int i = 0; i < len; i++) {
        if (data[i] < min) {
            min = data[i];
        }
        if (data[i] > max) {
            max = data[i];
        }
        sum += data[i];
    }
    sum -= min;
    sum -= max;
    return sum / (len - 2);
}

void setup()
{
    M5.begin();
    M5.Display.begin();

    M5.Display.setTextColor(WHITE);
    M5.Display.setTextDatum(top_center);
    M5.Display.setFont(&fonts::FreeSansBold12pt7b);
    M5.Display.setTextSize(1);

    while (!driver.begin(&Wire, MODULE_4ENCODERMOTOR_ADDR, 21, 22)) {
        Serial.println("Driver Init faild!");
        M5.Display.drawString("Driver Init faild!", 160, 7);
        delay(1000);
    }

    Serial.println("Driver Init success!");
    M5.Display.clear();
    M5.Display.fillRect(0, 0, 320, 35, 0x27f);
    M5.Display.drawString("4Encoder Motor", 160, 7);
    M5.Display.setTextDatum(top_left);

    // motor channel 0 -3
    for (uint8_t i = 0; i < 4; i++) {
        driver.setMode(i, NORMAL_MODE);
        driver.setMotorSpeed(i, 127);
    }
    M5.Display.drawString("NORMAL MODE", 20, 40 + 35 * 5);
}

bool direction = true;
int mode       = NORMAL_MODE;

void loop()
{
    M5.update();
    for (uint8_t i = 0; i < 4; i++) {
        M5.Display.fillRect(20, 40 + 35 * i, 300, 35, BLACK);
        int32_t encoder_value = driver.getEncoderValue(i);
        M5.Display.drawString("CH" + String(i) + ": " + String(encoder_value), 20, 40 + 35 * i);
    }

    if (avg_filter_level != 0) {
        amp_record[record_index] = driver.getMotorCurrent();
        record_index++;
        if (record_index >= avg_filter_level) {
            record_index = 0;
        }
        amp_value = avg_filter(amp_record, avg_filter_level);
    }

    float voltage = driver.getAnalogInput(_8bit) / 255.0 * 3.3 / 0.16;
    float current = amp_value;

    M5.Display.fillRect(20, 40 + 35 * 4, 300, 35, BLACK);
    M5.Display.drawString("POWER: " + String(voltage) + "V/" + String(current) + "A", 20, 40 + 35 * 4);

    if (M5.BtnA.wasClicked() || (M5.Touch.getCount() && M5.Touch.getDetail(0).wasClicked())) {
        mode++;
        if (mode > SPEED_MODE) {
            mode = NORMAL_MODE;
        }
        M5.Display.fillRect(20, 40 + 35 * 5, 300, 35, BLACK);

        switch (mode) {
            case NORMAL_MODE: {
                M5.Display.drawString("NORMAL MODE", 20, 40 + 35 * 5);
                // motor channel 0 -3 NORMAL_MODE
                for (uint8_t i = 0; i < 4; i++) {
                    driver.setMode(i, NORMAL_MODE);
                    driver.setMotorSpeed(i, 127);
                }
                break;
            }
            case POSITION_MODE: {
                M5.Display.drawString("POSITION MODE", 20, 40 + 35 * 5);
                // motor channel 0 -3 POSITION_MODE

                for (uint8_t i = 0; i < 4; i++) {
                    driver.setMode(i, POSITION_MODE);
                    driver.setEncoderValue(i, 0);
                    driver.setPostionPIDMaxSpeed(i, 127);
                    driver.setPositionPoint(i, 1000);
                }
                break;
            }
            case SPEED_MODE: {
                M5.Display.drawString("SPEED MODE", 20, 40 + 35 * 5);
                // motor channel 0 -3 SPEED_MODE
                for (uint8_t i = 0; i < 4; i++) {
                    driver.setMode(i, SPEED_MODE);
                    driver.setSpeedPoint(i, 127);
                }
                break;
            }
        }
    }
}
