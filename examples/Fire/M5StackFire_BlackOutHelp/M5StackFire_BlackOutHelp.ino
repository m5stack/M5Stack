/************************************************************************

  M5StackFire BlackOut Help

  Here in this region we have a BlackOut.

  Fortunatelly I have the M5StackFire with a little spare energie in its
  batttery. With this I can illumnate my room ...        
  
  4.October 2018, ChrisMicro, Swizerland, close to Zürich

************************************************************************/
#include <M5Stack.h>

#define USENEOLED

#ifdef USENEOLED
  #include <Adafruit_NeoPixel.h>
  
  #define M5STACK_FIRE_NEO_NUM_LEDS 10
  #define M5STACK_FIRE_NEO_DATA_PIN 15
  
  Adafruit_NeoPixel pixels = Adafruit_NeoPixel(M5STACK_FIRE_NEO_NUM_LEDS, M5STACK_FIRE_NEO_DATA_PIN, NEO_GRB + NEO_KHZ800);
#endif

void setup()
{
  M5.Lcd.begin();
  M5.Lcd.fillScreen( WHITE );

  #ifdef USENEOLED
    pixels.begin(); 
    int r=255;
    int g=255;
    int b=255;
    for(int n=0;n<10;n++)pixels.setPixelColor(n, pixels.Color(r, g, b));     
    pixels.show();  
  #endif
}

void loop(void)
{
}
