#include <M5Stack.h>

// the setup routine runs once when M5Stack starts up
void setup(){
  // initialize the M5Stack object
  m5.begin();

  // lcd display
  // m5.lcd.setFont();
  m5.lcd.fillScreen(BLACK);
  m5.lcd.setCursor(10, 10);
  m5.lcd.setTextColor(WHITE);
  m5.lcd.setTextSize(1);
  m5.lcd.printf("Display Test!");

  // delay(1000);
  // m5.lcd.drawRect(100, 100, 50, 50, BLUE);
  // delay(1000);
  // m5.lcd.fillRect(100, 100, 50, 50, BLUE);
  // delay(1000);
  // m5.lcd.fillCircle(100, 100, 50, RED);
  // delay(1000);
  // m5.lcd.fillTriangle(30, 30, 180, 100, 80, 150, YELLOW);
  
  randomSeed(100);
  while(1) {
    m5.lcd.fillTriangle(random(219), random(175), random(219), random(175), random(219), random(175), random(0xfffe));
  }

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

  m5.loop();
}
