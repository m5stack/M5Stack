// Icons are stored in tabs ^ e.g. Alert.h etc above this line
// more than one icon can be in a header file

/*
 This sketch demonstrates loading images from arrays stored in program (FLASH) memory.

 Works with TFT_eSPI library here:
 https://github.com/Bodmer/TFT_eSPI

 This sketch does not use/need any fonts at all...

 Code derived from ILI9341_due example

 Make sure all the display driver and pin comnenctions are correct by
 editting the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
*/

#include <M5Stack.h>

// Include the header files that contain the icons
#include "alert.h"
#include "Close.h"
#include "Info.h"

long count = 0; // Loop count

void setup()
{
  M5.begin();

  M5.Lcd.fillScreen(TFT_BLACK);

  // Draw the icons
  drawIcon(info, 100, 100, infoWidth, infoHeight);
  drawIcon(alert, 140, 100, alertWidth, alertHeight);
  drawIcon(close, 180, 100, closeWidth, closeHeight);

  // Pause here to admire the icons!
  delay(2000);

}

void loop()
{
  // Loop filling and clearing screen
  drawIcon(info,  random(M5.Lcd.width() -  infoWidth), random(M5.Lcd.height() -  infoHeight),  infoWidth,  infoHeight);
  drawIcon(alert, random(M5.Lcd.width() - alertWidth), random(M5.Lcd.height() - alertHeight), alertWidth, alertHeight);
  drawIcon(close, random(M5.Lcd.width() - closeWidth), random(M5.Lcd.height() - closeHeight), alertWidth, closeHeight);

  // Clear screen after 100 x 3 = 300 icons drawn
  if (100 == count++) {
    count = 1;
    M5.Lcd.setRotation(2 * random(2)); // Rotate randomly to clear display left>right or right>left to reduce monotony!
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setRotation(1);
    //Serial.println(millis());
  }
}


//====================================================================================
// This is the function to draw the icon stored as an array in program memory (FLASH)
//====================================================================================

// To speed up rendering we use a 64 pixel buffer
#define BUFF_SIZE 64

// Draw array "icon" of defined width and height at coordinate x,y
// Maximum icon size is 255x255 pixels to avoid integer overflow

void drawIcon(const unsigned short* icon, int16_t x, int16_t y, int8_t width, int8_t height) {

  uint16_t  pix_buffer[BUFF_SIZE];   // Pixel buffer (16 bits per pixel)

  // Set up a window the right size to stream pixels into
  M5.Lcd.setWindow(x, y, x + width - 1, y + height - 1);

  // Work out the number whole buffers to send
  uint16_t nb = ((uint16_t)height * width) / BUFF_SIZE;

  // Fill and send "nb" buffers to TFT
  for (int i = 0; i < nb; i++) {
    for (int j = 0; j < BUFF_SIZE; j++) {
      pix_buffer[j] = pgm_read_word(&icon[i * BUFF_SIZE + j]);
    }
    M5.Lcd.pushColors(pix_buffer, BUFF_SIZE);
  }

  // Work out number of pixels not yet sent
  uint16_t np = ((uint16_t)height * width) % BUFF_SIZE;

  // Send any partial buffer left over
  if (np) {
    for (int i = 0; i < np; i++) pix_buffer[i] = pgm_read_word(&icon[nb * BUFF_SIZE + i]);
    M5.Lcd.pushColors(pix_buffer, np);
  }
}

