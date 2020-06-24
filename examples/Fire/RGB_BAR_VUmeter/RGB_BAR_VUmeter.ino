/*
  microphone neopixel VU meter

  Neopixel LEDS flash dependent on the microphone amplitude

   hardwware:  M5StackFire

  please install the Adafruit library first!

  September 2018, ChrisMicro, MIT License

*/

#include <Adafruit_NeoPixel.h>

#define M5STACK_FIRE_NEO_NUM_LEDS  10
#define M5STACK_FIRE_NEO_DATA_PIN  15
#define M5STACKFIRE_MICROPHONE_PIN 34

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(M5STACK_FIRE_NEO_NUM_LEDS, M5STACK_FIRE_NEO_DATA_PIN, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial.begin(115200);
  pixels.begin();
}

#define NUMBEROFSAMPLES 1000
uint16_t micValue[NUMBEROFSAMPLES];

void loop()
{
  uint32_t power = 0;
  uint32_t meanValue = 0;
  for (uint32_t n = 0; n < NUMBEROFSAMPLES; n++)
  {
    int value = analogRead(M5STACKFIRE_MICROPHONE_PIN);
    micValue[n] = value;
    meanValue += value;
    delayMicroseconds(20);
  }
  meanValue /= NUMBEROFSAMPLES;
  for (uint32_t n = 0; n < NUMBEROFSAMPLES; n++)
  {
    power += (micValue[n] - meanValue) * (micValue[n] - meanValue);
  }
  power /= NUMBEROFSAMPLES;

  Serial.println(power);

  int threshold = 1000;
  for (uint8_t n = 0; n < M5STACK_FIRE_NEO_NUM_LEDS; n++)
  {
    pixels.setPixelColor(n, pixels.Color(0, 0, 1));
    if (power > threshold)  pixels.setPixelColor(n, pixels.Color(100, 0, 0));
    threshold *= 5;
  }
  pixels.show();
  delay(10);

}
