#include <M5Stack.h>

// the setup routine runs once when M5Stack starts up
void setup(){
  // initialize the M5Stack object
  m5.begin();
  // lcd display
  m5.lcd.printf("button test\r\n");
}

// the loop routine runs over and over again forever
void loop(){
  if(m5.pressed(BTN_A)) {
    m5.lcd.printf("Pressed A \r\n");
  }

  if(m5.pressed(BTN_B)) {
    m5.lcd.printf("Pressed B \r\n");
  }

  if(m5.pressed(BTN_C)) {
    m5.lcd.printf("Pressed C \r\n");
  }

  m5.loop();
}
