/*
    Name:       button.ino
    Created:    2018/9/21 14:06:15
    Author:     sakabin
*/

#include <M5Stack.h>
// The setup() function runs once each time the micro-controller starts
void setup() {
  // init lcd, serial, but don't init sd card
  M5.begin(true, false, true);
  
  /*
    Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project
  */
  M5.Power.begin();

  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(65, 10);
  M5.Lcd.println("Button example");
  M5.Lcd.setCursor(3, 35);
  M5.Lcd.println("Press button B for 700ms");
  M5.Lcd.println("to clear screen.");
  M5.Lcd.setTextColor(RED);
}

// Add the main program code into the continuous loop() function
void loop() {
  // update button state
  M5.update();
 
  // if you want to use Releasefor("was released for"), use .wasReleasefor(int time) below
  if (M5.BtnA.wasReleased()) {
    M5.Lcd.print('A');
  } else if (M5.BtnB.wasReleased()) {
    M5.Lcd.print('B');
  } else if (M5.BtnC.wasReleased()) {
    M5.Lcd.print('C');
  } else if (M5.BtnB.wasReleasefor(700)) {
    M5.Lcd.clear(BLACK);
    M5.Lcd.setCursor(0, 0);
  }
}
