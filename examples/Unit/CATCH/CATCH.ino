/*
    Description: Control Catch Unit through PWM.
*/

#include <M5Stack.h>

const int servoPin = 26;
int freq = 50;
int ledChannel = 0;
int resolution = 10;
void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Power.begin();
  M5.Lcd.setCursor(100, 50, 4);
  M5.Lcd.println("Catch Unit");
  M5.Lcd.setCursor(40, 120, 4);
  M5.Lcd.println("Connect to the Port B");
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(servoPin, ledChannel);
}

void loop() {
  // High level 0.5ms is angle 0°
  // duty = 0.5/20ms = 0.025, 0.025*1023≈25
    ledcWrite(ledChannel, 25);
    delay(2000);
  // High level 1ms is angle 45°
  // duty = 1/20ms = 0.05, 0.05*1023≈50
    ledcWrite(ledChannel, 50);
    delay(2000);
}
