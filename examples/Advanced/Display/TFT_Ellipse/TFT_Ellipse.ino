/*
  Ellipse drawing example

  This sketch does not use any fonts.
*/

#include <M5Stack.h>

void setup(void) {
  M5.begin();

  // M5.Lcd.setRotation(1);
}

void loop() {

  M5.Lcd.fillScreen(TFT_BLACK);

  // Draw some random ellipses
  for (int i = 0; i < 40; i++)
  {
    int rx = random(60);
    int ry = random(60);
    int x = rx + random(320 - rx - rx);
    int y = ry + random(240 - ry - ry);
    M5.Lcd.fillEllipse(x, y, rx, ry, random(0xFFFF));
  }

  delay(2000);
  M5.Lcd.fillScreen(TFT_BLACK);

  for (int i = 0; i < 40; i++)
  {
    int rx = random(60);
    int ry = random(60);
    int x = rx + random(320 - rx - rx);
    int y = ry + random(240 - ry - ry);
    M5.Lcd.drawEllipse(x, y, rx, ry, random(0xFFFF));
  }

  delay(2000);
}



