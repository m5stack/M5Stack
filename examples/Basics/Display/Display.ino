#include <M5Stack.h>

// the setup routine runs once when M5Stack starts up
void setup() {
  
  // initialize the M5Stack object
  m5.begin();
  m5.lcd.drawBitmap(0, 0, 320, 240, (uint16_t *)gImage_logoM5);
  delay(500);

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

}

// the loop routine runs over and over again forever
void loop(){

  //rand draw 
  m5.Lcd.fillTriangle(random(m5.Lcd.width()-1), random(m5.Lcd.height()-1), random(m5.Lcd.width()-1), random(m5.Lcd.height()-1), random(m5.Lcd.width()-1), random(m5.Lcd.height()-1), random(0xfffe));

  m5.update();
}
