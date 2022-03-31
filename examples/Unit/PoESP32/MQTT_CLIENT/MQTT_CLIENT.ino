/*
    Description:
    Use UNIT PoESP32 connect to MQTT Server
    UNIT PoESP32 Connect to M5Core PORT-C (G16/17)
    before compiling:
    M5Stack: https://github.com/m5stack/M5Stack
    M5GFX: https://github.com/m5stack/M5GFX
    UNIT_PoESP32: https://github.com/m5stack/UNIT_PoESP32
*/

#include "UNIT_PoESP32.h"
#include "M5Stack.h"
#include "M5GFX.h"

UNIT_PoESP32 eth;
String readstr;

M5GFX display;
M5Canvas canvas(&display);

void setup() {
    M5.begin();
    display.begin();
    canvas.setColorDepth(1);  // mono color
    canvas.setFont(&fonts::efontCN_14);
    canvas.setTextSize(2);
    canvas.setPaletteColor(1, GREEN);
    canvas.createSprite(display.width(), display.height());
    canvas.setTextScroll(true);
    eth.Init(&Serial2, 9600, 16, 17);
    delay(10);
    canvas.println("wait device connect");
    canvas.pushSprite(0, 0);
    while (!eth.checkDeviceConnect()) {
        delay(10);
    }

    canvas.println("device connected");
    canvas.println("wait ethernet connect");
    canvas.pushSprite(0, 0);
    while (!eth.checkETHConnect()) {
        delay(10);
    }
    canvas.println("ethernet connected");
    canvas.println("wait mqtt connect");
    canvas.pushSprite(0, 0);

    while (!eth.createMQTTClient("120.77.157.90", "1883", "client_id",
                                 "user_name", "password")) {
        delay(10);
    }
    canvas.println("mqtt connected");
    canvas.pushSprite(0, 0);
    while (!eth.subscribeMQTTMsg("PoESP32_MQTT_D", "2")) {
        delay(10);
    }
}

void loop() {
    if (Serial2.available()) {
        readstr = Serial2.readString();
        Serial.println(readstr);
        canvas.println(readstr);
        canvas.pushSprite(0, 0);
    }
    eth.publicMQTTMsg("PoESP32_MQTT_U", "Hello From PoESP32", "2");
    delay(2000);
}
