/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: Basic + Unit DAC2(GP8413)
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5GFX: https://github.com/m5stack/M5GFX
 * M5Unified: https://github.com/m5stack/M5Unified
 * DFRobot_GP8XXX: https://github.com/DFRobot/DFRobot_GP8XXX
 */

#include <M5GFX.h>
#include <M5Unified.h>
#include <DFRobot_GP8XXX.h>

DFRobot_GP8XXX_IIC GP8413(RESOLUTION_15_BIT, 0x59, &Wire);

// range is 0~10000mv
void setDacVoltage(uint16_t vol, uint8_t ch)
{
    uint16_t setting_vol = 0;
    if (vol > 10000) {
        vol = 10000;
    }
    if (ch > 1) ch = 1;
    setting_vol = (int16_t)((float)vol / 10000.0f * 32767.0f);
    if (setting_vol > 32767) {
        setting_vol = 32767;
    }
    GP8413.setDACOutVoltage(setting_vol, ch);
}

void AllOutputCtl(uint16_t vol)
{
    M5.Display.fillRect(0, 0, M5.Display.width(), 30, vol > 0 ? GREEN : ORANGE);
    M5.Display.drawString("OUTPUT " + String(vol) + "mv", M5.Display.width() / 2, 0);
    // set channel0
    setDacVoltage(vol, 0);
    // set channel1
    setDacVoltage(vol, 1);
}

void setup(void)
{
    auto cfg = M5.config();

    M5.begin(cfg);
    M5.Display.setRotation(1);
    M5.Display.setTextDatum(top_center);
    M5.Display.setTextColor(WHITE);
    M5.Display.setFont(&fonts::FreeSansBoldOblique12pt7b);
    M5.Display.setTextSize(1);
    M5.Display.drawString("DAC2", M5.Display.width() / 2, M5.Display.height() / 2 - 20);
    Wire.end();
    Wire.begin(21, 22);

    while (GP8413.begin() != 0) {
        Serial.println("Init Fail!");
        M5.Display.drawString("Init Fail!", M5.Display.width() / 2, M5.Display.height() / 2);
        delay(1000);
    }
    M5.Display.clear();
    M5.Display.drawString("DAC2", M5.Display.width() / 2, M5.Display.height() / 2 - 20);
    GP8413.setDACOutRange(GP8413.eOutputRange10V);
    M5.Display.drawString("BtnA En/Dis Output", M5.Display.width() / 2, M5.Display.height() / 2 + 20);

    AllOutputCtl(0);
}

bool output = false;

void loop(void)
{
    M5.update();
    if (M5.BtnA.wasClicked()) {
        output = !output;
        if (output) {
            AllOutputCtl(3300);
            // AllOutputCtl(10000);
        } else {
            AllOutputCtl(0);
        }
    }
}
