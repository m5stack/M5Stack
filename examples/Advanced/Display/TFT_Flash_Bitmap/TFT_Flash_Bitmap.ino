// Icon images are stored in tabs ^ e.g. Alert.h etc above this line
// more than one icon can be in a header file

// Arrays containing FLASH images can be created with UTFT library tool:
// (libraries\UTFT\Tools\ImageConverter565.exe)
// Convert to .c format then copy into a new tab

/*
 This sketch demonstrates loading images from arrays stored in program (FLASH) memory.

 Works with TFT_eSPI library here:
 https://github.com/Bodmer/TFT_eSPI

 This sketch does not use/need any fonts at all...

 Code derived from ILI9341_due library example

 Make sure all the display driver and pin comnenctions are correct by
 editting the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
*/

#include <M5Stack.h>

// Include the header files that contain the icons
#include "Alert.h"
#include "Close.h"
#include "Info.h"

long count = 0; // Loop count

void setup()
{
  M5.begin();
  M5.Power.begin();
  M5.Lcd.fillScreen(TFT_BLACK);

  // Swap the colour byte order when rendering
  M5.Lcd.setSwapBytes(true);

  // Draw the icons
  M5.Lcd.pushImage(100, 100, infoWidth, infoHeight, info);
  M5.Lcd.pushImage(140, 100, alertWidth, alertHeight, alert);
  M5.Lcd.pushImage(180, 100, closeWidth, closeHeight, closeX);

  // Pause here to admire the icons!
  delay(2000);

}

void loop()
{
  // Loop filling and clearing screen
  M5.Lcd.pushImage(random(M5.Lcd.width() -  infoWidth), random(M5.Lcd.height() -  infoHeight),  infoWidth,  infoHeight, info);
  M5.Lcd.pushImage(random(M5.Lcd.width() - alertWidth), random(M5.Lcd.height() - alertHeight), alertWidth, alertHeight, alert);
  M5.Lcd.pushImage(random(M5.Lcd.width() - closeWidth), random(M5.Lcd.height() - closeHeight), alertWidth, closeHeight, closeX);

  // Clear screen after 100 x 3 = 300 icons drawn
  if (1000 == count++) {
    count = 1;
    M5.Lcd.setRotation(2 * random(2)); // Rotate randomly to clear display left>right or right>left to reduce monotony!
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setRotation(1);
  }
}
