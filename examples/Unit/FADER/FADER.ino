/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/unit/fader
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/unit/fader
*
* describe：UNIT FADER. 滑动电位器/推子
* date：2021/8/20
*******************************************************************************
  Connect UNIT FADER to port B and push the FADER slider to adjust the input value and light brightness
  将UNIT FADER连接到B端口, 推动FADER滑杆即可实现调整输入数值大小与灯光亮度
*/

#include "M5Stack.h"
#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 14
#define INPUT_PINS 36

#define DATA_PIN 26

// Define the array of leds
CRGB leds[NUM_LEDS];

uint8_t beginHue = 0;
uint8_t deltaHue = 30;
uint8_t brightness = 100;
uint16_t rawADC = 0;

void setup()
{
  M5.begin();
  M5.Lcd.setTextDatum(MC_DATUM);
  M5.Lcd.drawString("FADER UNIT TEST", 160, 60, 4);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  delay(1000);
  pinMode(36, INPUT);
  fill_rainbow(leds, NUM_LEDS, beginHue, deltaHue); 
}


void loop()
{
  rawADC = analogRead(INPUT_PINS); //Read ADC value 读取ADC数值
  brightness = map(rawADC, 0, 4095, 0, 255); //The mapping ADC value is the brightness value range  映射ADC值为亮度值范围
  FastLED.setBrightness(brightness); //Adjust the brightness of the FADER LED 调整FADER LED灯亮度
  FastLED.show();
  Serial.printf("%d\r\n", rawADC);
  M5.Lcd.fillRect(0, 120, 320, 100, BLACK);
  M5.Lcd.drawString("value: "+String(rawADC), 160, 160, 4);
  delay(100);
}