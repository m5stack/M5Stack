/* This example shows how to use continuous mode to take
range measurements with the VL53L0X. It is based on
vl53l0x_ContinuousRanging_Example.c from the VL53L0X API.

The range readings are in units of mm. */

#include <Wire.h>
#include <M5Stack.h>
#include <VL53L0X.h>

VL53L0X sensor;
uint32_t sum;
uint16_t num;

void header(const char *string, uint16_t color){
    M5.Lcd.fillScreen(color);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_MAGENTA, TFT_BLUE);
    M5.Lcd.fillRect(0, 0, 320, 30, TFT_BLUE);
    M5.Lcd.setTextDatum(TC_DATUM);
    M5.Lcd.drawString(string, 160, 3, 4);  
}

void setup()
{
  Serial.begin(115200);
  M5.begin();
  Wire.begin();
  header("T O F", TFT_BLACK);
  M5.Lcd.setTextColor(TFT_WHITE,TFT_BLACK);
  
  sensor.init();
  sensor.setTimeout(500);

  // Start continuous back-to-back mode (take readings as
  // fast as possible).  To use continuous timed mode
  // instead, provide a desired inter-measurement period in
  // ms (e.g. sensor.startContinuous(100)).
  sensor.startContinuous();
}


void loop()
{
//  Serial.print(sensor.readRangeContinuousMillimeters());
  M5.Lcd.setCursor(80,100,8);
  for(int i=0;i<10;){
    uint16_t temp = sensor.readRangeContinuousMillimeters();
    delay(40);
    sum += temp;
    i++;
  }

  sum = sum/10;
  M5.Lcd.printf("%d   \n",sum);
  sum = 0;
  if (sensor.timeoutOccurred()) { M5.Lcd.printf("TIMEOUT"); }

}


