#include <M5Stack.h>

void setup(void) {

  M5.begin();
  M5.Lcd.fillScreen(BLACK);

  Serial.print("Initializing SD card...");
  if (!SD.begin(TFCARD_CS_PIN)) {
    Serial.println("failed!");
  }
  Serial.println("OK!");

  M5.Lcd.bmpDraw("/bmp1.bmp", 0, 0);
}

void loop() {

}
