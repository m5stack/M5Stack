#include <M5Stack.h>

void setup() {
  M5.BtnA.off = M5.BtnB.off = M5.BtnC.off = {BLUE, WHITE, NODRAW};
  M5.BtnA.on  = M5.BtnB.on  = M5.BtnC.on  = {RED,  WHITE, NODRAW};
  M5.begin();
  M5.Buttons.draw();
}

void loop() {
  M5.update();
}
