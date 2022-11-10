#include <M5Stack.h>
#include <M5GFX.h>
#include "M5_ANGLE8.h"

M5GFX display;
M5Canvas canvas(&display);
M5_ANGLE8 angle8;

uint32_t rgb_c = 0;

void print_adc_val(uint8_t i, uint16_t adc_v) {
    canvas.drawRect(0, i * 20, 200, 15, 1);
    canvas.fillRect(0, i * 20, map(adc_v, 0, 4096, 0, 200), 15, 1);
    canvas.setCursor(215, i * 20);
    canvas.setTextSize(1);
    canvas.printf("CH:%d ADC: %d", i, adc_v);
}

// ADC 12 Bit
void TaskADC12(uint16_t delay_t) {
    canvas.createSprite(display.width(), 160);
    canvas.fillSprite(0);
    uint16_t adc_v = 0;
    for (uint8_t i = 0; i < ANGLE8_TOTAL_ADC; i++) {
        adc_v = angle8.getAnalogInput(i, _12bit);
        print_adc_val(i, adc_v);
    }
    canvas.pushSprite(0, 45);
    vTaskDelay(delay_t);
}

// Breathing RGBLED
void TaskRGBLED_1(uint8_t br, uint16_t delay_t) {
    canvas.createSprite(display.width(), 35);
    rgb_c = 0;
    canvas.fillSprite(0);
    for (uint8_t m = 0; m < 12; m++) {
        rgb_c = 0x03 << (m * 2) | rgb_c;
        for (uint8_t i = 0; i < ANGLE8_TOTAL_LED; i++) {
            angle8.setLEDColor(i, rgb_c, br);
            vTaskDelay(25);
        }
        canvas.setCursor(0, 0);
        canvas.printf("COLOR: 0x%X", rgb_c);
        canvas.pushSprite(0, 205);
        vTaskDelay(delay_t);
    }
}

void TaskRGBLED_2(uint8_t br, uint16_t delay_t) {
    canvas.createSprite(display.width(), 35);
    rgb_c = 0;
    canvas.fillSprite(0);
    for (uint8_t m = 0; m < 6; m++) {
        rgb_c = 0xF0 << (m * 4);
        for (uint8_t i = 0; i < ANGLE8_TOTAL_LED; i++) {
            angle8.setLEDColor(i, rgb_c, br);
            vTaskDelay(25);
        }
        canvas.setCursor(0, 0);
        canvas.printf("COLOR: 0x%X", rgb_c);
        canvas.pushSprite(0, 205);
        vTaskDelay(delay_t);
    }
}

void TaskRGBLED_3(uint32_t color, uint8_t br, uint16_t delay_t) {
    canvas.createSprite(display.width(), 35);
    rgb_c = 0;
    canvas.fillSprite(0);
    for (uint8_t i = 0; i < ANGLE8_TOTAL_LED; i++) {
        angle8.setLEDColor(i, color, br);
        vTaskDelay(delay_t);
    }
    canvas.setCursor(0, 0);
    canvas.printf("COLOR: 0x%X", rgb_c);
    canvas.pushSprite(0, 205);
}

void setup() {
    M5.begin();
    display.begin();
    canvas.setColorDepth(1);  // mono color
    canvas.setFont(&fonts::efontCN_14);
    while (!angle8.begin(ANGLE8_I2C_ADDR)) {
        Serial.println("angle8 Connect Error");
        M5.Lcd.print("angle8 Connect Error");
        delay(100);
    }
    canvas.createSprite(display.width(), 45);
    canvas.setPaletteColor(1, GREENYELLOW);
    canvas.fillSprite(0);
    canvas.setTextSize(1);
    canvas.drawString("UNIT ANGLE8", 120, 5);
    canvas.drawString("FW VERSION: " + String(angle8.getVersion()), 10, 25);
    canvas.pushSprite(0, 0);
}

void loop() {
    M5.update();
    if (M5.BtnA.wasReleased()) {
        TaskRGBLED_3(0xffffff, 100, 2);
    } else if (M5.BtnB.wasReleased()) {
        TaskRGBLED_3(0xffffff, 50, 2);
    } else if (M5.BtnC.wasReleased()) {
        TaskRGBLED_3(0xffffff, 10, 2);
    }
    if (angle8.getDigitalInput())
        TaskADC12(100);
    else
        TaskRGBLED_2(5, 100);
}
