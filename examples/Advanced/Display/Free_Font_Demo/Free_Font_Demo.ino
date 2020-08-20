/*
  This example draws fonts (as used by the Adafruit_GFX library) onto the
  screen. These fonts are called the GFX Free Fonts (GFXFF) in this library.

  Other True Type fonts could be converted using the utility within the
  "fontconvert" folder inside the library. This converted has also been
  copied from the Adafruit_GFX library.

  Since these fonts are a recent addition Adafruit do not have a tutorial
  available yet on how to use the utility.   Linux users will no doubt
  figure it out!  In the meantime there are 48 font files to use in sizes
  from 9 point to 24 point, and in normal, bold, and italic or oblique
  styles.

  This example sketch uses both the print class and drawString() functions
  to plot text to the screen.

  Make sure LOAD_GFXFF is defined in the User_Setup.h file within the
  TFT_eSPI library folder.

  --------------------------- NOTE ----------------------------------------
  The free font encoding format does not lend itself easily to plotting
  the background without flicker. For values that changes on screen it is
  better to use Fonts 1- 8 which are encoded specifically for rapid
  drawing with background.
  -------------------------------------------------------------------------
  
  >>>>>>>>>>>>>>>>>>>>>>>>>>> WARNING <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

  As supplied with the default settings the sketch has 11 fonts loaded,
  i.e. GLCD (Font 1), Font 2, Font 4, Font 6, Font 7, Font 8 and five Free Fonts,
  even though they are not all used in the sketch.
  
  Disable fonts you do not need in User_Setup.h in the library folder.

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################
*/

#include <M5Stack.h>
#include "Free_Fonts.h" // Include the header file attached to this sketch

unsigned long drawTime = 0;

void setup(void) {

  M5.begin();
  M5.Power.begin();
}

void loop() {

  int xpos =  0;
  int ypos = 40;

  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // Select different fonts to draw on screen using the print class
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  header("Using print() method", TFT_NAVY);

  // For comaptibility with Adafruit_GFX library the text background is not plotted when using the print class
  // even if we specify it.
  M5.Lcd.setTextColor(TFT_YELLOW);
  M5.Lcd.setCursor(xpos, ypos);    // Set cursor near top left corner of screen

  M5.Lcd.setFreeFont(TT1);     // Select the orginal small TomThumb font
  M5.Lcd.println();             // Move cursor down a line
  M5.Lcd.print("The really tiny TomThumb font");    // Print the font name onto the TFT screen
  M5.Lcd.println();
  M5.Lcd.println();

  M5.Lcd.setFreeFont(FSB9);   // Select Free Serif 9 point font, could use:
  // M5.Lcd.setFreeFont(&FreeSerif9pt7b);
  M5.Lcd.println();          // Free fonts plot with the baseline (imaginary line the letter A would sit on)
  // as the datum, so we must move the cursor down a line from the 0,0 position
  M5.Lcd.print("Serif Bold 9pt");  // Print the font name onto the TFT screen

  M5.Lcd.setFreeFont(FSB12);       // Select Free Serif 12 point font
  M5.Lcd.println();                // Move cursor down a line
  M5.Lcd.print("Serif Bold 12pt"); // Print the font name onto the TFT screen

  M5.Lcd.setFreeFont(FSB18);       // Select Free Serif 12 point font
  M5.Lcd.println();                // Move cursor down a line
  M5.Lcd.print("Serif Bold 18pt"); // Print the font name onto the TFT screen

  M5.Lcd.setFreeFont(FSB24);       // Select Free Serif 24 point font
  M5.Lcd.println();                // Move cursor down a line
  M5.Lcd.print("Serif Bold 24pt"); // Print the font name onto the TFT screen


  delay(4000);

  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // Now use drawString() so we can set font background colours and the datum
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  header("Using drawString()", TFT_BLACK);

  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);

  M5.Lcd.setTextDatum(TC_DATUM); // Centre text on x,y position

  xpos = M5.Lcd.width() / 2; // Half the screen width
  ypos = 50;

  M5.Lcd.setFreeFont(FSB9);                              // Select the font
  M5.Lcd.drawString("Serif Bold 9pt", xpos, ypos, GFXFF);  // Draw the text string in the selected GFX free font
  ypos += M5.Lcd.fontHeight(GFXFF);                      // Get the font height and move ypos down

  M5.Lcd.setFreeFont(FSB12);
  M5.Lcd.drawString("Serif Bold 12pt", xpos, ypos, GFXFF);
  ypos += M5.Lcd.fontHeight(GFXFF);

  M5.Lcd.setFreeFont(FSB18);
  M5.Lcd.drawString("Serif Bold 18pt", xpos, ypos, GFXFF);
  ypos += M5.Lcd.fontHeight(GFXFF);

  M5.Lcd.setFreeFont(FSB24);
  M5.Lcd.drawString("Serif Bold 24pt", xpos, ypos, GFXFF);
  ypos += M5.Lcd.fontHeight(GFXFF);

  // Set text padding to 100 pixels wide area to over-write old values on screen
  M5.Lcd.setTextPadding(100);
  for (int i = 0; i <= 20; i++) {
    M5.Lcd.drawFloat(i / 10.0, 1, xpos, ypos, GFXFF);
    delay (200);
  }

  delay(4000);

  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // Same again but with colours that show bounding boxes
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


  header("With background", TFT_DARKGREY);

  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);

  M5.Lcd.setTextDatum(TC_DATUM); // Centre text on x,y position

  xpos = M5.Lcd.width() / 2; // Half the screen width
  ypos = 50;

  M5.Lcd.setFreeFont(FSB9);                              // Select the font
  M5.Lcd.drawString("Serif Bold 9pt", xpos, ypos, GFXFF);  // Draw the text string in the selected GFX free font
  ypos += M5.Lcd.fontHeight(GFXFF);                        // Get the font height and move ypos down

  M5.Lcd.setFreeFont(FSB12);
  M5.Lcd.drawString("Serif Bold 12pt", xpos, ypos, GFXFF);
  ypos += M5.Lcd.fontHeight(GFXFF);

  M5.Lcd.setFreeFont(FSB18);
  M5.Lcd.drawString("Serif Bold 18pt", xpos, ypos, GFXFF);
  ypos += M5.Lcd.fontHeight(GFXFF);

  M5.Lcd.setFreeFont(FSBI24);
  M5.Lcd.drawString("Bold Italic 24pt", xpos, ypos, GFXFF);
  ypos += M5.Lcd.fontHeight(GFXFF);

  // Set text padding to 100 pixels wide area to over-write old values on screen
  M5.Lcd.setTextPadding(100);
  for (int i = 0; i <= 20; i++) {
    M5.Lcd.drawFloat(i / 10.0, 1, xpos, ypos, GFXFF);
    delay (200);
  }

  delay(4000);

  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  // Now show setting the 12 datum positions works with free fonts
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

  // Numbers, floats and strings can be drawn relative to a datum
  header("Text with a datum", TFT_BLACK);
  M5.Lcd.setTextColor(TFT_DARKGREY, TFT_BLACK);
  M5.Lcd.setFreeFont(FSS12);
  M5.Lcd.setTextDatum(TL_DATUM);
  M5.Lcd.drawString("[Top left]", 160, 120, GFXFF);
  drawDatumMarker(160,120);
  delay(1000);

  M5.Lcd.fillRect(0, 80, 320, 80, TFT_BLACK);
  M5.Lcd.setTextDatum(TC_DATUM);
  M5.Lcd.drawString("[Top centre]", 160, 120, GFXFF);
  drawDatumMarker(160,120);
  delay(1000);

  M5.Lcd.fillRect(0, 80, 320, 80, TFT_BLACK);
  M5.Lcd.setTextDatum(TR_DATUM);
  M5.Lcd.drawString("[Top right]", 160, 120, GFXFF);
  drawDatumMarker(160,120);
  delay(1000);

  M5.Lcd.fillRect(0, 80, 320, 80, TFT_BLACK);
  M5.Lcd.setTextDatum(ML_DATUM);
  M5.Lcd.drawString("[Middle left]", 160, 120, GFXFF);
  drawDatumMarker(160,120);
  delay(1000);

  M5.Lcd.fillRect(0, 80, 320, 80, TFT_BLACK);
  M5.Lcd.setTextDatum(MC_DATUM);
  M5.Lcd.drawString("[Middle centre]", 160, 120, GFXFF);
  drawDatumMarker(160,120);
  delay(1000);

  M5.Lcd.fillRect(0, 80, 320, 80, TFT_BLACK);
  M5.Lcd.setTextDatum(MR_DATUM);
  M5.Lcd.drawString("[Middle right]", 160, 120, GFXFF);
  drawDatumMarker(160,120);
  delay(1000);

  M5.Lcd.fillRect(0, 80, 320, 80, TFT_BLACK);
  M5.Lcd.setTextDatum(BL_DATUM);
  M5.Lcd.drawString("[Bottom left]", 160, 120, GFXFF);
  drawDatumMarker(160,120);
  delay(1000);

  M5.Lcd.fillRect(0, 80, 320, 80, TFT_BLACK);
  M5.Lcd.setTextDatum(BC_DATUM);
  M5.Lcd.drawString("[Bottom centre]", 160, 120, GFXFF);
  drawDatumMarker(160,120);
  delay(1000);

  M5.Lcd.fillRect(0, 80, 320, 80, TFT_BLACK);
  M5.Lcd.setTextDatum(BR_DATUM);
  M5.Lcd.drawString("[Bottom right]", 160, 120, GFXFF);
  drawDatumMarker(160,120);
  delay(1000);

  M5.Lcd.fillRect(0, 80, 320, 80, TFT_BLACK);
  M5.Lcd.setTextDatum(L_BASELINE);
  M5.Lcd.drawString("[Left baseline]", 160, 120, GFXFF);
  drawDatumMarker(160,120);
  delay(1000);

  M5.Lcd.fillRect(0, 80, 320, 80, TFT_BLACK);
  M5.Lcd.setTextDatum(C_BASELINE);
  M5.Lcd.drawString("[Centre baseline]", 160, 120, GFXFF);
  drawDatumMarker(160,120);
  delay(1000);

  M5.Lcd.fillRect(0, 80, 320, 80, TFT_BLACK);
  M5.Lcd.setTextDatum(R_BASELINE);
  M5.Lcd.drawString("[Right baseline]", 160, 120, GFXFF);
  drawDatumMarker(160,120);
  delay(1000);

  //while(1);
  delay(8000);

}

// Print the header for a display screen
void header(const char *string, uint16_t color)
{
  M5.Lcd.fillScreen(color);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(TFT_MAGENTA, TFT_BLUE);
  M5.Lcd.fillRect(0, 0, 320, 30, TFT_BLUE);
  M5.Lcd.setTextDatum(TC_DATUM);
  M5.Lcd.drawString(string, 160, 2, 4); // Font 4 for fast drawing with background
}

// Draw a + mark centred on x,y
void drawDatumMarker(int x, int y)
{
  M5.Lcd.drawLine(x - 5, y, x + 5, y, TFT_GREEN);
  M5.Lcd.drawLine(x, y - 5, x, y + 5, TFT_GREEN);
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

#ifndef LOAD_FONT2
//ERROR_Please_enable_LOAD_FONT2_in_User_Setup!
#endif

#ifndef LOAD_FONT4
//ERROR_Please_enable_LOAD_FONT4_in_User_Setup!
#endif

#ifndef LOAD_FONT6
//ERROR_Please_enable_LOAD_FONT6_in_User_Setup!
#endif

#ifndef LOAD_FONT7
//ERROR_Please_enable_LOAD_FONT7_in_User_Setup!
#endif

#ifndef LOAD_FONT8
//ERROR_Please_enable_LOAD_FONT8_in_User_Setup!
#endif

#ifndef LOAD_GFXFF
ERROR_Please_enable_LOAD_GFXFF_in_User_Setup!
#endif

