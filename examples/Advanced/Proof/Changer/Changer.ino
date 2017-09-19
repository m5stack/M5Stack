/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
  
  modified 8 Sep 2016
  by Colby Newman
*/

#include <M5Stack.h>

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize the M5Stack object
  // m5.begin();
  Wire.begin();
  Serial.begin(115200);
  Serial.printf("start\r\n");
  
  // initialize digital pin LED_BUILTIN as an output.
  // pinMode(LED_PIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  uint8_t data1, data2;
  Wire.beginTransmission(0x75); 
  Wire.write(0xa3); 
  Wire.endTransmission(); 

  Wire.requestFrom((int)0x75, 1);

  // while(Wire.available()) 
  {
      data1 = Wire.read();    // receive a byte as character
      // data2 = Wire.read();    // receive a byte as character
      // Serial.printf("data: 0x%x, 0x%x\r\n", data1, data2);
      Serial.printf("data: 0x%x\r\n", data1);
  }

  // Wire.beginTransmission(0x75); 
  // Wire.write(2); 
  // Wire.write((uint8_t)0x78); 
  // // Wire.write((uint8_t)0x78); 
  // Wire.endTransmission();

  delay(2000);
}
