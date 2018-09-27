#include <M5Stack.h>
#include <Wire.h>

#define NOTE_D1 294

#define NOTE_DL1 261
#define NOTE_DL2 293
#define NOTE_DL3 329
#define NOTE_DL4 349
#define NOTE_DL5 392
#define NOTE_DL6 440
#define NOTE_DL7 494

void setup() {
  // Power ON Stabilizing...
  delay(500);
  M5.begin();
  pinMode(39, INPUT);
  pinMode(38, INPUT);
  // Init I2C
  Wire.begin();
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.println("M5Stack Makey Piano.");
}
int Key1 = 0, Key2 = 0, Index = 0;
void showKey() {

  M5.Lcd.setCursor(20, 200);
  M5.Lcd.setTextColor(BLUE, BLACK);
  M5.Lcd.print(Index);
  M5.Lcd.print("   ");

  M5.Lcd.setCursor(5 + 0 * 16, 110);
  if ((Key1 & (0x01 << 0)) == 0x00)
    M5.Lcd.setTextColor(WHITE, BLACK);
  else {
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Speaker.tone(NOTE_DL1, 20);
  }
  M5.Lcd.print("1");
  M5.Lcd.setCursor(5 + 1 * 16, 110);
  if ((Key1 & (0x01 << 1)) == 0x00)
    M5.Lcd.setTextColor(WHITE, BLACK);
  else {
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Speaker.tone(NOTE_DL2, 20);
  }
  M5.Lcd.print("2");
  M5.Lcd.setCursor(5 + 2 * 16, 110);
  if ((Key1 & (0x01 << 2)) == 0x00)
    M5.Lcd.setTextColor(WHITE, BLACK);
  else {
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Speaker.tone(NOTE_DL3, 20);
  }
  M5.Lcd.print("3");
  M5.Lcd.setCursor(5 + 3 * 16, 110);
  if ((Key1 & (0x01 << 3)) == 0x00)
    M5.Lcd.setTextColor(WHITE, BLACK);
  else {
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Speaker.tone(NOTE_DL4, 20);
  }
  M5.Lcd.print("4");
  M5.Lcd.setCursor(5 + 4 * 16, 110);
  if ((Key1 & (0x01 << 4)) == 0x00)
    M5.Lcd.setTextColor(WHITE, BLACK);
  else {
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Speaker.tone(NOTE_DL5, 20);
  }
  M5.Lcd.print("5");
  M5.Lcd.setCursor(5 + 5 * 16, 110);
  if ((Key1 & (0x01 << 5)) == 0x00)
    M5.Lcd.setTextColor(WHITE, BLACK);
  else {
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Speaker.tone(NOTE_DL6, 20);
  }
  M5.Lcd.print("6");
  M5.Lcd.setCursor(5 + 6 * 16, 110);
  if ((Key1 & (0x01 << 6)) == 0x00)
    M5.Lcd.setTextColor(WHITE, BLACK);
  else {
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Speaker.tone(NOTE_DL7, 20);
  }
  M5.Lcd.print("7");

  M5.Lcd.setCursor(10 + 7 * 16, 110);
  if ((Key1 & (0x01 << 7)) == 0x00)
    M5.Lcd.setTextColor(WHITE, BLACK);
  else {
    M5.Lcd.setTextColor(RED, BLACK);
    M5.Speaker.tone(NOTE_D1, 20);
  }

}

int CommandStatus = 0;
void loop() {
  Wire.requestFrom(0x51, 2);
  while (Wire.available()) {
    Key1 = Wire.read();
    Key2 = Wire.read();
    Index++;
  }
  showKey();
  // delay(10);
  M5.update();
}