/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
* Visit the website for more information：https://docs.m5stack.com/en/products
*
* describe：MPU6886 example
* date：2021/7/15
*******************************************************************************
*/
#define M5STACK_MPU6886
#include <M5Stack.h>

float accX = 0.0F;  // Define variables for storing inertial sensor data
float accY = 0.0F;
float accZ = 0.0F;

float gyroX = 0.0F;
float gyroY = 0.0F;
float gyroZ = 0.0F;

float pitch = 0.0F;
float roll  = 0.0F;
float yaw   = 0.0F;

float temp = 0.0F;

// After M5Core is started or reset
// the program in the setUp () function will be run, and this part will only be run once.
void setup(){
  M5.begin(); //Init M5Core
  M5.Power.begin(); //Init Power module
    
  M5.IMU.Init();  //Init IMU sensor

  M5.Lcd.fillScreen(BLACK); // Set the screen background color to black
  M5.Lcd.setTextColor(GREEN , BLACK); // Sets the foreground color and background color of the displayed text
  M5.Lcd.setTextSize(2);  // Set the font size
}

//After the program in setup() runs, it runs the program in loop()
//The loop() function is an infinite loop in which the program runs repeatedly
void loop() {
  M5.IMU.getGyroData(&gyroX,&gyroY,&gyroZ); // Stores the triaxial gyroscope data of the inertial sensor to the relevant variable
  M5.IMU.getAccelData(&accX,&accY,&accZ); // Stores the triaxial accelerometer data of the inertial sensor to the relevant variable
  M5.IMU.getAhrsData(&pitch,&roll,&yaw);  // Stores the inertial sensor attitude to the relevant variable
  M5.IMU.getTempData(&temp);  // Stores the inertial sensor temperature to temp

// The M5Core screen is 320x240 pixels, starting at the top left corner of the screen (0,0).
// gyroscope output related
  M5.Lcd.setCursor(0, 20);  // Move the cursor position to (x,y)
  M5.Lcd.printf("gyroX,  gyroY, gyroZ"); // Screen printingformatted string
  M5.Lcd.setCursor(0, 42);
  M5.Lcd.printf("%6.2f %6.2f%6.2f o/s", gyroX, gyroY, gyroZ);

// Accelerometer output is related
  M5.Lcd.setCursor(0, 70);
  M5.Lcd.printf("accX,   accY,  accZ");
  M5.Lcd.setCursor(0, 92);
  M5.Lcd.printf("%5.2f  %5.2f  %5.2f G", accX, accY, accZ);

// Pose output is related
  M5.Lcd.setCursor(0, 120);
  M5.Lcd.printf("pitch,  roll,  yaw");
  M5.Lcd.setCursor(0, 142);
  M5.Lcd.printf("%5.2f  %5.2f  %5.2f deg", pitch, roll, yaw);

// Inertial sensor temperature output related
  M5.Lcd.setCursor(0, 175);
  M5.Lcd.printf("Temperature : %.2f C", temp);

  delay(1000);  // Delay 1000ms (1 sec)
}