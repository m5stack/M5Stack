/*
    Description:
    This case uses the ODrive module to control the high-speed and precise
   rotation of the servo motor Press button C to calibrate (do not touch the
   motor shaft during this period), long press and short button A to control the
   motor rotation. Note: The motor parameter configuration in this case is only
   applicable to the motor model matched with the M5 Odrive kit. When driving
   other types of motors, please configure the parameters according to the motor
   used.
*/

#include "M5Stack.h"
#include "odrive.h"

ODrive odrive(Serial1);
TFT_eSprite canvas = TFT_eSprite(&M5.Lcd);

void showStringCenter(const char* str, uint16_t color) {
    canvas.fillScreen(TFT_BLACK);
    canvas.setTextColor(color);
    canvas.drawString(str, 160, 120, 4);
    canvas.pushSprite(0, 0);
}

void setup() {
    M5.begin(true, false, true, true);
    canvas.setColorDepth(1);
    canvas.createSprite(320, 240);
    Serial1.begin(115200, SERIAL_8N1, 13, 5);

    canvas.setTextDatum(MC_DATUM);
    showStringCenter("ODrive", TFT_GREEN);
}

void loop() {
    M5.update();
    if (M5.BtnA.wasReleased()) {
        odrive.setPosition(10);
    }

    if (M5.BtnA.wasReleasefor(800)) {
        odrive.setPosition(0);
    }

    if (M5.BtnB.wasReleased()) {
        showStringCenter("Save default config", TFT_GREEN);
        odrive.setDefaultConfig();
        odrive.reboot();
    }

    if (M5.BtnB.wasReleasefor(800)) {
        showStringCenter("Clear config", TFT_GREEN);
        odrive.eraseConfig();
    }

    if (M5.BtnC.wasReleased()) {
        showStringCenter("Motor CALIBRATION", TFT_WHITE);
        odrive.runState(odrive.AXIS_STATE_MOTOR_CALIBRATION, 10000);
        if (odrive.checkError()) {
            showStringCenter("Motor CALIBRATION", TFT_RED);
            return;
        }

        showStringCenter("Encoder CALIBRATION", TFT_WHITE);
        odrive.runState(odrive.AXIS_STATE_ENCODER_INDEX_SEARCH, 10000);
        if (odrive.checkError()) {
            showStringCenter("Encoder Failed", TFT_RED);
            return;
        }

        showStringCenter("Offset CALIBRATION", TFT_WHITE);
        odrive.runState(odrive.AXIS_STATE_ENCODER_OFFSET_CALIBRATION, 10000);
        if (odrive.checkError()) {
            showStringCenter("Offset Failed", TFT_RED);
            return;
        }

        odrive.setGain(211.0, 0.0225, 0.01125);
        odrive.setControlMode(odrive.CONTROL_MODE_POSITION_CONTROL);
        odrive.runState(odrive.AXIS_STATE_CLOSED_LOOP_CONTROL, 2000);

        showStringCenter("Finish", TFT_GREEN);
    }

    char data_show[100];
    sprintf(data_show, "%f, %d, %d\r\n", odrive.getVbusVoltage(),
            odrive.getEncoderShadowCount(), odrive.checkError());
    showStringCenter(data_show, TFT_WHITE);
    Serial.printf(data_show);
    delay(20);
}
