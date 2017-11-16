#include <M5Stack.h>

void setup(void) {

  M5.begin();

  //Draw the jpeg file form TF card
  M5.Lcd.setBrightness(200);
  M5.Lcd.drawJpgFile(SD, "/p2.jpg");
  // M5.Lcd.drawJpgFile(SD, "/p1.jpg", 20, 30, 40, 50);
  // M5.Lcd.drawJpgFile(fs::FS &fs, const char *path, uint16_t x, uint16_t y, uint16_t maxWidth, uint16_t maxHeight, uint16_t offX, uint16_t offY, jpeg_div_t scale);
}

void loop() {

}
