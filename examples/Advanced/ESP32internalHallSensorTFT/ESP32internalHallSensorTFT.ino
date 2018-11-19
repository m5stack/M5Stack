/************************************************************************

  M5StackFire internal Hall sensor example

  The ESP32 MCU has build in Hall sensor.
  It is not very suscescible but if you bring a magnet close to the
  upper screen you will see the influence on the measurement.
  In this example we use some low pass filters to get rid of the noise.

  Hardware M5StackFire         
  Oktober 2018, ChrisMicro

************************************************************************/
#include <M5Stack.h>

#define M5STACKFIRE_SPEAKER_PIN 25 // speaker DAC, only 8 Bit

#define HORIZONTAL_RESOLUTION 320
#define VERTICAL_RESOLUTION   240
#define POSITION_OFFSET_Y      20
#define SIGNAL_LENGTH HORIZONTAL_RESOLUTION

uint16_t oldSignal[SIGNAL_LENGTH];
uint16_t adcBuffer[SIGNAL_LENGTH];

float ESP32_hallRead()
{
  float value = 0;
  int count = 400;
  // mean value filter
  for (int n = 0; n < count; n++) value += hallRead();
  return value / count * 10; 
}

float HallOffset = 0;

void setup()
{
  dacWrite(M5STACKFIRE_SPEAKER_PIN, 0); // make sure that the speaker is quite
  M5.Lcd.begin();
  M5.Lcd.fillScreen( BLACK );
  M5.Lcd.fillRect(10, 1, 150, 160, BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(BLUE, BLACK);
  M5.Lcd.setTextSize(2);

  M5.Lcd.println("ESP32 Hall sensor:");
  HallOffset = ESP32_hallRead(); // callibrate the output value to the magnetic field at start up
  M5.Lcd.setTextSize(2);
}

float LowPassFilteredValue=0;
void showSignal()
{
  int n;

  int oldx;
  int oldy;
  int oldSig;
  int x, y;

  for (n = 0; n < SIGNAL_LENGTH; n++)
  {
    x = n;
    float value = ESP32_hallRead()- HallOffset;
    LowPassFilteredValue+=(value-LowPassFilteredValue)*0.05;
    M5.Lcd.setCursor(220, 1);
    M5.Lcd.print((int)LowPassFilteredValue);M5.Lcd.print("    ");
    
    y = map(value , -127, 127, VERTICAL_RESOLUTION, POSITION_OFFSET_Y);

    if (n > 0)
    {
      // delete old line element
      M5.Lcd.drawLine(oldx , oldSig, x, oldSignal[n], BLACK );

      // draw new line element
      if (n < SIGNAL_LENGTH - 1) // don't draw last element because it would generate artifacts
      {
        M5.Lcd.drawLine(oldx,    oldy, x,            y, GREEN );
      }
    }
    oldx = x;
    oldy = y;
    oldSig = oldSignal[n];
    oldSignal[n] = y;
  }
}

void loop(void)
{
  showSignal();
}
