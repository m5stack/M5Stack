/*
    Description: Use BPS Unit to read atmospheric pressure, and display the data on the screen.
*/

#include "bmp280.h"
#include <M5Stack.h>

#define P0 1013.25

BMP280 bmp;

void display_result(double t, double p, double a) {
//  M5.Lcd.
  M5.Lcd.drawString("Temperature: ", 20, 40, 4 );
  M5.Lcd.drawFloat(t, 2, 180, 40, 4);
  M5.Lcd.drawString("C", 280, 40, 4);

  M5.Lcd.drawString("Pressure: ", 20, 80, 4);
  M5.Lcd.drawFloat(p, 2, 180, 80, 4);
  M5.Lcd.drawString("Pa", 280, 80, 4);

  M5.Lcd.drawString("Altitude: ", 20, 120, 4);
  M5.Lcd.drawFloat(a, 2, 180, 120, 4);
  M5.Lcd.drawString("M", 280, 120, 4);
}

void setup()
{
  M5.begin();
  if(!bmp.begin()){
    Serial.println("BMP init failed!");
    while(1);
  }
  else Serial.println("BMP init success!");
  
  bmp.setOversampling(4);
  M5.Lcd.drawString("BPS Example", 90, 0, 4);
  
}
void loop()
{
  double T,P;
  char result = bmp.startMeasurment();
 
  if(result!=0){
    delay(result);
    result = bmp.getTemperatureAndPressure(T,P);
    
      if(result!=0)
      {
        double A = bmp.altitude(P,P0);
        
        Serial.print("T = \t");Serial.print(T,2); Serial.print(" degC\t");
        Serial.print("P = \t");Serial.print(P,2); Serial.print(" mBar\t");
        Serial.print("A = \t");Serial.print(A,2); Serial.println(" m");
        display_result(T, P, A);
      }
      else {
        Serial.println("Error.");
      }
  }
  else {
    Serial.println("Error.");
  }
  
  delay(2000);
  M5.Lcd.fillRect(160, 40, 100, 80, BLACK);
}
