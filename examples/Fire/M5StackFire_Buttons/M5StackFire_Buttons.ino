/*
 Button Test

   hardwware:  M5StackFire 
 
 September 2018, ChrisMicro
*/
/*
function list:

 uint8_t read();
 uint8_t isPressed();
 uint8_t isReleased();
 uint8_t wasPressed();
 uint8_t wasReleased();
 uint8_t pressedFor(uint32_t ms);
 uint8_t releasedFor(uint32_t ms);
 uint8_t wasReleasefor(uint32_t ms);
 uint32_t lastChange();

*/
#include <M5Stack.h>

void setup()
{
  M5.begin();
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE , BLUE);
  M5.Lcd.printf("Button Test\n\n");
  M5.Lcd.setTextColor(GREEN , BLACK);
}

void loop()
{
  if (M5.BtnA.wasPressed())
  {
    M5.Lcd.printf("A");
  }
  
  if (M5.BtnB.wasPressed())
  {
    M5.Lcd.printf("B");
  }
  
  if (M5.BtnC.wasPressed())
  {
    M5.Lcd.printf("C ... clear screen");
    delay(1000);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 10);
  }

  M5.update();

}
