/*
    Description: Measure current and display
    EEPROM (0x51) has built-in calibration parameters when leaving the factory.
    Please do not write to the EEPROM, otherwise the calibration data will be overwritten and the measurement results will be inaccurate.
*/

#include <Wire.h>
#include "ammeter.h"
#include "M5Stack.h"

Ammeter ammeter;

float page512_volt = 2000.0F;

int16_t volt_raw_list[10];
uint8_t raw_now_ptr = 0;
int16_t adc_raw = 0;

int16_t hope = 0.0;

ammeterGain_t now_gain = PAG_512;

void setup(void) {
  M5.begin();
  Wire.begin();

  ammeter.setMode(SINGLESHOT);
  ammeter.setRate(RATE_8);
  ammeter.setGain(PAG_512);
  hope = page512_volt / ammeter.resolution;
  // | PAG      | Max Input Voltage(V) |
  // | PAG_6144 |        128           |
  // | PAG_4096 |        64            |
  // | PAG_2048 |        32            |
  // | PAG_512  |        16            |
  // | PAG_256  |        8             |

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextFont(4);

  M5.Lcd.setCursor(52, 210);
  M5.Lcd.printf("2A");


  M5.Lcd.setCursor(230, 210);
  M5.Lcd.printf("SAVE");

  // bool result1 = ammeter.saveCalibration2EEPROM(PAG_256, 1024, 1024);
  // delay(10);
}

void loop(void) {
  M5.update();
  if (M5.BtnA.wasPressed()) {
    ammeter.setMode(SINGLESHOT);
    ammeter.setRate(RATE_8);
    ammeter.setGain(PAG_512);
    now_gain = PAG_512;
    hope = page512_volt / ammeter.resolution;

    for (uint8_t i = 0; i < 10; i++) {
      volt_raw_list[i] = 0;
    }
  }

  if (M5.BtnC.wasPressed()) {
    bool success = ammeter.saveCalibration2EEPROM(now_gain, hope, adc_raw);
    M5.Lcd.setCursor(230, 210);
    
    if (success) {
      M5.Lcd.setTextColor(GREEN, BLACK);
    } else {
      M5.Lcd.setTextColor(RED, BLACK);
    }

    M5.Lcd.printf("SAVE");

    delay(300);
    M5.Lcd.setCursor(230, 210);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.printf("SAVE");

    ammeter.setGain(now_gain);
  }

  float current = ammeter.getCurrent();
  
  volt_raw_list[raw_now_ptr] = ammeter.adc_raw;
  raw_now_ptr = (raw_now_ptr == 9) ? 0 : (raw_now_ptr + 1);

  int count = 0;
  int total = 0;

  for (uint8_t i = 0; i < 10; i++) {
    if (volt_raw_list[i] == 0) {
      continue ;
    }
    total += volt_raw_list[i];
    count += 1;
  }

  if (count == 0) {
    adc_raw = 0;
  } else {
    adc_raw = total / count;
  }

  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(10, 10);
  M5.Lcd.printf("Hope volt: %.2f mA             \r\n", page512_volt);

  M5.Lcd.setCursor(10, 40);
  M5.Lcd.printf("Hope ADC: %d       \r\n", hope);

  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(10, 80);
  M5.Lcd.printf("Cal volt: %.2f mA             \r\n", current);

  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(10, 110);
  M5.Lcd.printf("Cal ADC: %.0f         \r\n", adc_raw * ammeter.calibration_factor);

  M5.Lcd.setCursor(10, 150);

  if (abs(adc_raw) <= hope * 1.005 && abs(adc_raw) >= hope * 0.995) {
    M5.Lcd.setTextColor(GREEN, BLACK);
  } else {
    M5.Lcd.setTextColor(RED, BLACK);
  }

  M5.Lcd.printf("RAW ADC: %d        \r\n", adc_raw);
}

