/************************************************************************

  M5StackFire Discovery simple Spectrum example

  The microphone signal is sampled and a frequency analysis is performed.


  Please install the arduinoFFT library.
  You will find it in the library manager or you can get it from github:
  https://github.com/kosme/arduinoFFT


  M5StackFire         September 2018, ChrisMicro

************************************************************************/
#include <M5Stack.h>
#include "arduinoFFT.h"
//#include <WiFi.h>

arduinoFFT FFT = arduinoFFT(); /* Create FFT object */

#define M5STACKFIRE_MICROPHONE_PIN 34
#define M5STACKFIRE_SPEAKER_PIN 25 // speaker DAC, only 8 Bit

#define HORIZONTAL_RESOLUTION 320
#define VERTICAL_RESOLUTION   240
#define POSITION_OFFSET_Y      20
#define SIGNAL_LENGTH 512

double oldSignal[SIGNAL_LENGTH];
double adcBuffer[SIGNAL_LENGTH];
double vImag[SIGNAL_LENGTH];

#define SAMPLINGFREQUENCY 40000
#define SAMPLING_TIME_US     ( 1000000UL/SAMPLINGFREQUENCY )
#define ANALOG_SIGNAL_INPUT        M5STACKFIRE_MICROPHONE_PIN

void setup()
{
  M5.begin();
  //WiFi.mode(WIFI_OFF);
  //btStop();
  dacWrite(M5STACKFIRE_SPEAKER_PIN, 0); // make sure that the speaker is quite
  M5.Lcd.begin();
  M5.Lcd.fillScreen( BLACK );
  M5.Lcd.fillRect(10, 1, 150, 160, BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextColor(GREEN);  //M5.Lcd.setTextSize(3);
  M5.Lcd.setTextSize(1);

  M5.Lcd.println("analog input MIC");
  M5.Lcd.print("max. frequency: "); M5.Lcd.print(SAMPLINGFREQUENCY / 2); M5.Lcd.println(" Hz");
  M5.Lcd.setTextSize(2);
}

void showSignal()
{
  int n;

  int oldx;
  int oldy;
  int oldSig;
  int x, y;

  for (n = 0; n < SIGNAL_LENGTH/2; n++)
  {
    x = n;
    y = map(adcBuffer[n], 0, 512, VERTICAL_RESOLUTION, POSITION_OFFSET_Y);

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

double AdcMeanValue = 0;

void loop(void)
{
  int n;
  uint32_t nextTime = 0;

  // record signal
  for (n = 1; n < SIGNAL_LENGTH; n++)
  {
    double v = analogRead( ANALOG_SIGNAL_INPUT );
    AdcMeanValue += (v - AdcMeanValue) * 0.001;
    adcBuffer[n] = v - AdcMeanValue;

    // wait for next sample
    while (micros() < nextTime);
    nextTime = micros() + SAMPLING_TIME_US;
  }

  FFT.Windowing(adcBuffer, SIGNAL_LENGTH, FFT_WIN_TYP_HAMMING, FFT_FORWARD);  /* Weigh data */
  FFT.Compute(adcBuffer, vImag, SIGNAL_LENGTH, FFT_FORWARD); /* Compute FFT */
  FFT.ComplexToMagnitude(adcBuffer, vImag, SIGNAL_LENGTH); /* Compute magnitudes */
  //int x = FFT.MajorPeak(adcBuffer, SIGNAL_LENGTH, 1000000UL / SAMPLING_TIME_US);//SAMPLINGFREQUENCY
  int x = FFT.MajorPeak(adcBuffer, SIGNAL_LENGTH, SAMPLINGFREQUENCY);

  int maxAmplitudeDB = 0;
  for (n = 1; n < SIGNAL_LENGTH; n++)
  {
    int a = log10(adcBuffer[n]) * 20 - 54.186; // convert amplitude to dB scale, dB relative to log10(512samples)*20=54.186dB
    if (a > maxAmplitudeDB) maxAmplitudeDB = a;
    adcBuffer[n] = (a + 30) * 5; // scale for TFT display
    vImag[n] = 0; // clear imaginary part
  }

  showSignal();

  M5.Lcd.fillRect(200, 0, 119, 40, BLUE);
  M5.Lcd.setCursor(210, 1);
  M5.Lcd.print(x); M5.Lcd.print(" Hz");
  M5.Lcd.setCursor(210, 21);
  M5.Lcd.print(maxAmplitudeDB); M5.Lcd.print(" dB");

}
