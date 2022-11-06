/*
    Description: Use Pbhub to read the analog input value of the slave device,
   or drive multiple sets of RGB LEDs.
*/

#include <M5Stack.h>
#include <Wire.h>

#include "porthub.h"

#define X_OFFSET 10
#define Y_OFFSET 18

PortHub porthub;
uint8_t HUB_ADDR[6] = {HUB1_ADDR, HUB2_ADDR, HUB3_ADDR,
                       HUB4_ADDR, HUB5_ADDR, HUB6_ADDR};

void setup() {
    M5.begin(true, false, true);
    M5.Power.begin();
    porthub.begin();
    M5.Lcd.clear(BLACK);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.setTextSize(4);
}

void loop() {
    M5.Lcd.clear(BLACK);
    for (int i = 0; i < 6; i++) {
        M5.Lcd.setCursor(8 * X_OFFSET, (i * 2) * Y_OFFSET);
        M5.Lcd.printf("%d:%d", i + 1, porthub.hub_a_read_value(HUB_ADDR[i]));
    }

    for (int i = 0; i < 6; i++) {
        porthub.hub_wire_setBrightness(HUB_ADDR[i], 1);
        porthub.hub_wire_fill_color(HUB_ADDR[i], 0, 15, 250, 250, 250);
    }
    delay(1000);
}