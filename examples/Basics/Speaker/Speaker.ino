/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
* Visit the website for more information：https://docs.m5stack.com/en/products
*
* describe：Speaker example
* date：2021/7/15
*******************************************************************************
*/
#include <M5Stack.h>

// Set the frequency of the speaker
#define NOTE_D0 -1
#define NOTE_D1 294
#define NOTE_D2 330
#define NOTE_D3 350
#define NOTE_D4 393
#define NOTE_D5 441
#define NOTE_D6 495
#define NOTE_D7 556

#define NOTE_DL1 147
#define NOTE_DL2 165
#define NOTE_DL3 175
#define NOTE_DL4 196
#define NOTE_DL5 221
#define NOTE_DL6 248
#define NOTE_DL7 278

#define NOTE_DH1 589
#define NOTE_DH2 661
#define NOTE_DH3 700
#define NOTE_DH4 786
#define NOTE_DH5 882
#define NOTE_DH6 990
#define NOTE_DH7 112

// After M5Core is started or reset
// the program in the setUp () function will be run, and this part will only be run once.
void setup() {
  M5.begin(); //Init M5Core
  M5.Power.begin(); //Init Power module
  M5.Lcd.println("M5Stack Speaker test"); // Print the formatted string and wrap it
}

//After the program in setup() runs, it runs the program in loop()
//The loop() function is an infinite loop in which the program runs repeatedly
void loop() {
  M5.update();   //Read the press state of the key

  if(M5.BtnA.wasPressed()) {  //Check if the key is pressed
    M5.Lcd.println("A wasPressed");
    M5.Speaker.tone(NOTE_DH2, 200); // Set the speaker to ring at 661Hz for 200ms
  }else if(M5.BtnB.wasPressed()){
    M5.Lcd.println("B wasPressed");
    M5.Speaker.tone(NOTE_DH7); // Set the horn to continuously sound at 112Hz
  }else if(M5.BtnC.wasPressed()){
    M5.Lcd.println("C wasPressed");
    M5.Speaker.end(); // Turn off the speaker
  }
}
