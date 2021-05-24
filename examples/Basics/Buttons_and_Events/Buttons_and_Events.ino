#include <M5Stack.h>

void setup() {
  M5.begin();
  M5.BtnA.setLabel("Test");
  M5.BtnB.setLabel("Wow !");
  M5.BtnC.setLabel("Amazing !");
  M5.BtnA.off = M5.BtnB.off = M5.BtnC.off = {BLUE, WHITE, NODRAW};
  M5.BtnA.on = M5.BtnB.on = M5.BtnC.on = {RED, WHITE, NODRAW};
  M5.Buttons.addHandler(eventDisplay);
  M5.Buttons.draw();
}

void loop() {
	M5.update();
}

void eventDisplay(Event& e) {
  Serial.printf("\n%-12s %-18s", e.typeName(), e.objName());
  if (e.type == E_RELEASE || e.type == E_PRESSED) Serial.printf("%5d ms", e.duration);
}
