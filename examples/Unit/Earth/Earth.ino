#include <M5Stack.h>

void setup() {
  M5.begin();
  //disable the speak noise
  dacWrite(25, 0);

  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(65, 50);
  M5.Lcd.printf("UNIT_EARTH EXAMPLE\n");
  pinMode(26, INPUT);
  M5.Lcd.setCursor(85, 80);
  M5.Lcd.print("GPIO36:");
  M5.Lcd.setCursor(85, 100);
  M5.Lcd.print("GPIO26:");
}

uint16_t analogRead_value = 0;
uint16_t digitalRead_value = 0;
void loop() {
  // put your main code here, to run repeatedly:
  M5.Lcd.setCursor(175, 80);
  M5.Lcd.setTextColor(BLACK);
  M5.Lcd.printf("%d\n", analogRead_value);
  M5.Lcd.setCursor(175, 100);
  M5.Lcd.printf("%d\n", digitalRead_value);
  analogRead_value = analogRead(36);
  digitalRead_value = digitalRead(26);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setCursor(175, 80);
  M5.Lcd.printf("%d\n", analogRead_value);
  M5.Lcd.setCursor(175, 100);
  M5.Lcd.printf("%d\n", digitalRead_value);
  delay(10);
}
