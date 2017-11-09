#include <M5Stack.h>

HardwareSerial GPSRaw(2);

void setup() {
  
  M5.begin();
  GPSRaw.begin(9600);

  Serial.println("hello");
  termInit();
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
