#include <M5Stack.h>

// the setup routine runs once when M5Stack starts up
void setup(){
  // initialize the M5Stack object
  m5.begin();
}

// the loop routine runs over and over again forever
void loop(){

  m5.lcd.drawPicture("/ROBOT2.bmp");
  m5.lcd.drawPicture("/ROBOT3.bmp");
  m5.lcd.drawPicture("/ROBOT4.bmp");
  m5.lcd.drawPicture("/ROBOT5.bmp");  

  m5.loop();
}
