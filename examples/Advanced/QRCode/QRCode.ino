#include <M5Stack.h>

void setup() {
  M5.begin();

  // Display QRCode
  M5.Lcd.qrcode("http://www.m5stack.com");
  // M5.Lcd.qrcode(const char *string, uint16_t x = 50, uint16_t y = 10, uint8_t width = 220, uint8_t version = 6);
}

void loop() {
}
