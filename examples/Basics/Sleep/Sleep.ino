/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
* Visit the website for more information：https://docs.m5stack.com/en/products
*
* describe：Sleep example
* date：2021/7/15
*******************************************************************************
*/
#include <M5Stack.h>

// After M5Core is started or reset
// the program in the setUp () function will be run, and this part will only be run once.
void setup() {
  M5.begin(); //Init M5Core
  M5.Power.begin(); //Init Power module

  M5.Power.setWakeupButton(BUTTON_A_PIN); // Set the screen wake-up button to A
  M5.Lcd.setBrightness(200);  // Set the screen brightness to 200 nits
}

//After the program in setup() runs, it runs the program in loop()
//The loop() function is an infinite loop in which the program runs repeatedly
void loop() {
  M5.update();  //Read the press state of the key
  bool c=M5.Power.isResetbyPowerSW(); // Determine if M5Core is started when powered on
  bool d=M5.Power.isResetbyDeepsleep(); // Determine if M5Core starts after deep sleep
 
  M5.Lcd.println("<Sleep test>"); // Print the formatted string and wrap it
  M5.Lcd.printf("power-on triggered at:%s%s\n\n",(c) ? ("POWER-SW") : (""),(d) ? ("DeepSleep-end") : (""));
  
  M5.Lcd.printf("Go lightSleep (5s or press buttonA wake up)\n");
  delay(2500);  //delay 2500ms
  M5.Power.lightSleep(SLEEP_SEC(10));  /*/Restart after 10 seconds of light sleep and continue from the next line
                                          Calling this function will disable the power button to restore the power button
                                          Please call M5.Power.setPowerBoostKeepOn(false)*/
  M5.Lcd.printf("Go lightSleep (press buttonA wake up)\n");
  delay(2500);
  M5.Power.lightSleep(0);
  
  M5.Lcd.printf("resume.\n\nGo deepSleep (press buttonA wake up) ");
  delay(2500);
  M5.Power.deepSleep(0);  /*After waking up from deep sleep for 0 seconds, 
                            the CPU will restart and the program will be executed from the beginning
                            Calling this function will disable the power button to restore the power button
                            Please call M5.Power.setPowerBoostKeepOn(false)*/
}
