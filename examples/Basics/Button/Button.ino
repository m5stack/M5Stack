/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
* Visit the website for more information：https://docs.m5stack.com/en/products
*
* describe：Button example
* date：2021/7/15
*******************************************************************************
*/
#include <M5Stack.h>
// After M5Core is started or reset
// the program in the setUp () function will be run, and this part will only be run once.  
void setup() {
  M5.begin();  //Init M5Core
  M5.Power.begin();//Init Power module

  M5.Lcd.setTextColor(YELLOW);  // Set the font color to yellow
  M5.Lcd.setTextSize(2);  // Set the font size
  M5.Lcd.setCursor(65, 10); //Move the cursor position to (x, y)
  M5.Lcd.println("Button example"); //The screen prints the formatted string and wraps the line
  M5.Lcd.setCursor(3, 35);
  M5.Lcd.println("Press button B for 700ms");
  M5.Lcd.println("to clear screen.");
  M5.Lcd.setTextColor(RED);
}

//After the program in setup() runs, it runs the program in loop()
//The loop() function is an infinite loop in which the program runs repeatedly
void loop() {
  M5.update();  //Read the press state of the key
 //Returns 1 if key A is released or pressed longer than the specified time
  if (M5.BtnA.wasReleased() || M5.BtnA.pressedFor(1000, 200)) {
    M5.Lcd.print('A');
  } else if (M5.BtnB.wasReleased() || M5.BtnB.pressedFor(1000, 200)) {
    M5.Lcd.print('B');
  } else if (M5.BtnC.wasReleased() || M5.BtnC.pressedFor(1000, 200)) {
    M5.Lcd.print('C');
  } else if (M5.BtnB.wasReleasefor(700)) {
    M5.Lcd.clear(WHITE);  // Clear the screen and set white to the background color
    M5.Lcd.setCursor(0, 0);
  }
}
