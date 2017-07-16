#include <M5Stack.h>

// the setup routine runs once when M5Stack starts up
void setup(){

  // initialize the M5Stack object
  M5.begin();

  // Lcd display
  M5.Lcd.printf("button test\r\n");
}

// the loop routine runs over and over again forever
void loop(){
  if(M5.BtnA.isPressed()) {
    M5.Lcd.printf("Pressed A \r\n");
    Serial.printf("Pressed A \r\n");
  }

  if(M5.BtnB.isPressed()) {
    M5.Lcd.printf("Pressed B \r\n");
    Serial.printf("Pressed B \r\n");
  }

  if(M5.BtnC.isPressed()) {
    M5.Lcd.printf("Pressed C \r\n");
    Serial.printf("Pressed C \r\n");
  }

  M5.update();
}
