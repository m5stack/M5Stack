/*
    Description: This exmpale can display the encoder gear reading of the PLUS Module and the state of the keys.
*/
#include <Arduino.h>
#include <M5Stack.h>

#define IrPin 13
#define PLUS_ADDR 0x62

int32_t number = 0;
uint8_t press = 0;


void setup() {
    M5.begin(true, false, false);
    M5.Power.begin();
    M5.Lcd.setTextFont(6);
    M5.Lcd.clear(BLACK);
    M5.Lcd.setTextColor(ORANGE, BLACK);
    Wire.begin();
    ledcSetup(1, 38000, 10);
    ledcAttachPin(IrPin, 1);
}

void plus_encode() {
    Wire.requestFrom(PLUS_ADDR, 2);
    while(Wire.available()) {
        int8_t encode = Wire.read();
        uint8_t press_n = Wire.read();
        number += encode;
        if(press_n == 0xff) {
            press = 0;
        }
        else {
            press = 1;
        }
    }
}

void loop() {
    char data[20];

    plus_encode();
    ledcWrite(1, ledcRead(1) ? 0 : 512);
    sprintf(data, "%d  %d        ", number, press);
    M5.Lcd.setCursor(100, 100);
    M5.Lcd.print(data);
    vTaskDelay(200);
}