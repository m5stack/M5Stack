#include <M5Stack.h>


void setup() {
  // put your setup code here, to run once:
  M5.begin();
  //Wire.begin();
  M5.Lcd.setCursor(120, 10, 4);
  M5.Lcd.println("90/180 OPTICAL");

  pinMode(36,INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  M5.Lcd.setCursor(80, 120, 4);
  M5.Lcd.printf("ir receive: %d",digitalRead(36));

}
