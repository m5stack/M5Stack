/*
    Description: Use WEIGHT Unit to read the analog value returned by the pressure sensor, 
    convert it into intuitive weight data and send it to M5Core,Press ButtonA to calibrate
*/

#include "HX711.h"
#include<M5Stack.h>

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 36;
const int LOADCELL_SCK_PIN = 26;

HX711 scale;
const long LOADCELL_OFFSET = 50682624;
const long LOADCELL_DIVIDER = 5895655;

void setup() {
  M5.begin();
  Serial.begin(115200);
  Serial.println("Initializing the scale");
  M5.Lcd.setCursor(80,100,4);
  M5.Lcd.print("Initializing......");
  // Initialize library with data output pin, clock input pin and gain factor.
  // Channel selection is made by passing the appropriate gain:
  // - With a gain factor of 64 or 128, channel A is selected
  // - With a gain factor of 32, channel B is selected
  // By omitting the gain factor parameter, the library
  // default "128" (Channel A) is used here.
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(LOADCELL_DIVIDER);
  scale.set_offset(LOADCELL_OFFSET);
  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());      // print a raw reading from the ADC
  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));   // print the average of 20 readings from the ADC
  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)
  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
  // by the SCALE parameter (not set yet)
  scale.set_scale(61.2f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0
  Serial.println("After setting up the scale:");
  
  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
            // by the SCALE parameter set with set_scale

  Serial.println("Readings:");
  M5.Lcd.setCursor(55,180,2);
  M5.Lcd.print("pls connect the Weight Unit to PortB");
  M5.Lcd.setCursor(70,210,2);
  M5.Lcd.print("Click button A to calibration");
}

void loop() {
  if (M5.BtnA.wasPressed()) {
    scale.set_offset(LOADCELL_OFFSET + scale.read());
    scale.set_scale(61.2f);
    scale.tare();
  }
  M5.update();
  int weight = scale.get_units(5);
  M5.Lcd.setCursor(40,30,4);
  M5.Lcd.fillRect(0, 30, 320, 30, TFT_BLACK);
  M5.Lcd.printf("%1d g", weight);
  M5.Lcd.fillRect(0, 80, 320, 80, TFT_BLACK);
  M5.Lcd.fillRect(0, 80, weight*0.16, 80, TFT_BLUE);
  Serial.print("average:\t");
  Serial.println(weight, 1);
  
}