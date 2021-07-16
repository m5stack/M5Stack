/*
*******************************************************************************
*Copyright (c) 2021 by  M5Stack
*                 Equipped with M5Core sample source code
*Visit the website for more information：https://docs.m5stack.com/en/products
*
* describe：Hello World
* date：2021/7/15
*******************************************************************************
*/
#include <M5Stack.h>

// After M5Core is started or reset
// the program in the setUp () function will be run, and this part will only be run once.
void setup(){
  M5.begin(); //Init M5Core
  M5.Power.begin(); /*Power chip connected to gpio21, gpio22, I2C device
                      Set battery charging voltage and current
                     If used battery, please call this function in your project*/

  M5.Lcd.print("Hello World");  // Print text on the screen (string)
}

//After the program in setup() runs, it runs the program in loop()
//The loop() function is an infinite loop in which the program runs repeatedly
void loop() {

}