#define USE_INTERNAL_HZK

#include <M5Stack.h>
#include "str.h"

void setup() {
  M5.begin();
  // Use HZK16 and ASC16 font set
  // NOTE: font file should be copied to your TF card.
  M5.Lcd.loadHzk16("/Fonts/HZK16","/Fonts/ASC16");
  // Set text with white foreground color and black background color
  M5.Lcd.setTextColor(WHITE, BLACK);
  // Set text with red highlight color
  M5.Lcd.setHighlightColor(RED);
  
  // Set text size to 1
  M5.Lcd.setTextSize(1);
  M5.Lcd.setCursor(0,10);
  // Print some ASCII text on the screen
  M5.Lcd.printf(AscStr);
  delay(1000);
  // Highlight the text
  M5.Lcd.highlight(true);
  M5.Lcd.setCursor(0,10);
  M5.Lcd.printf(AscStr);
  
  delay(1000);
  // Clear screen
  M5.Lcd.clear();
  // Turn highlight off
  M5.Lcd.highlight(false);
  M5.Lcd.setCursor(0,10);
  // print some GBK text on the screen
  M5.Lcd.printf(GbkStr);  
  delay(1000);
  M5.Lcd.highlight(true);
  M5.Lcd.setCursor(0,10);
  M5.Lcd.printf(GbkStr);
  
  delay(1000);
  M5.Lcd.clear();
  
  // Set text size to 2
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0,10);
  M5.Lcd.highlight(false);
  M5.Lcd.printf(AscStr);
  delay(1000);
  M5.Lcd.highlight(true);
  M5.Lcd.setCursor(0,10);
  M5.Lcd.printf(AscStr);  
  delay(1000);
  
  M5.Lcd.clear();
  M5.Lcd.highlight(false);
  M5.Lcd.setCursor(0,10);
  M5.Lcd.printf(GbkStr);
  delay(1000);
  M5.Lcd.highlight(true);
  M5.Lcd.setCursor(0,10);
  M5.Lcd.printf(GbkStr);
  
  delay(1000);
  
  M5.Lcd.clear();
  M5.Lcd.highlight(false);
  
  // Set text with non-transparent back color, BLACK as set at the beginning.
  M5.Lcd.setTransparentBgColor(false);  
  M5.Lcd.setCursor(0,10);
  M5.Lcd.printf(GbkStr);
  
  // Set text with transparent back color
  M5.Lcd.setTransparentBgColor(true);
  M5.Lcd.setCursor(0,130);
  M5.Lcd.printf(GbkStr);
}

void loop() {
  // put your main code here, to run repeatedly:

}
