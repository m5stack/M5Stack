/*
 * Need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin
 * put png file in (<project directory>/data)
 * Select Tools > ESP32 Sketch Data Upload menu item. This should start
 uploading the files into ESP32 flash file system.
*/
#include <M5Stack.h>

void setup() {
    M5.begin();
    M5.Power.begin();
    if (!SPIFFS.begin(true)) {
        Serial.println("SPIFFS Mount Failed");
        return;
    }

    M5.Lcd.drawPngFile(SPIFFS, "/test.png", 0, 0);
}

void loop() {
    M5.update();
    if (M5.BtnA.wasPressed()) {
        M5.Lcd.drawPngFile(SPIFFS, "/test.png", 0, 0);
    }

    if (M5.BtnB.wasPressed()) {
        M5.Lcd.drawPngFile(SPIFFS, "/test1.png", 0, 0);
    }
}
