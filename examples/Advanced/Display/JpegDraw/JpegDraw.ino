#include <M5Stack.h>

void setup(void) {

  M5.begin();

  //Draw the jpeg file form TF card
  M5.Lcd.setBrightness(200);
  M5.Lcd.drawJpgFile(SD, "/p2.jpg");
}

void loop() {

}
