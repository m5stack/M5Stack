#include <M5Stack.h>

HardwareSerial Serial2(2);

void setup() {
  
  M5.begin();

  Serial.begin(115200);

  // Serial2.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t txPin, bool invert)
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
}

void loop() {

  if(Serial.available()) {
    int ch = Serial.read();
    Serial2.write(ch);  
  }

  if(Serial2.available()) {
    int ch = Serial2.read();
    Serial.write(ch);
  }
}
