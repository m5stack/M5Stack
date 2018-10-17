/************************************************************************

  M5StackFire simple oscilloscope example

  The oscilloscope has a auto trigger function to achive a stable
  visual impression when a repeating signal like a sine wave is applied.

  original version
  STM32F429 Discovery June 2018, ChrisMicro
  this version reworked for
  M5StackFire         September 2018, ChrisMicro

************************************************************************/
#include <M5Stack.h>

#define M5STACKFIRE_MICROPHONE_PIN 34
#define M5STACKFIRE_SPEAKER_PIN 25 // speaker DAC, only 8 Bit

#define HORIZONTAL_RESOLUTION 320
#define VERTICAL_RESOLUTION   240
#define POSITION_OFFSET_Y      20
#define SIGNAL_LENGTH HORIZONTAL_RESOLUTION

uint16_t oldSignal[SIGNAL_LENGTH];
uint16_t adcBuffer[SIGNAL_LENGTH];

#define SAMPLING_TIME_US           25
#define ANALOG_SIGNAL_INPUT        M5STACKFIRE_MICROPHONE_PIN

void setup()
{
  dacWrite(M5STACKFIRE_SPEAKER_PIN, 0); // make sure that the speaker is quite
  M5.Lcd.begin();
  M5.Lcd.fillScreen( BLACK );
  M5.Lcd.fillRect(10, 1, 150, 160, BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(GREEN);  //M5.Lcd.setTextSize(3);
  M5.Lcd.setTextSize(1);

  M5.Lcd.println("analog input MIC");
  M5.Lcd.print("sampling frequency: "); M5.Lcd.print(1000000 / SAMPLING_TIME_US); M5.Lcd.println(" Hz");
}

void waitForAutoTrigger()
{
  uint32_t triggerLevel = 512;
  uint32_t hysteresis   =  10;
  uint32_t timeOut_ms   = 100;

  uint32_t timeOutLimit = millis() + timeOut_ms;
  uint32_t timeOutFlag = false;

  uint32_t adcValue = 0;

  adcValue = analogRead( ANALOG_SIGNAL_INPUT );

  // wait for low level
  while ( ( adcValue > triggerLevel - hysteresis ) && !timeOutFlag )
  {
    adcValue = analogRead( ANALOG_SIGNAL_INPUT );

    if ( millis() > timeOutLimit ) timeOutFlag = 1 ;
  }

  if ( !timeOutFlag )
  {
    // wait for high level
    while ( ( adcValue < triggerLevel + hysteresis ) && ( millis() < timeOutLimit ) )
    {
      adcValue = analogRead(ANALOG_SIGNAL_INPUT);
    }
  }

}

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
    y = map(adcBuffer[n], 0, 4096, VERTICAL_RESOLUTION, POSITION_OFFSET_Y);

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
  int n;
  uint32_t nextTime = 0;

  waitForAutoTrigger();

  // record signal
  for (n = 0; n < SIGNAL_LENGTH; n++)
  {
    adcBuffer[n] = analogRead( ANALOG_SIGNAL_INPUT );

    // wait for next sample
    while (micros() < nextTime);
    nextTime = micros() + SAMPLING_TIME_US;
  }

  showSignal();
}
