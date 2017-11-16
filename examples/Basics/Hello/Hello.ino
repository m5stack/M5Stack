#include <M5Stack.h>

// the setup routine runs once when M5Stack starts up
void setup(){

  // Initialize the M5Stack object
  M5.begin();

  // LCD display
  M5.Lcd.printf("hello world");
}

// the loop routine runs over and over again forever
void loop() {

}
