#include <M5Stack.h>

// the setup routine runs once when M5Stack starts up
void setup(){
  // initialize the M5Stack object
  m5.begin();
  randomSeed(100);

  // lcd display
  m5.lcd.fillScreen(ILI9341_WHITE);
  delay(500);
  m5.lcd.fillScreen(ILI9341_RED);
  delay(500);
  m5.lcd.fillScreen(ILI9341_GREEN);
  delay(500);
  m5.lcd.fillScreen(ILI9341_BLUE);
  delay(500);
  m5.lcd.fillScreen(ILI9341_BLACK);
  delay(500);

  // text print
  m5.lcd.setFont();
  m5.lcd.fillScreen(ILI9341_BLACK);
  m5.lcd.setCursor(10, 10);
  m5.lcd.setTextColor(ILI9341_WHITE);
  m5.lcd.setTextSize(1);
  m5.lcd.printf("Display Test!");

  // draw graphic
  delay(1000);
  m5.lcd.drawRect(100, 100, 50, 50, ILI9341_BLUE);
  delay(1000);
  m5.lcd.fillRect(100, 100, 50, 50, ILI9341_BLUE);
  delay(1000);
  m5.lcd.drawCircle(100, 100, 50, ILI9341_RED);
  delay(1000);
  m5.lcd.fillCircle(100, 100, 50, ILI9341_RED);
  delay(1000);
  m5.lcd.drawTriangle(30, 30, 180, 100, 80, 150, ILI9341_YELLOW);
  delay(1000);
  m5.lcd.fillTriangle(30, 30, 180, 100, 80, 150, ILI9341_YELLOW);
  
/*
  m5.lcd.drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
  m5.lcd.drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
  m5.lcd.fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
  m5.lcd.fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
  m5.lcd.drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  m5.lcd.fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  m5.lcd.drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
  m5.lcd.fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
*/
}

// the loop routine runs over and over again forever
void loop(){

  //rand draw 
  // m5.lcd.fillTriangle(random(219), random(175), random(219), random(175), random(219), random(175), random(0xfffe));
  m5.lcd.fillTriangle(random(319), random(239), random(319), random(239), random(319), random(239), random(0xfffe));

  m5.loop();
}
