/*
    Description: Use SERVO Module to control the rotation of 12-channel servo.
*/
#include <Arduino.h>
#include <M5Stack.h>
#include <Wire.h>

#define SERVO_ADDR 0x53
void setup() {
    M5.begin(true, false, true);
    M5.Power.begin();
    M5.Lcd.setTextFont(4);
    M5.Lcd.setCursor(70, 100);
    M5.Lcd.print("Servo Example");

    Wire.begin(21, 22, 100000);
    // put your setup code here, to run once:
}

// addr 0x01 mean control the number 1 servo by us
void Servo_write_us(uint8_t number, uint16_t us) {
    Wire.beginTransmission(SERVO_ADDR);
    Wire.write(0x00 | number);
    Wire.write(us & 0x00ff);
    Wire.write(us >> 8 & 0x00ff);
    Wire.endTransmission();
}

// addr 0x11 mean control the number 1 servo by angle
void Servo_write_angle(uint8_t number, uint8_t angle) {
    Wire.beginTransmission(SERVO_ADDR);
    Wire.write(0x10 | number);
    Wire.write(angle);
    Wire.endTransmission();
}

void loop() {
    for(uint8_t i = 0; i < 12; i++){
        Servo_write_us(i, 700);
        // Servo_write_angle(i, 0);
    }
    delay(1000);
    for(uint8_t i = 0; i < 12; i++){
        Servo_write_us(i, 2300);
        // Servo_write_angle(i, 180);
    }
    delay(1000);
}