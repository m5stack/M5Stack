/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit Gesture
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5GFX@^0.2.3: https://github.com/m5stack/M5GFX
 * M5Unified@^0.2.2: https://github.com/m5stack/M5Unified
 * DFRobot_PAJ7620U2：https://github.com/ouki-wang/DFRobot_PAJ7620U2
 */

#include <M5Unified.h>
#include <M5GFX.h>
#include <DFRobot_PAJ7620U2.h>

M5GFX display;
M5Canvas canvas(&display);
DFRobot_PAJ7620U2 sensor;

void log(String info)
{
    Serial.println(info);
    canvas.println(info);
    canvas.pushSprite(0, 0);
}

void setup()
{
    M5.begin();
    display.begin();

    canvas.setColorDepth(1);  // mono color
    canvas.createSprite(display.width(), display.height());
    canvas.setPaletteColor(1, ORANGE);
    canvas.setTextSize((float)canvas.width() / 160);
    canvas.setTextScroll(true);

    Serial.begin(115200);
    delay(300);
    log("PAJ7620U2 Init");
    while (sensor.begin() != 0) {
        log("initial PAJ7620U2 failure!");
        delay(500);
    }
    sensor.setGestureHighRate(true);
    log("Start to recognize");
}

void loop()
{
    /*
     * eGestureNone  eGestureRight  eGestureLeft  eGestureUp  eGestureDown
     * eGestureForward eGestureBackward  eGestureClockwise eGestureAntiClockwise
     * eGestureWave  eGestureWaveSlowlyDisorder eGestureWaveSlowlyLeftRight
     * eGestureWaveSlowlyUpDown  eGestureWaveSlowlyForwardBackward
     */
    DFRobot_PAJ7620U2::eGesture_t gesture = sensor.getGesture();
    if (gesture != sensor.eGestureNone) {
        /*
         * "None","Right","Left", "Up", "Down", "Forward", "Backward",
         * "Clockwise", "Anti-Clockwise", "Wave", "WaveSlowlyDisorder",
         * "WaveSlowlyLeftRight", "WaveSlowlyUpDown",
         * "WaveSlowlyForwardBackward"
         */
        String description = sensor.gestureDescription(gesture);
        log("Gesture = " + description);
    }
}