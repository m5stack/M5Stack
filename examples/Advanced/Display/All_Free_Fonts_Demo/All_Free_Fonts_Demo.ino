/*
  Example for TFT_eSPI library

  Created by Bodmer 31/12/16

  This example draws all fonts (as used by the Adafruit_GFX library) onto the
  screen. These fonts are called the GFX Free Fonts (GFXFF) in this library.

  The fonts are referenced by a short name, see the Free_Fonts.h file
  attached to this sketch.

  Other True Type fonts could be converted using the utility within the
  "fontconvert" folder inside the library. This converted has also been
  copied from the Adafruit_GFX library. 

  Since these fonts are a recent addition Adafruit do not have a tutorial
  available yet on how to use the fontconvert utility.   Linux users will
  no doubt figure it out!  In the meantime there are 48 font files to use
  in sizes from 9 point to 24 point, and in normal, bold, and italic or
  oblique styles.

  This example sketch uses both the print class and drawString() functions
  to plot text to the screen.

  Make sure LOAD_GFXFF is defined in the User_Setup.h file within the
  library folder.

  --------------------------- NOTE ----------------------------------------
  The free font encoding format does not lend itself easily to plotting
  the background without flicker. For values that changes on screen it is
  better to use Fonts 1- 8 which are encoded specifically for rapid
  drawing with background.
  -------------------------------------------------------------------------

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  ######       TO SELECT YOUR DISPLAY TYPE AND ENABLE FONTS          ######
  #########################################################################
*/
#include <M5Stack.h>

#define TEXT "aA MWyz~12" // Text that will be printed on screen in any font

#include "Free_Fonts.h" // Include the header file attached to this sketch

// #include "SPI.h"
// #include "TFT_eSPI.h"

// Use hardware SPI
// TFT_eSPI tft = TFT_eSPI();

unsigned long drawTime = 0;

void setup(void) {

  M5.begin();
  M5.Power.begin();
  // M5.Lcd.setRotation(1);

}

void loop() {

  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // Show all 48 fonts in centre of screen ( x,y coordinate 160,120)
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  // Where font sizes increase the screen is not cleared as the larger fonts overwrite
  // the smaller one with the background colour.

  // Set text datum to middle centre
  M5.Lcd.setTextDatum(MC_DATUM);

  // Set text colour to orange with black background
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  
  M5.Lcd.fillScreen(TFT_BLACK);            // Clear screen
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF1, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF1);                 // Select the font
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);// Print the string name of the font
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF2, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF2);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF3, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF3);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF4, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF4);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF5, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF5);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF6, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF6);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF7, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF7);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF8, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF8);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF9, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF9);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF10, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF10);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF11, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF11);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF12, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF12);
    M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF13, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF13);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF14, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF14);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF15, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF15);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF16, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF16);
    M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF17, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF17);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF18, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF18);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF19, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF19);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF20, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF20);
    M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF21, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF21);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF22, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF22);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF23, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF23);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF24, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF24);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF25, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF25);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF26, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF26);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF27, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF27);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF28, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF28);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF29, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF29);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF30, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF30);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF31, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF31);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF32, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF32);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF33, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF33);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF34, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF34);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF35, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF35);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF36, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF36);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF37, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF37);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF38, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF38);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF39, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF39);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF40, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF40);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF41, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF41);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF42, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF42);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF43, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF43);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF44, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF44);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF45, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF45);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF46, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF46);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF47, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF47);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);
  //M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setFreeFont(FF18);                 // Select the font
  M5.Lcd.drawString(sFF48, 160, 60, GFXFF);// Print the string name of the font
  M5.Lcd.setFreeFont(FF48);
  M5.Lcd.drawString(TEXT, 160, 120, GFXFF);
  delay(1000);

}

// There follows a crude way of flagging that this example sketch needs fonts which
// have not been enbabled in the User_Setup.h file inside the TFT_HX8357 library.
//
// These lines produce errors during compile time if settings in User_Setup are not correct
//
// The error will be "does not name a type" but ignore this and read the text between ''
// it will indicate which font or feature needs to be enabled
//
// Either delete all the following lines if you do not want warnings, or change the lines
// to suit your sketch modifications.

#ifndef LOAD_GLCD
//ERROR_Please_enable_LOAD_GLCD_in_User_Setup
#endif

#ifndef LOAD_GFXFF
ERROR_Please_enable_LOAD_GFXFF_in_User_Setup!
#endif

