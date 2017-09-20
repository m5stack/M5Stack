#include <M5Stack.h>

uint16_t blynk_color;
// the setup routine runs once when M5Stack starts up
void setup() {
  
  // initialize the M5Stack object
  m5.begin();
  blynk_color = M5.Lcd.color565(0, 216, 128);

  M5.Lcd.fillCircle(100, 100, 50, blynk_color);
}

// the loop routine runs over and over again forever
void loop(){

  m5.update();
}
