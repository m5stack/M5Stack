/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
* Visit the website for more information：https://docs.m5stack.com/en/products
*
* describe：Display Example
* date：2021/7/15
*******************************************************************************
*/
#include <M5Stack.h>

// After M5Core is started or reset
// the program in the setUp () function will be run, and this part will only be run once.
void setup() {
  M5.begin(); //Init M5Core
  M5.Power.begin(); //Init Power module

  M5.Lcd.fillScreen(WHITE); // Set the screen background color to white
  delay(500); //Delay 500ms
  M5.Lcd.fillScreen(RED);
  delay(500);
  M5.Lcd.fillScreen(GREEN);
  delay(500);
  M5.Lcd.fillScreen(BLUE);
  delay(500);
  M5.Lcd.fillScreen(BLACK);
  delay(500);

  M5.Lcd.setCursor(10, 10); // Move the cursor position to (x,y)
  M5.Lcd.setTextColor(WHITE); // Set the font color to white,
  M5.Lcd.setTextSize(1);  // Set the font size
  M5.Lcd.printf("Display Test!");  // Serial output format string

  // draw graphic
  delay(1000);
  M5.Lcd.drawRect(100, 100, 50, 50, BLUE);  // Draw a 50x50 blue rectangle wireframe at (x,y)
  delay(1000);
  M5.Lcd.fillRect(100, 100, 50, 50, BLUE);  // Draw a blue rectangle 50x50 at (x,y)
  delay(1000);
  M5.Lcd.drawCircle(100, 100, 50, RED); // Draw a red circle of radius 50 at (x,y)
  delay(1000);
  M5.Lcd.fillCircle(100, 100, 50, RED); //Draw a red circle of radius 50 at (x,y)
  delay(1000);
  M5.Lcd.drawTriangle(30, 30, 180, 100, 80, 150, YELLOW); // Make a triangle wireframe with (x1,y1) (x2,y2) (x3,y3) as the vertices
  delay(1000);
  M5.Lcd.fillTriangle(30, 30, 180, 100, 80, 150, YELLOW); // Construct a triangle with (x1,y1) (x2,y2) (x3,y3) as its vertices
}

//After the program in setup() runs, it runs the program in loop()
//The loop() function is an infinite loop in which the program runs repeatedly
void loop(){

  M5.Lcd.fillTriangle(random(M5.Lcd.width()-1), random(M5.Lcd.height()-1), random(M5.Lcd.width()-1), random(M5.Lcd.height()-1), random(M5.Lcd.width()-1), random(M5.Lcd.height()-1), random(0xfffe));

  M5.update();  //Read the press state of the key
}
