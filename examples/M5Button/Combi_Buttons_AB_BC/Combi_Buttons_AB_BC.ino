/*

  This example will show that the original buttons will not fire any
  high-level events after the combinations of buttons are detected, and their
  buttons will generate these events instead. Notice how it is not printing
  the low-level (E_TOUCH and E_RELEASE) events to not clutter the display.

  See https://github.com/m5stack/M5Stack/blob/master/src/utility/M5Button.h
  for complete documentation of buttons and events.

*/

#include <M5Stack.h>

// Shortcuts: the & means it's a reference to the same object
Button& A = M5.BtnA;
Button& B = M5.BtnB;
Button& C = M5.BtnC;

// New buttons, not tied to hardware pins
Button AB = Button(55, 193, 102, 21, true, "BtnAB");
Button BC = Button(161, 193, 102, 21, true, "BtnBC");

void setup() {
  M5.begin();
  A.off = B.off = C.off = AB.off = BC.off = {BLUE, WHITE, NODRAW};
  A.on  = B.on  = C.on  = AB.on  = BC.on  = {RED,  WHITE, NODRAW};
  M5.Buttons.draw();
  M5.Buttons.addHandler(eventDisplay, E_ALL - E_TOUCH - E_RELEASE);
  M5.Buttons.addHandler(buttonDown, E_TOUCH);
  M5.Buttons.addHandler(buttonUp, E_RELEASE);
}

void loop() {
  M5.update();
}

void buttonDown(Event& e) {
  if (A && B && !AB) {
    A.cancel();
    B.cancel();
    AB.fingerDown();
  }
  if (B && C && !BC) {
    B.cancel();
    C.cancel();
    BC.fingerDown();
  }
}

void buttonUp(Event& e) {
  if (AB && !(A && B)) AB.fingerUp();
  if (BC && !(B && C)) BC.fingerUp();
}


void eventDisplay(Event& e) {
  Serial.printf("%-14s %-18s   %5d ms\n", e.typeName(), e.objName(),
                e.duration);
}
