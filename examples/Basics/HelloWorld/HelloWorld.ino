#include <M5Stack.h>

// the setup routine runs once when M5Stack starts up
void setup(){

  // Initialize the M5Stack object
  M5.begin();

  /*
    Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project
  */
  M5.Power.begin();
    
  // LCD display
  M5.Lcd.print("Hello World");
}

// the loop routine runs over and over again forever
void loop() {

}
