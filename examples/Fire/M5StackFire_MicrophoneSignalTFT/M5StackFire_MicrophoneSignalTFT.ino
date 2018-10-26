/************************************************************************

  M5StackFire Discovery simple oscilloscope example

  The oscilloscope has a auto trigger function to achive a stable
  visual impression when a repeating signal like a sine wave is applied.

  original version
  STM32F429 Discovery June 2018, ChrisMicro
  this version reworked for
  M5StackFire         September 2018, ChrisMicro

************************************************************************/
#include <M5Stack.h>

#define M5STACKFIRE_MICROPHONE_PIN 34

#define HORIZONTAL_RESOLUTION 320
#define VERTICAL_RESOLUTION   240
#define POSITION_OFFSET_Y      30
#define SIGNAL_LENGTH HORIZONTAL_RESOLUTION

uint16_t oldSignal[SIGNAL_LENGTH];
uint16_t adcBuffer[SIGNAL_LENGTH];

#define SAMPLING_TIME_US           25
#define ANALOG_SIGNAL_INPUT        M5STACKFIRE_MICROPHONE_PIN

void setup()
{
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
  int y;

  for (n = 1; n < SIGNAL_LENGTH; n++)
  {
    y = VERTICAL_RESOLUTION - adcBuffer[n] * ( VERTICAL_RESOLUTION - POSITION_OFFSET_Y) / 4096 - POSITION_OFFSET_Y / 2  ;

    if (n == 1)
    {
      oldx = n;
      oldy = y;
    }

    if (n < SIGNAL_LENGTH - 1)
    {
      // delete old line element
      M5.Lcd.drawLine(n - 1 , oldSignal[n - 1], n, oldSignal[n], BLACK );

      // draw new line element
      M5.Lcd.drawLine(oldx,               oldy, n,            y, GREEN );
    }

    oldSignal[n - 1] = oldy;
    oldx = n;
    oldy = y;
  }
}

void loop(void)
{
  int n;
  uint32_t nextTime = 0;

  waitForAutoTrigger();

  // record signal
  for (n = 1; n < SIGNAL_LENGTH; n++)
  {
    adcBuffer[n] = analogRead( ANALOG_SIGNAL_INPUT );

    // wait for next sample
    while (micros() < nextTime);
    nextTime = micros() + SAMPLING_TIME_US;
  }

  showSignal();
}
