/*
    Description: Read JOYSTICK Unit X, Y axis offset data and button status
*/
#include <M5Stack.h>
#include "Wire.h"

#define JOY_ADDR 0x52
void setup() {
  M5.begin();
  M5.Power.begin();
  M5.Lcd.clear();

  M5.Lcd.setTextFont(4);
  M5.Lcd.setCursor(70, 0, 4);
  M5.Lcd.println(("Joystick Test"));
  //disable the speak noise
  dacWrite(25, 0);

  Wire.begin(21, 22, 400000);
}

uint8_t x_data;
uint8_t y_data;
uint8_t button_data;
char data[100];
void loop() {
  // put your main code here, to run repeatedly:
  Wire.requestFrom(JOY_ADDR, 3);
  if (Wire.available()) {
    x_data = Wire.read();
    y_data = Wire.read();
    button_data = Wire.read();
    sprintf(data, "x:%d y:%d button:%d\n", x_data, y_data, button_data);
    Serial.print(data);

    M5.Lcd.setCursor(100, 50, 4);
    M5.Lcd.printf("X:%d      ",x_data);
    M5.Lcd.setCursor(100, 80, 4);
    M5.Lcd.printf("Y:%d      ",y_data);
    M5.Lcd.setCursor(100, 110, 4);
    M5.Lcd.printf("B:%d      ",button_data);
  }
  delay(200);
}
