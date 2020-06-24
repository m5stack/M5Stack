/*
    Description: Adjust the speed of FAN Unit through PWM.
*/

#include <M5Stack.h>

const int motor_pin = 21;
int freq = 10000;
int ledChannel = 0;
int resolution = 10;
void setup() {
  // put your setup code here, to run once:
  M5.begin();
  M5.Lcd.setCursor(120, 110, 4);
  M5.Lcd.println("MOTOR");
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(motor_pin, ledChannel);

}

void loop() {
  // put your main code here, to run repeatedly:
    ledcWrite(ledChannel, 512);
    delay(1000);
    ledcWrite(ledChannel, 0);
    delay(1000);
}
