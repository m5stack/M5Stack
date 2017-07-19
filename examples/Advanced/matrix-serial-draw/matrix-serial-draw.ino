#include <M5Stack.h>
#include "matrix_rgb.h"
#include <DHT12.h>
#include <Wire.h>     //The DHT12 uses I2C comunication.

HardwareSerial Serial2(2);
MATRIX_RGB matrix;
DHT12 dht12;          //Preset scale CELSIUS and ID 0x5c.


/***************************************************
  This is our GFX example for the Adafruit ILI9341 Breakout and Shield
  ----> http://www.adafruit.com/products/1651

  Check out the links above for our tutorials and wiring diagrams
  These displays use SPI to communicate, 4 or 5 pins are required to
  interface (RST is optional)
  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/

//  void updateEnvLCD(float tmp, float humidty, float pressure, float light) {
 void updateEnvLCD(float tmp, float humidty) {

  // m5.lcd.setFont(&FreeMonoBoldOblique12pt7b);
  // m5.lcd.fillScreen(WHITE);
  m5.lcd.setFont(&FreeSansOblique9pt7b);
  m5.lcd.setTextSize(1);
  m5.lcd.setTextColor(BLACK);
  
  //tmp 
  m5.lcd.fillRect(10, 51, 38, 28, WHITE);
  m5.lcd.setCursor(18, 72);
  m5.lcd.printf("%0.0f", tmp);

  //humidty
  m5.lcd.fillRect(83, 51, 38, 28, WHITE);
  m5.lcd.setCursor(88, 72);
  m5.lcd.printf("%0.0f", humidty);

  // //pressure
  // m5.lcd.fillRect(152, 51, 38, 28, WHITE);
  // m5.lcd.setCursor(153, 72);
  // m5.lcd.printf("%0.0f", pressure);

  // //light 
  // m5.lcd.fillRect(10, 110, 42, 28, WHITE);
  // m5.lcd.setCursor(13, 130);
  // m5.lcd.printf("%3.0f", light);
}

void setup() {
  // Serial2.begin(9600);
  // Serial2.println("ILI9341 Test!"); 
  Serial.begin(74880);
  // Serial.begin(250000);
  Serial2.begin(115200);
  m5.begin();
  Wire.begin();
  matrix.begin();
  // tft.begin();

  // testFillScreen();
  // tft.fillScreen(ILI9341_GREEN);
  matrix.fillScreen(ILI9341_BLACK);
  // testText();



  // testCircles(5, ILI9341_GREEN);
    // draw a pixel in solid white
  matrix.drawPixel(0, 0, matrix.Color333(7, 7, 7)); 
  delay(500);

  // fix the screen with green
  matrix.fillRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(0, 7, 0));
  delay(500);

  // draw a box in yellow
  matrix.drawRect(0, 0, matrix.width(), matrix.height(), matrix.Color333(7, 7, 0));
  delay(500);
  
  // draw an 'X' in red
  matrix.drawLine(0, 0, matrix.width()-1, matrix.height()-1, matrix.Color333(7, 0, 0));
  matrix.drawLine(matrix.width()-1, 0, 0, matrix.height()-1, matrix.Color333(7, 0, 0));
  delay(500);
  
  // draw a blue circle
  matrix.drawCircle(10, 10, 10, matrix.Color333(0, 0, 7));
  delay(500);
  
  // fill a violet circle
  matrix.fillCircle(40, 21, 10, matrix.Color333(7, 0, 7));
  delay(500);
  
  // fill the screen with 'black'
  matrix.fillScreen(matrix.Color333(0, 0, 0));
  
  // draw some text!
  matrix.setTextSize(1);     // size 1 == 8 pixels high
  matrix.setTextWrap(false); // Don't wrap at end of line - will do ourselves

  matrix.setCursor(8, 0);    // start at top left, with 8 pixel of spacing
  uint8_t w = 0;
  char *str = "AdafruitIndustries";
  for (w=0; w<8; w++) {
    matrix.setTextColor(Wheel(w));
    matrix.print(str[w]);
  }
  matrix.setCursor(2, 8);    // next line
  for (w=8; w<18; w++) {
    matrix.setTextColor(Wheel(w));
    matrix.print(str[w]);
  }
  matrix.println();
  //matrix.setTextColor(matrix.Color333(4,4,4));
  //matrix.println("Industries");
  matrix.setTextColor(matrix.Color333(7,7,7));
  matrix.println("LED MATRIX!");
  
  // print each letter with a rainbow color
  matrix.setTextColor(matrix.Color333(7,0,0));
  matrix.print('3');
  matrix.setTextColor(matrix.Color333(7,4,0)); 
  matrix.print('2');
  matrix.setTextColor(matrix.Color333(7,7,0));
  matrix.print('x');
  matrix.setTextColor(matrix.Color333(4,7,0)); 
  matrix.print('6');
  matrix.setTextColor(matrix.Color333(0,7,0));  
  matrix.print('4');
  matrix.setCursor(34, 24);  
  matrix.setTextColor(matrix.Color333(0,7,7)); 
  matrix.print("*");
  matrix.setTextColor(matrix.Color333(0,4,7)); 
  matrix.print('R');
  matrix.setTextColor(matrix.Color333(0,0,7));
  matrix.print('G');
  matrix.setTextColor(matrix.Color333(4,0,7)); 
  matrix.print("B");
  matrix.setTextColor(matrix.Color333(7,0,4)); 
  matrix.println("*");
  
}

// Input a value 0 to 24 to get a color value.
// The colours are a transition r - g - b - back to r.
uint16_t Wheel(byte WheelPos) {
  if(WheelPos < 8) {
   return matrix.Color333(7 - WheelPos, WheelPos, 0);
  } else if(WheelPos < 16) {
   WheelPos -= 8;
   return matrix.Color333(0, 7-WheelPos, WheelPos);
  } else {
   WheelPos -= 16;
   return matrix.Color333(0, WheelPos, 7 - WheelPos);
  }
}


void loop(void) {
  updateEnvLCD(dht12.readTemperature(), dht12.readHumidity());
}

// static const int8_t PROGMEM sinetab[256] = {
//      0,   2,   5,   8,  11,  15,  18,  21,
//     24,  27,  30,  33,  36,  39,  42,  45,
//     48,  51,  54,  56,  59,  62,  65,  67,
//     70,  72,  75,  77,  80,  82,  85,  87,
//     89,  91,  93,  96,  98, 100, 101, 103,
//    105, 107, 108, 110, 111, 113, 114, 116,
//    117, 118, 119, 120, 121, 122, 123, 123,
//    124, 125, 125, 126, 126, 126, 126, 126,
//    127, 126, 126, 126, 126, 126, 125, 125,
//    124, 123, 123, 122, 121, 120, 119, 118,
//    117, 116, 114, 113, 111, 110, 108, 107,
//    105, 103, 101, 100,  98,  96,  93,  91,
//     89,  87,  85,  82,  80,  77,  75,  72,
//     70,  67,  65,  62,  59,  56,  54,  51,
//     48,  45,  42,  39,  36,  33,  30,  27,
//     24,  21,  18,  15,  11,   8,   5,   2,
//      0,  -3,  -6,  -9, -12, -16, -19, -22,
//    -25, -28, -31, -34, -37, -40, -43, -46,
//    -49, -52, -55, -57, -60, -63, -66, -68,
//    -71, -73, -76, -78, -81, -83, -86, -88,
//    -90, -92, -94, -97, -99,-101,-102,-104,
//   -106,-108,-109,-111,-112,-114,-115,-117,
//   -118,-119,-120,-121,-122,-123,-124,-124,
//   -125,-126,-126,-127,-127,-127,-127,-127,
//   -128,-127,-127,-127,-127,-127,-126,-126,
//   -125,-124,-124,-123,-122,-121,-120,-119,
//   -118,-117,-115,-114,-112,-111,-109,-108,
//   -106,-104,-102,-101, -99, -97, -94, -92,
//    -90, -88, -86, -83, -81, -78, -76, -73,
//    -71, -68, -66, -63, -60, -57, -55, -52,
//    -49, -46, -43, -40, -37, -34, -31, -28,
//    -25, -22, -19, -16, -12,  -9,  -6,  -3
// };

// const float radius1  = 16.3, radius2  = 23.0, radius3  = 40.8, radius4  = 44.2,
//             centerx1 = 16.1, centerx2 = 11.6, centerx3 = 23.4, centerx4 =  4.1, 
//             centery1 =  8.7, centery2 =  6.5, centery3 = 14.0, centery4 = -2.9;
// float       angle1   =  0.0, angle2   =  0.0, angle3   =  0.0, angle4   =  0.0;
// long        hueShift =  0;

// void loop() {
//   int           x1, x2, x3, x4, y1, y2, y3, y4, sx1, sx2, sx3, sx4;
//   unsigned char x, y;
//   long          value;

//   sx1 = (int)(cos(angle1) * radius1 + centerx1);
//   sx2 = (int)(cos(angle2) * radius2 + centerx2);
//   sx3 = (int)(cos(angle3) * radius3 + centerx3);
//   sx4 = (int)(cos(angle4) * radius4 + centerx4);
//   y1  = (int)(sin(angle1) * radius1 + centery1);
//   y2  = (int)(sin(angle2) * radius2 + centery2);
//   y3  = (int)(sin(angle3) * radius3 + centery3);
//   y4  = (int)(sin(angle4) * radius4 + centery4);

//   for(y=0; y<matrix.height(); y++) {
//     x1 = sx1; x2 = sx2; x3 = sx3; x4 = sx4;
//     for(x=0; x<matrix.width(); x++) {
//       value = hueShift
//         + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x1 * x1 + y1 * y1) >> 2))
//         + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x2 * x2 + y2 * y2) >> 2))
//         + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x3 * x3 + y3 * y3) >> 3))
//         + (int8_t)pgm_read_byte(sinetab + (uint8_t)((x4 * x4 + y4 * y4) >> 3));
//       matrix.drawPixel(x, y, matrix.ColorHSV(value * 3, 255, 255, true));
//       x1--; x2--; x3--; x4--;
//     }
//     y1--; y2--; y3--; y4--;
//   }

//   angle1 += 0.03;
//   angle2 -= 0.07;
//   angle3 += 0.13;
//   angle4 -= 0.15;
//   hueShift += 2;
// }


// unsigned long testFillScreen() {
//   unsigned long start = micros();
//   tft.fillScreen(ILI9341_BLACK);
//   yield();
//   tft.fillScreen(ILI9341_RED);
//   yield();
//   tft.fillScreen(ILI9341_GREEN);
//   yield();
//   tft.fillScreen(ILI9341_BLUE);
//   yield();
//   tft.fillScreen(ILI9341_BLACK);
//   yield();
//   return micros() - start;
// }

// unsigned long testCircles(uint8_t radius, uint16_t color) {
//   unsigned long start;
//   int           x, y, r2 = radius * 2,
//                 w = tft.width()  + radius,
//                 h = tft.height() + radius;

//   // Screen is not cleared for this one -- this is
//   // intentional and does not affect the reported time.
//   start = micros();
//   for(x=0; x<w; x+=r2) {
//     for(y=0; y<h; y+=r2) {
//       tft.drawCircle(x, y, radius, color);
//     }
//   }

//   return micros() - start;
// }

// unsigned long testText() {
//   tft.fillScreen(ILI9340_BLACK);
//   unsigned long start = micros();
//   tft.setCursor(0, 0);
//   tft.setTextColor(ILI9340_WHITE);  tft.setTextSize(1);
//   tft.println("Hello World!");
//   tft.setTextColor(ILI9340_YELLOW); tft.setTextSize(2);
//   tft.println(1234.56);
//   tft.setTextColor(ILI9340_RED);    tft.setTextSize(3);
//   tft.println(0xDEADBEEF, HEX);
//   tft.println();
//   tft.setTextColor(ILI9340_GREEN);
//   tft.setTextSize(5);
//   tft.println("Groop");
//   tft.setTextSize(2);
//   tft.println("I implore thee,");
//   tft.setTextSize(1);
//   tft.println("my foonting turlingdromes.");
//   tft.println("And hooptiously drangle me");
//   tft.println("with crinkly bindlewurdles,");
//   tft.println("Or I will rend thee");
//   tft.println("in the gobberwarts");
//   tft.println("with my blurglecruncheon,");
//   tft.println("see if I don't!");
//   return micros() - start;
// }

