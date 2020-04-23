/*
    Description: The command data of the USB UART  is forwarded to the GPS Unit for debugging and functional verification.
*/
#include <M5Stack.h>

HardwareSerial GPSRaw(2);

void setup() {
  
  M5.begin();
  M5.Power.begin();
  GPSRaw.begin(9600);

  Serial.println("hello");
  termInit();
  M5.Lcd.setTextFont(4);
  M5.Lcd.setCursor(50, 100, 4);
  M5.Lcd.println(("GPS Raw Example"));
} 

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()) {
    int ch = Serial.read();
    GPSRaw.write(ch);  
  }

  if(GPSRaw.available()) {
    int ch = GPSRaw.read();
    Serial.write(ch);
    termPutchar(ch);
  }
}
