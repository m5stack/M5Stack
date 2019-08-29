#include <M5Stack.h>
#include <Wire.h>

void setup() {
  M5.begin();
  M5.Power.begin();
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(3);
  M5.Lcd.clear(BLACK);
  M5.Lcd.setCursor(120, 100);
}

uint16_t result;
float temperature;

void loop() {
  Wire.beginTransmission(0x5A);          // Send Initial Signal and I2C Bus Address
  Wire.write(0x07);                      // Send data only once and add one address automatically.
  Wire.endTransmission(false);           // Stop signal
  Wire.requestFrom(0x5A, 2);             // Get 2 consecutive data from 0x5A, and the data is stored only.
  result = Wire.read();                  // Receive DATA
  result |= Wire.read() << 8;            // Receive DATA
  
  temperature = result * 0.02 - 273.15;
  
  M5.Lcd.fillRect(120,100,120,100,BLACK);
  M5.Lcd.setCursor(120, 100);
  
  M5.Lcd.print(temperature);
  Serial.println(temperature);

  delay(500);
  M5.update();
}
