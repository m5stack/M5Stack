/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
* Visit the website for more information：https://docs.m5stack.com/en/products
*
* describe：Power Management Example
* date：2021/7/15
*******************************************************************************
*/
#include <M5Stack.h>

// After M5Core is started or reset
// the program in the setUp () function will be run, and this part will only be run once.
void setup(){
  M5.begin(); //Init M5Core
  M5.Power.begin(); //Init Power module
  M5.Power.lightSleep(SLEEP_SEC(5)); 
  M5.Lcd.setTextSize(2);  // Set the font size
  M5.Lcd.print("press ButtonA: shutdown,  use power button to turn  back on");// Screen printing the formatted string
}  

//After the program in setup() runs, it runs the program in loop()
//The loop() function is an infinite loop in which the program runs repeatedly
void loop() {
  M5.update();  //Read the press state of the key
  if(M5.BtnA.wasPressed()) {  // Check if the key is pressed
    M5.Power.powerOFF();  //Turn off power
  }
}
