#include <M5Stack.h>

#define KEYBOARD_I2C_ADDR     0X88
#define KEYBOARD_INT          5

static xQueueHandle keyboard_queue = NULL;

static void IRAM_ATTR keyboard_cb() {
  uint8_t key_val;
  Wire.requestFrom(KEYBOARD_I2C_ADDR, 1);  // request 1 byte from keyboard
  key_val = Wire.read();                  // receive a byte as character
  if(key_val != 0) {
    xQueueSendFromISR(keyboard_queue, &key_val, NULL);
  }
}

void setup()
{
  M5.begin();
  Wire.begin();
  M5.Lcd.setTextFont(2);
  M5.Lcd.println("FACES Keyboard I2C Read Example");

  //create a queue to handle gpio event from isr
  keyboard_queue = xQueueCreate(10, sizeof(uint8_t));
  pinMode(KEYBOARD_INT, INPUT_PULLUP);
  attachInterrupt(KEYBOARD_INT, keyboard_cb, FALLING);
}

void loop() {
  uint8_t key_val;
  // xQueueReceive(keyboard_queue, &key_val, portMAX_DELAY) // block
  if(xQueueReceive(keyboard_queue, &key_val, 0)) { // Non-blocking
      if(key_val > 20 && key_val < 0x7F) { // ASCII String
        Serial.print((char)key_val);
        M5.Lcd.print((char)key_val);
      } else {
        Serial.printf("0x%02X ",key_val);
        M5.Lcd.printf("0x%02X ",key_val);
      }
  }
}