// Example sketch to demonstrate the drawing of X BitMap (XBM)
// format image onto the display.

// Information on the X BitMap (XBM) format can be found here:
// https://en.wikipedia.org/wiki/X_BitMap

// This example is part of the TFT_eSPI library:
// https://github.com/Bodmer/TFT_eSPI

// Created by Bodmer 23/14/18

#include <M5Stack.h>  // Hardware-specific library

#include "xbm.h"  // Sketch tab header for xbm images

void setup() {
    M5.begin();  // Initialise the display
    M5.Power.begin();
    M5.Lcd.fillScreen(TFT_BLACK);  // Black screen fill
}

void loop() {
    // Example 1
    // =========
    // Random x and y coordinates
    int x = random(M5.Lcd.width() - logoWidth);
    int y = random(M5.Lcd.height() - logoHeight);

    // Draw bitmap with top left corner at x,y with foreground only color
    // Bits set to 1 plot as the defined color, bits set to 0 are not plotted
    //              x  y  xbm   xbm width  xbm height  color
    M5.Lcd.drawXBitmap(x, y, logo, logoWidth, logoHeight, TFT_WHITE);

    delay(500);

    // Erase old one by drawing over with background colour
    M5.Lcd.drawXBitmap(x, y, logo, logoWidth, logoHeight, TFT_BLACK);

    // Example 2
    // =========
    // New random x and y coordinates
    x = random(M5.Lcd.width() - logoWidth);
    y = random(M5.Lcd.height() - logoHeight);

    // Draw bitmap with top left corner at x,y with foreground and background
    // colors Bits set to 1 plot as the defined fg color, bits set to 0 are
    // plotted as bg color
    //              x  y  xbm   xbm width  xbm height  fg color   bg color
    M5.Lcd.drawXBitmap(x, y, logo, logoWidth, logoHeight, TFT_WHITE, TFT_RED);

    delay(500);

    // Erase old one by drawing over with background colour
    M5.Lcd.drawXBitmap(x, y, logo, logoWidth, logoHeight, TFT_BLACK, TFT_BLACK);
}
