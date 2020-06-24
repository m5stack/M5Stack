/*
    Description: Use RELAY to switch on and off the circuit.
*/
#include <M5Stack.h>

void setup() {
  M5.begin();
  M5.Power.begin();
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextFont(4);
  M5.Lcd.setTextColor(YELLOW, BLACK);
  M5.Lcd.setCursor(50, 0, 4);
  M5.Lcd.println(("Relay Example"));
  //disable the speak noise
  dacWrite(25, 0);
  pinMode(26, OUTPUT);
}

void loop(void) {
  digitalWrite(26, HIGH);
  delay(500);
  digitalWrite(26, LOW);
  delay(500);
}
