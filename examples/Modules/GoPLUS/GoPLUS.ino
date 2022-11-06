/*
    Description: Use GoPLUS Module for four-channel servo test and three-channel
   ADC test.
*/
#include <M5Stack.h>
#include <Wire.h>

#include "GoPlus.h"

GoPlus goPlus;

#define X_LOCAL 40
#define Y_LOCAL 30

#define X_OFFSET 160
#define Y_OFFSET 23

// Print the header for a display screen
void header(const char *string, uint16_t color) {
    M5.Lcd.fillScreen(color);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_MAGENTA, TFT_BLUE);
    M5.Lcd.fillRect(0, 0, 320, 30, TFT_BLUE);
    M5.Lcd.setTextDatum(TC_DATUM);
    M5.Lcd.drawString(string, 160, 3, 4);
}

void Motor(void) {
    goPlus.Motor_write_speed(MOTOR_NUM0, (uint8_t)0x80, 250);
    goPlus.Motor_write_speed(MOTOR_NUM1, (uint8_t)0x80, 250);
    delay(500);
    goPlus.Motor_write_speed(MOTOR_NUM0, (uint8_t)0x80, 0);
    goPlus.Motor_write_speed(MOTOR_NUM1, (uint8_t)0x00, 0);
    delay(500);

    goPlus.Motor_write_speed(MOTOR_NUM1, (uint8_t)0x00, 250);
    goPlus.Motor_write_speed(MOTOR_NUM0, (uint8_t)0x00, 250);
    delay(500);
    goPlus.Motor_write_speed(MOTOR_NUM0, (uint8_t)0x80, 0);
    goPlus.Motor_write_speed(MOTOR_NUM1, (uint8_t)0x00, 0);
    delay(500);
}

void setup() {
    // put your setup code here, to run once:
    M5.begin();
    M5.Power.begin();
    goPlus.begin();
    header("G O P L U S", TFT_BLACK);
    M5.Lcd.setTextColor(TFT_MAGENTA, TFT_BLACK);

    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL, 2);
    M5.Lcd.printf("Read analog\n");

    M5.Lcd.setCursor(X_LOCAL + X_OFFSET, Y_LOCAL, 2);
    M5.Lcd.printf("Test Servo\n");

    M5.Lcd.setCursor(X_OFFSET - 60, Y_LOCAL + Y_OFFSET * 5, 2);
    M5.Lcd.printf("Test MoTor\n");

    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    pinMode(13, OUTPUT);
}
uint8_t a             = 20;
uint16_t adInValue[6] = {0};
int led               = 0;
void loop() {
    // put your main code here, to run repeatedly:
    digitalWrite(13, led);
    goPlus.Servo_write_angle(SERVO_NUM0, a);
    delay(100);
    goPlus.Servo_write_angle(SERVO_NUM1, a);
    delay(100);
    goPlus.Servo_write_angle(SERVO_NUM2, a);
    delay(100);
    goPlus.Servo_write_angle(SERVO_NUM3, a);
    delay(100);

    if (a > 180) {
        a = 10;
    }
    a = a + 170;

    M5.Lcd.fillRect(X_LOCAL + X_OFFSET, Y_LOCAL + Y_OFFSET, 100, 23, TFT_BLACK);
    M5.Lcd.setCursor(X_LOCAL + X_OFFSET, Y_LOCAL + Y_OFFSET, 2);
    M5.Lcd.printf("Angle:%d", a);

    adInValue[0] = goPlus.hub1_d_read_value(HUB_NUM0);
    delay(50);
    M5.Lcd.fillRect(X_LOCAL, Y_LOCAL + Y_OFFSET * 1, 100, 23, TFT_BLACK);
    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET * 1, 2);
    M5.Lcd.printf("A0:%d", adInValue[0]);

    adInValue[1] = goPlus.hub2_d_read_value(HUB_NUM0);
    delay(50);
    M5.Lcd.fillRect(X_LOCAL, Y_LOCAL + Y_OFFSET * 2, 100, 23, TFT_BLACK);
    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET * 2, 2);
    M5.Lcd.printf("A1:%d", adInValue[1]);

    adInValue[2] = goPlus.hub3_d_read_value(HUB_NUM0);
    delay(50);
    M5.Lcd.fillRect(X_LOCAL, Y_LOCAL + Y_OFFSET * 3, 100, 23, TFT_BLACK);
    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET * 3, 2);
    M5.Lcd.printf("A2:%d", adInValue[2]);

    led = !led;
    Motor();
}
