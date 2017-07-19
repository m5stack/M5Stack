#include <M5Stack.h>

// the setup routine runs once when M5Stack starts up
void setup(){

  // initialize the M5Stack object
  M5.begin();

  // Lcd display
  M5.Lcd.printf("button test\r\n");
}

// the loop routine runs over and over again forever
void loop() {

  if(M5.BtnA.wasPressed()) {
    M5.Lcd.printf("wasPressed A \r\n");
    Serial.printf("wasPressed A \r\n");
  }

  if(M5.BtnA.wasReleased()) {
    M5.Lcd.printf("wasReleased A \r\n");
    Serial.printf("wasReleased A \r\n");
  }

  if(M5.BtnA.pressedFor(2000)) {
    M5.Lcd.printf("pressedFor 2s A \r\n");
    Serial.printf("pressedFor 2s A \r\n");
  }

  if(M5.BtnB.wasPressed()) {
    M5.Lcd.printf("wasPressed B \r\n");
    Serial.printf("wasPressed B \r\n");
  }

  if(M5.BtnB.wasReleased()) {
    M5.Lcd.printf("wasReleased B \r\n");
    Serial.printf("wasReleased B \r\n");
  }

  if(M5.BtnB.pressedFor(2000)) {
    M5.Lcd.printf("pressedFor 2s B \r\n");
    Serial.printf("pressedFor 2s B \r\n");
  }

  if(M5.BtnC.wasPressed()) {
    M5.Lcd.printf("wasPressed C \r\n");
    Serial.printf("wasPressed C \r\n");
  }

  if(M5.BtnC.wasReleased()) {
    M5.Lcd.printf("wasReleased C \r\n");
    Serial.printf("wasReleased C \r\n");
  }

  if(M5.BtnC.pressedFor(2000)) {
    M5.Lcd.printf("pressedFor 2s C \r\n");
    Serial.printf("pressedFor 2s C \r\n");
  }

  M5.update();
}
