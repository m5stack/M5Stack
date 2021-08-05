/*
    Description: GRBL 13.2 Module TEST Example.Stack two Module at the same time.
*/
#include <M5Stack.h>
#include "MODULE_GRBL13.2.h"

/*
 * The I2C address of GRBL 13.2  Module is 0x70 by default.
 * You could use the DIP Switch for modify I2C address to 0x71
 */

#define STEPMOTOR_I2C_ADDR_1 0x70
#define STEPMOTOR_I2C_ADDR_2 0x71

GRBL _GRBL_A = GRBL(STEPMOTOR_I2C_ADDR_1);
GRBL _GRBL_B = GRBL(STEPMOTOR_I2C_ADDR_2);

void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Power.begin();
  _GRBL_A.Init();
  _GRBL_B.Init();
  Serial.begin(115200);
  m5.Lcd.setTextColor(WHITE, BLACK);
  m5.Lcd.setTextSize(3);
  m5.lcd.setBrightness(100);
  M5.Lcd.setCursor(80, 40);
  M5.Lcd.println("GRBL 13.2");
  M5.Lcd.setCursor(50, 80);
  M5.Lcd.println("Press Btn A/B");
  M5.Lcd.setCursor(50, 120);
  M5.Lcd.println("Control Motor");
  _GRBL_A.setMode("absolute");
  _GRBL_B.setMode("absolute");
}

void loop() {
  /*
      If Button A was pressed,
      stepmotor will rotate back and forth at a time
   */
  if (M5.BtnA.wasPressed())  // A button
  {
    Serial.print(_GRBL_A.readStatus());
    _GRBL_A.setMotor(5,5,5,200);
    _GRBL_B.setMotor(5,5,5,200);
    _GRBL_A.setMotor(0,0,0,200);
    _GRBL_B.setMotor(0,0,0,200);
  }

  if (M5.BtnB.wasPressed())
  {
       //USE Gcode
      _GRBL_A.sendGcode("G1 X5Y5Z5 F200");
      _GRBL_B.sendGcode("G1 X5Y5Z5 F200");
      _GRBL_A.sendGcode("G1 X0Y0Z0 F200");
      _GRBL_B.sendGcode("G1 X0Y0Z0 F200");
  }

  if (M5.BtnC.wasReleased()) {
      _GRBL_A.unLock();
      _GRBL_B.unLock();
  }
  M5.update();
}
