/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/gesture
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/gesture
*
* Product: GESTURE.  手势传感器
* Date: 2022/8/17
*******************************************************************************
  Please connect to Port A (G21, G22)
  请连接端口A (G21, G22)

  编译前请安装依赖库
  Please install dependent libraries before compiling:
  https://github.com/ouki-wang/DFRobot_PAJ7620U2
*/

#include <M5Stack.h>
#include <M5GFX.h>
#include <DFRobot_PAJ7620U2.h>

M5GFX display;
M5Canvas canvas(&display);

DFRobot_PAJ7620U2 sensor;

void log(String info) {
    Serial.println(info);
    canvas.println(info);
    canvas.pushSprite(0, 0);
}

void setup() {
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

void loop() {
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