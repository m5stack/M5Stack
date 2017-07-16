#include <M5Stack.h>

// the setup routine runs once when M5Stack starts up
void setup(){
  // initialize the M5Stack object
  m5.begin();
  
  // lcd display
  m5.Lcd.printf("hello world");
}

// the loop routine runs over and over again forever
void loop(){

  m5.loop();
}
