#include <M5Stack.h>
#include "Wire.h"

#define FACE_JOY_ADDR 0x5e

void Init(){
  Wire.begin();
  for (int i = 0; i < 256; i++)
  {
    Wire.beginTransmission(FACE_JOY_ADDR);
    Wire.write(i % 4);
    Wire.write(random(256) * (256 - i) / 256);
    Wire.write(random(256) * (256 - i) / 256);
    Wire.write(random(256) * (256 - i) / 256);
    Wire.endTransmission();
    delay(2);
  }
  Led(0, 0, 0, 0);
  Led(1, 0, 0, 0);
  Led(2, 0, 0, 0);
  Led(3, 0, 0, 0);
}


void Led(int indexOfLED, int r, int g, int b){
  Wire.beginTransmission(FACE_JOY_ADDR);
  Wire.write(indexOfLED);
  Wire.write(r);
  Wire.write(g);
  Wire.write(b);
  Wire.endTransmission();
}

void setup() {
  M5.begin();
  M5.Power.begin();
  M5.Lcd.clear();
  M5.Lcd.setCursor(60, 0, 4);    
  M5.Lcd.printf("FACE JOYSTICK");

  Init();
}

uint8_t x_data_L;
uint8_t x_data_H;
uint16_t x_data;
uint8_t y_data_L;
uint8_t y_data_H;
uint16_t y_data;
uint8_t button_data;
char data[100];

void loop() {
  
  Wire.requestFrom(FACE_JOY_ADDR, 5);
  if (Wire.available()) {
    
    y_data_L = Wire.read();
    y_data_H = Wire.read();
    x_data_L = Wire.read();
    x_data_H = Wire.read();
    
    button_data = Wire.read();// Z(0: released 1: pressed)
    
    x_data = x_data_H << 8 |x_data_L;
    y_data = y_data_H << 8 |y_data_L;
    
    sprintf(data, "x:%d y:%d button:%d\n", x_data, y_data, button_data);
    Serial.print(data);

    M5.Lcd.setCursor(45, 120);    
    M5.Lcd.println(data);

    if (x_data > 600){
      Led(2,  0, 0, 50);
      Led(0, 0, 0, 0);
    }
    else  if (x_data < 400)
    {
      Led(0,  0, 0, 50);
      Led(2, 0, 0, 0);
    } 
    else{
      Led(0,  0, 0,0);
      Led(2, 0, 0, 0);
    }

    
    if (y_data > 600)
    {
      Led(3,  0, 0, 50);
      Led(1, 0, 0, 0);
    }
    else if (y_data < 400)
    {
      Led(1,  0, 0, 50);
      Led(3, 0, 0, 0);
    }
    else{
      Led(1,  0, 0, 0);
      Led(3, 0, 0, 0);
    }
  }
  delay(200);
}
