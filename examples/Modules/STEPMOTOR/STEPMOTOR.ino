/*
    Description: STEP-MOTOR Module TEST Example,If Button A was pressed, stepmotor will rotate back and forth at a time.
*/
#include <M5Stack.h>
#include <Wire.h>

/*
 * The I2C address of StepMotor Module is 0x70 by default.
 * But if you change this I2C address through burning this firmware
 * (https://github.com/m5stack/stepmotor_module/blob/master/Firmware%20for%20stepmotor%20module/GRBL-Arduino-Library/examples/GRBL_I2C/GRBL_I2C_0x71.hex),
 * you need to use I2C address `0x71` for correct communication.
 */

#define STEPMOTOR_I2C_ADDR 0x70
// #define STEPMOTOR_I2C_ADDR 0x71

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Power.begin();
  Wire.begin();
  Serial.begin(115200);
  m5.Lcd.setTextColor(WHITE, BLACK);
  m5.Lcd.setTextSize(2);
  m5.lcd.setBrightness(100);
  M5.Lcd.setCursor(4, 10);
  M5.Lcd.println("StepMotor Test: 0x70");
  M5.Lcd.setCursor(4, 30);
  M5.Lcd.println("Press A: 0x70");
}

void SendByte(byte addr, byte b) {
  Wire.beginTransmission(addr);
  Wire.write(b);
  Wire.endTransmission();
}

void SendCommand(byte addr, char *c) {
  Wire.beginTransmission(addr);
  while ((*c) != 0) {
    Wire.write(*c);
    c++;
  }
  Wire.write(0x0d);
  Wire.write(0x0a);
  Wire.endTransmission();
}

void loop() {
  /*
      If Button A was pressed,
      stepmotor will rotate back and forth at a time
   */
  if (digitalRead(39) == LOW)  // A button
  {
    while (digitalRead(39) == LOW) delay(1);
    SendCommand(STEPMOTOR_I2C_ADDR, "G1 X20Y20Z20 F500");
    SendCommand(STEPMOTOR_I2C_ADDR, "G1 X0Y0Z0 F400");
  }
  if (digitalRead(37) == LOW)  // C button
  {
    while (1) {
      SendCommand(STEPMOTOR_I2C_ADDR, "G1 X0Y0Z0 F500");
      delay(1000);
      M5.Lcd.print(".");
      delay(1000);
      SendCommand(STEPMOTOR_I2C_ADDR, "G1 X5Y5Z5 F500");
      delay(1000);
      M5.Lcd.print(".");
      delay(1000);
    }
  }
  // Get Data from Module.
  Wire.requestFrom(STEPMOTOR_I2C_ADDR, 1);
  if (Wire.available() > 0) {
    int u = Wire.read();
    if (u != 0) Serial.write(u);
  }
  delay(1);
  // Send Data to Module.
  while (Serial.available() > 0) {
    int inByte = Serial.read();
    SendByte(STEPMOTOR_I2C_ADDR, inByte);
  }
}
