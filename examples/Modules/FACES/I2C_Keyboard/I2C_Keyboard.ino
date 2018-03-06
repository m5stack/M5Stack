#include <M5Stack.h>

#define KEYBOARD_I2C_ADDR     0X88
#define KEYBOARD_INT          5

void setup()
{
  M5.begin();
  Wire.begin();
  M5.Lcd.setTextFont(2);
  M5.Lcd.println("FACES Keyboard I2C Read Example");
  pinMode(KEYBOARD_INT, INPUT_PULLUP);
}

void loop()
{
  if(digitalRead(KEYBOARD_INT) == LOW) {
    Wire.requestFrom(KEYBOARD_I2C_ADDR, 1);  // request 1 byte from keyboard
    while (Wire.available()) { 
      uint8_t key_val = Wire.read();                  // receive a byte as character
      if(key_val != 0) {
        if(key_val > 20 && key_val < 0x7F) { // ASCII String
          Serial.print((char)key_val);
          M5.Lcd.print((char)key_val);
        } else {
          Serial.printf("0x%02X ",key_val);
          M5.Lcd.printf("0x%02X ",key_val);
        }
      }
    }
  }
}