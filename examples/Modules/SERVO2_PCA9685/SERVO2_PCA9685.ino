/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/module/servo2
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/module/servo2
*
* Describe: servo2.
* Date: 2021/9/2
*******************************************************************************
  Use SERVO2 Module to control the rotation of 16-channel servo.
  使用 SERVO2 模块控制 16 通道舵机的旋转。
*/

#include <M5Stack.h>
#include <Wire.h>

#include "Adafruit_PWMServoDriver.h"

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40, Wire);

#define SERVOMIN \
    102  // This is the 'minimum' pulse length count (out of 4096)
         // 这是“最小”脉冲长度计数（共 4096 个）
#define SERVOMAX \
    512  // This is the 'maximum' pulse length count (out of 4096)
         // 这是“最大”脉冲长度计数（共 4096 个）
#define USMIN \
    500  // This is the rounded 'minimum' microsecond length based on the
         // minimum pulse of 102  这是基于 102 的最小脉冲的舍入“最小”微秒长度
#define USMAX \
    2500  // This is the rounded 'maximum' microsecond length based on the
          // maximum pulse of 512  这是基于 512 的最大脉冲的舍入“最大”微秒长度
#define SERVO_FREQ \
    50  // Analog servos run at ~50 Hz updates  模拟伺服以 ~50 Hz 更新运行

void setup() {
    M5.begin(true, true, true, true);
    pwm.begin();
    pwm.setPWMFreq(50);
    M5.Lcd.setCursor(115, 0, 4);
    M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
    M5.Lcd.print("Servo2");
}

void setServoPulse(uint8_t n, double pulse) {
    double pulselength;
    pulselength = 1000000;  // 1,000,000 us per second
    pulselength /= 50;      // 50 Hz
    Serial.print(pulselength);
    Serial.println(" us per period");
    pulselength /= 4096;  // 12 bits of resolution
    Serial.print(pulselength);
    Serial.println(" us per bit");
    pulse *= 1000;
    pulse /= pulselength;
    Serial.println(pulse);
    pwm.setPWM(n, 0, pulse);
}

void servo_angle_write(uint8_t n, int Angle) {
    double pulse = Angle;
    pulse        = pulse / 90 + 0.5;
    setServoPulse(n, pulse);
}

void loop() {
    for (int i = 0; i < 16; i++) {
        setServoPulse(i, 0.5);
    }
    delay(500);
    for (int i = 0; i < 16; i++) {
        setServoPulse(i, 2.5);
    }
    delay(500);
}
