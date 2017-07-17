#include <M5Stack.h>

// the setup routine runs once when M5Stack starts up
void setup() {
  // initialize the M5Stack object
  m5.begin();
  randomSeed(100);

  // Lcd display
  m5.Lcd.fillScreen(WHITE);
  delay(500);
  m5.Lcd.fillScreen(RED);
  delay(500);
  m5.Lcd.fillScreen(GREEN);
  delay(500);
  m5.Lcd.fillScreen(BLUE);
  delay(500);
  m5.Lcd.fillScreen(BLACK);
  delay(500);

  // text print
  m5.Lcd.setFont();
  m5.Lcd.fillScreen(BLACK);
  m5.Lcd.setCursor(10, 10);
  m5.Lcd.setTextColor(WHITE);
  m5.Lcd.setTextSize(1);
  m5.Lcd.printf("Display Test!");

  // draw graphic
  delay(1000);
  m5.Lcd.drawRect(100, 100, 50, 50, BLUE);
  delay(1000);
  m5.Lcd.fillRect(100, 100, 50, 50, BLUE);
  delay(1000);
  m5.Lcd.drawCircle(100, 100, 50, RED);
  delay(1000);
  m5.Lcd.fillCircle(100, 100, 50, RED);
  delay(1000);
  m5.Lcd.drawTriangle(30, 30, 180, 100, 80, 150, YELLOW);
  delay(1000);
  m5.Lcd.fillTriangle(30, 30, 180, 100, 80, 150, YELLOW);
  
/*
  m5.Lcd.drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
  m5.Lcd.drawCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, uint16_t color);
  m5.Lcd.fillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
  m5.Lcd.fillCircleHelper(int16_t x0, int16_t y0, int16_t r, uint8_t cornername, int16_t delta, uint16_t color);
  m5.Lcd.drawTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  m5.Lcd.fillTriangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color);
  m5.Lcd.drawRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
  m5.Lcd.fillRoundRect(int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
*/
}

// the loop routine runs over and over again forever
void loop(){

  //rand draw 
  // m5.Lcd.fillTriangle(random(219), random(175), random(219), random(175), random(219), random(175), random(0xfffe));
  m5.Lcd.fillTriangle(random(m5.Lcd.width()-1), random(m5.Lcd.height()-1), random(m5.Lcd.width()-1), random(m5.Lcd.height()-1), random(m5.Lcd.width()-1), random(m5.Lcd.height()-1), random(0xfffe));

  m5.update();
}
