// A fun MATRIX-like screen demo of scrolling
// Screen will flicker initially until fully drawn
// then scroll smoothly

// Needs GLCD font

/*
 Make sure all the display driver and pin comnenctions are correct by
 editting the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
*/

#include <M5Stack.h>

#define TEXT_HEIGHT 8 // Height of text to be printed and scrolled
#define BOT_FIXED_AREA 0  // Number of lines in bottom fixed area (lines counted from bottom of screen)
#define TOP_FIXED_AREA 0  // Number of lines in top fixed area (lines counted from top of screen)

uint16_t yStart = TOP_FIXED_AREA;
uint16_t yArea = 320 - TOP_FIXED_AREA - BOT_FIXED_AREA;
uint16_t yDraw = 320 - BOT_FIXED_AREA - TEXT_HEIGHT;
byte     pos[42];
uint16_t xPos = 0;

void setup() {
  randomSeed(analogRead(A0));
  M5.begin();
  M5.Power.begin();
  // M5.Lcd.setRotation(2);
  M5.Lcd.fillScreen(ILI9341_BLACK);
  setupScrollArea(TOP_FIXED_AREA, BOT_FIXED_AREA);
}

void loop(void) {
  // First fill the screen with random streaks of characters
  for (int j = 0; j < 600; j += TEXT_HEIGHT) {
    for (int i = 0; i < 40; i++) {
      if (pos[i] > 20) pos[i] -= 3; // Rapid fade initially brightness values
      if (pos[i] > 0) pos[i] -= 1; // Slow fade later
      if ((random(20) == 1) && (j<400)) pos[i] = 63; // ~1 in 20 probability of a new character
      M5.Lcd.setTextColor(pos[i] << 5, ILI9341_BLACK); // Set the green character brightness
      if (pos[i] == 63) M5.Lcd.setTextColor(ILI9341_WHITE, ILI9341_BLACK); // Draw white character
      xPos += M5.Lcd.drawChar(random(32, 128), xPos, yDraw, 1); // Draw the character
    }
    yDraw = scroll_slow(TEXT_HEIGHT, 14); // Scroll, 14ms per pixel line
    xPos = 0;
  }

  //M5.Lcd.setRotation(2);
  //M5.Lcd.setTextColor(63 << 5, ILI9341_BLACK);
  //M5.Lcd.drawCentreString("MATRIX",120,60,4);
  //M5.Lcd.setRotation(0);

  // Now scroll smoothly forever
  while (1) {yield(); yDraw = scroll_slow(320,5); }// Scroll 320 lines, 5ms per line

}

void setupScrollArea(uint16_t TFA, uint16_t BFA) {
  M5.Lcd.writecommand(ILI9341_VSCRDEF); // Vertical scroll definition
  M5.Lcd.writedata(TFA >> 8);
  M5.Lcd.writedata(TFA);
  M5.Lcd.writedata((320 - TFA - BFA) >> 8);
  M5.Lcd.writedata(320 - TFA - BFA);
  M5.Lcd.writedata(BFA >> 8);
  M5.Lcd.writedata(BFA);
}

int scroll_slow(int lines, int wait) {
  int yTemp = yStart;
  for (int i = 0; i < lines; i++) {
    yStart++;
    if (yStart == 320 - BOT_FIXED_AREA) yStart = TOP_FIXED_AREA;
    scrollAddress(yStart);
    delay(wait);
  }
  return  yTemp;
}

void scrollAddress(uint16_t VSP) {
  M5.Lcd.writecommand(ILI9341_VSCRSADD); // Vertical scrolling start address
  M5.Lcd.writedata(VSP >> 8);
  M5.Lcd.writedata(VSP);
}



