#include <M5Stack.h>

// the setup routine runs once when M5Stack starts up
void setup(){

  // initialize the M5Stack object
  M5.begin();

  // Lcd display
  M5.Lcd.println("This is software power off demo");
  M5.Lcd.println("Press the button A to power off.");
  
  // Set the wakeup button
  M5.setWakeupButton(BUTTON_A_PIN);
}

// the loop routine runs over and over again forever
void loop() {

  if(M5.BtnA.wasPressed()) {
    M5.powerOFF();
  }

  M5.update();
}
