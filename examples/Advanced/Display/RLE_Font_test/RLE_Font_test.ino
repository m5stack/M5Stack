/*
 Display all the fonts.

 This sketch uses the GLCD (font 1) and fonts 2, 4, 6, 7, 8
 
 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 ######       TO SELECT THE FONTS AND PINS YOU USE, SEE ABOVE       ######
 #########################################################################

 */

// New background colour
#define TFT_BROWN 0x38E0

// Pause in milliseconds between screens, change to 0 to time font rendering
#define WAIT 1000

#include <M5Stack.h>

unsigned long targetTime = 0; // Used for testing draw times

void setup(void) {
  M5.begin();
  M5.Power.begin();
}

void loop() {
  targetTime = millis();

  // First we test them with a background colour set
  M5.Lcd.setTextSize(1);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);

  M5.Lcd.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
  M5.Lcd.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 16, 2);
  M5.Lcd.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 32, 2);
  M5.Lcd.drawString("abcdefghijklmnopqrstuvw", 0, 48, 2);
  int xpos = 0;
  xpos += M5.Lcd.drawString("xyz{|}~", 0, 64, 2);
  M5.Lcd.drawChar(127, xpos, 64, 2);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);

  M5.Lcd.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
  M5.Lcd.drawString("/0123456789:;", 0, 26, 4);
  M5.Lcd.drawString("<=>?@ABCDE", 0, 52, 4);
  M5.Lcd.drawString("FGHIJKLMNO", 0, 78, 4);
  M5.Lcd.drawString("PQRSTUVWX", 0, 104, 4);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.drawString("YZ[\\]^_`abc", 0, 0, 4);
  M5.Lcd.drawString("defghijklmno", 0, 26, 4);
  M5.Lcd.drawString("pqrstuvwxyz", 0, 52, 4);
  xpos = 0;
  xpos += M5.Lcd.drawString("{|}~", 0, 78, 4);
  M5.Lcd.drawChar(127, xpos, 78, 4);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_BLUE, TFT_BLACK);

  M5.Lcd.drawString("012345", 0, 0, 6);
  M5.Lcd.drawString("6789", 0, 40, 6);
  M5.Lcd.drawString("apm-:.", 0, 80, 6);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);

  M5.Lcd.drawString("0123", 0, 0, 7);
  M5.Lcd.drawString("4567", 0, 60, 7);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.drawString("890:.", 0, 0, 7);
  M5.Lcd.drawString("", 0, 60, 7);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);

  M5.Lcd.drawString("0123", 0, 0, 8);
  M5.Lcd.drawString("4567", 0, 72, 8);
  delay(WAIT);;

  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.drawString("890:.", 0, 0, 8);
  M5.Lcd.drawString("", 0, 72, 8);
  delay(WAIT);;

  M5.Lcd.setTextSize(2);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);

  M5.Lcd.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
  M5.Lcd.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 32, 2);
  M5.Lcd.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 64, 2);
  M5.Lcd.drawString("abcdefghijklmnopqrstuvw", 0, 96, 2);
  xpos = 0;
  xpos += M5.Lcd.drawString("xyz{|}~", 0, 128, 2);
  M5.Lcd.drawChar(127, xpos, 128, 2);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);

  M5.Lcd.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
  M5.Lcd.drawString("/0123456789:;", 0, 52, 4);
  M5.Lcd.drawString("<=>?@ABCDE", 0, 104, 4);
  M5.Lcd.drawString("FGHIJKLMNO", 0, 156, 4);
  M5.Lcd.drawString("PQRSTUVWX", 0, 208, 4);
  delay(WAIT);
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.drawString("YZ[\\]^_`abc", 0, 0, 4);
  M5.Lcd.drawString("defghijklmno", 0, 52, 4);
  M5.Lcd.drawString("pqrstuvwxyz", 0, 104, 4);
  xpos = 0;
  xpos += M5.Lcd.drawString("{|}~", 0, 156, 4);
  M5.Lcd.drawChar(127, xpos, 156, 4);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_BLUE, TFT_BLACK);

  M5.Lcd.drawString("01234", 0, 0, 6);
  M5.Lcd.drawString("56789", 0, 80, 6);
  M5.Lcd.drawString("apm-:.", 0, 160, 6);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);

  M5.Lcd.drawString("0123", 0, 0, 7);
  M5.Lcd.drawString("4567", 0, 120, 7);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.drawString("890:.", 0, 0, 7);
  M5.Lcd.drawString("", 0, 120, 7);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);

  M5.Lcd.drawString("0123", 0, 0, 8);
  M5.Lcd.drawString("4567", 0, 144, 8);
  delay(WAIT);;

  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.drawString("890:.", 0, 0, 8);
  M5.Lcd.drawString("", 0, 144, 8);
  delay(WAIT);;

  M5.Lcd.setTextColor(TFT_MAGENTA, TFT_BROWN);

  M5.Lcd.drawNumber(millis() - targetTime, 0, 180, 4);
  delay(4000);;

  // Now test them with transparent background
  targetTime = millis();

  M5.Lcd.setTextSize(1);
  M5.Lcd.fillScreen(TFT_BROWN);
  M5.Lcd.setTextColor(TFT_GREEN);

  M5.Lcd.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
  M5.Lcd.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 16, 2);
  M5.Lcd.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 32, 2);
  M5.Lcd.drawString("abcdefghijklmnopqrstuvw", 0, 48, 2);
  xpos = 0;
  xpos += M5.Lcd.drawString("xyz{|}~", 0, 64, 2);
  M5.Lcd.drawChar(127, xpos, 64, 2);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BROWN);
  M5.Lcd.setTextColor(TFT_GREEN);

  M5.Lcd.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
  M5.Lcd.drawString("/0123456789:;", 0, 26, 4);
  M5.Lcd.drawString("<=>?@ABCDE", 0, 52, 4);
  M5.Lcd.drawString("FGHIJKLMNO", 0, 78, 4);
  M5.Lcd.drawString("PQRSTUVWX", 0, 104, 4);

  delay(WAIT);
  M5.Lcd.fillScreen(TFT_BROWN);
  M5.Lcd.drawString("YZ[\\]^_`abc", 0, 0, 4);
  M5.Lcd.drawString("defghijklmno", 0, 26, 4);
  M5.Lcd.drawString("pqrstuvwxyz", 0, 52, 4);
  xpos = 0;
  xpos += M5.Lcd.drawString("{|}~", 0, 78, 4);
  M5.Lcd.drawChar(127, xpos, 78, 4);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BROWN);
  M5.Lcd.setTextColor(TFT_BLUE);

  M5.Lcd.drawString("012345", 0, 0, 6);
  M5.Lcd.drawString("6789", 0, 40, 6);
  M5.Lcd.drawString("apm-:.", 0, 80, 6);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BROWN);
  M5.Lcd.setTextColor(TFT_RED);

  M5.Lcd.drawString("0123", 0, 0, 7);
  M5.Lcd.drawString("4567", 0, 60, 7);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BROWN);
  M5.Lcd.drawString("890:.", 0, 0, 7);
  M5.Lcd.drawString("", 0, 60, 7);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BROWN);
  M5.Lcd.setTextColor(TFT_YELLOW);

  M5.Lcd.drawString("0123", 0, 0, 8);
  M5.Lcd.drawString("4567", 0, 72, 8);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BROWN);
  M5.Lcd.drawString("890:.", 0, 0, 8);
  M5.Lcd.drawString("", 0, 72, 8);
  delay(WAIT);

  M5.Lcd.setTextSize(2);
  M5.Lcd.fillScreen(TFT_BROWN);
  M5.Lcd.setTextColor(TFT_GREEN);

  M5.Lcd.drawString(" !\"#$%&'()*+,-./0123456", 0, 0, 2);
  M5.Lcd.drawString("789:;<=>?@ABCDEFGHIJKL", 0, 32, 2);
  M5.Lcd.drawString("MNOPQRSTUVWXYZ[\\]^_`", 0, 64, 2);
  M5.Lcd.drawString("abcdefghijklmnopqrstuvw", 0, 96, 2);
  xpos = 0;
  xpos += M5.Lcd.drawString("xyz{|}~", 0, 128, 2);
  M5.Lcd.drawChar(127, xpos, 128, 2);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BROWN);
  M5.Lcd.setTextColor(TFT_GREEN);

  M5.Lcd.drawString(" !\"#$%&'()*+,-.", 0, 0, 4);
  M5.Lcd.drawString("/0123456789:;", 0, 52, 4);
  M5.Lcd.drawString("<=>?@ABCDE", 0, 104, 4);
  M5.Lcd.drawString("FGHIJKLMNO", 0, 156, 4);
  M5.Lcd.drawString("PQRSTUVWX", 0, 208, 4);
  delay(WAIT);
  M5.Lcd.fillScreen(TFT_BROWN);
  M5.Lcd.drawString("YZ[\\]^_`abc", 0, 0, 4);
  M5.Lcd.drawString("defghijklmno", 0, 52, 4);
  M5.Lcd.drawString("pqrstuvwxyz", 0, 104, 4);
  xpos = 0;
  xpos += M5.Lcd.drawString("{|}~", 0, 156, 4);
  M5.Lcd.drawChar(127, xpos, 156, 4);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BROWN);
  M5.Lcd.setTextColor(TFT_BLUE);

  M5.Lcd.drawString("01234", 0, 0, 6);
  M5.Lcd.drawString("56789", 0, 80, 6);
  M5.Lcd.drawString("apm-:.", 0, 160, 6);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BROWN);
  M5.Lcd.setTextColor(TFT_RED);

  M5.Lcd.drawString("0123", 0, 0, 7);
  M5.Lcd.drawString("4567", 0, 120, 7);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BROWN);
  M5.Lcd.drawString("890:.", 0, 0, 7);
  M5.Lcd.drawString("", 0, 120, 7);
  delay(WAIT);

  M5.Lcd.fillScreen(TFT_BROWN);
  M5.Lcd.setTextColor(TFT_YELLOW);

  M5.Lcd.drawString("0123", 0, 0, 8);
  M5.Lcd.drawString("4567", 0, 144, 8);
  delay(WAIT);;

  M5.Lcd.fillScreen(TFT_BROWN);
  M5.Lcd.drawString("890:.", 0, 0, 8);
  M5.Lcd.drawString("", 0, 144, 8);
  delay(WAIT);;

  M5.Lcd.setTextColor(TFT_MAGENTA);

  M5.Lcd.drawNumber(millis() - targetTime, 0, 180, 4);
  delay(4000);;
}

