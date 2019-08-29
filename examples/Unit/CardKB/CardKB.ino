#include <Wire.h>
#include <M5Stack.h>

#define CARDKB_ADDR 0x5F

void setup()
{
  M5.begin();
  M5.Power.begin();
  Serial.begin(115200);
  Wire.begin();
  pinMode(5, INPUT);
  digitalWrite(5, HIGH);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(1, 10);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("IIC Address: 0x5F\n");
  M5.Lcd.printf(">>");
}
void loop()
{
  Wire.requestFrom(CARDKB_ADDR, 1);
  while(Wire.available())
  {
    char c = Wire.read(); // receive a byte as characterif
    if (c != 0)
    {
      M5.Lcd.printf("%c", c);
      Serial.println(c, HEX);
     // M5.Speaker.beep();
    }
  }
  // delay(10);
}
