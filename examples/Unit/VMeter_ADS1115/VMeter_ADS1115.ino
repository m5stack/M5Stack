/*
    Description: Measure voltage and display
    EEPROM (0x53) has built-in calibration parameters when leaving the factory. 
    Please do not write to the EEPROM, otherwise the calibration data will be overwritten and the measurement results will be inaccurate
*/

#include <Wire.h>
#include "voltmeter.h"
#include "M5Stack.h"

Voltmeter voltmeter;

float page512_volt = 5000.0F;
float page4096_volt = 60000.0F;

int16_t volt_raw_list[10];
uint8_t raw_now_ptr = 0;
int16_t adc_raw = 0;

int16_t hope = 0.0;

voltmeterGain_t now_gain = PAG_512;

void setup(void) {
  M5.begin();
  Wire.begin();

  voltmeter.setMode(SINGLESHOT);
  voltmeter.setRate(RATE_8);
  voltmeter.setGain(PAG_512);
  hope = page512_volt / voltmeter.resolution;
  // | PAG      | Max Input Voltage(V) |
  // | PAG_6144 |        128           |
  // | PAG_4096 |        64            |
  // | PAG_2048 |        32            |
  // | PAG_512  |        16            |
  // | PAG_256  |        8             |

  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextFont(4);

  M5.Lcd.setCursor(52, 210);
  M5.Lcd.printf("5V");

  M5.Lcd.setCursor(138, 210);
  M5.Lcd.printf("60V");

  M5.Lcd.setCursor(230, 210);
  M5.Lcd.printf("SAVE");

  // bool result1 = voltmeter.saveCalibration2EEPROM(PAG_256, 1024, 1024);
  // delay(10);
}

void loop(void) {
  M5.update();
  if (M5.BtnA.wasPressed()) {
    voltmeter.setMode(SINGLESHOT);
    voltmeter.setRate(RATE_8);
    voltmeter.setGain(PAG_512);
    now_gain = PAG_512;
    hope = page512_volt / voltmeter.resolution;

    for (uint8_t i = 0; i < 10; i++) {
      volt_raw_list[i] = 0;
    }
  }

  if (M5.BtnB.wasPressed()) {
    voltmeter.setMode(SINGLESHOT);
    voltmeter.setRate(RATE_8);
    voltmeter.setGain(PAG_4096);
    now_gain = PAG_4096;
    hope = page4096_volt / voltmeter.resolution;

    for (uint8_t i = 0; i < 10; i++) {
      volt_raw_list[i] = 0;
    }
  }

//  if (M5.BtnC.wasPressed()) {
//    bool success = voltmeter.saveCalibration2EEPROM(now_gain, hope, adc_raw);
//    M5.Lcd.setCursor(230, 210);
//    
//    if (success) {
//      M5.Lcd.setTextColor(GREEN, BLACK);
//    } else {
//      M5.Lcd.setTextColor(RED, BLACK);
//    }
//
//    M5.Lcd.printf("SAVE");
//
//    delay(300);
//    M5.Lcd.setCursor(230, 210);
//    M5.Lcd.setTextColor(WHITE, BLACK);
//    M5.Lcd.printf("SAVE");
//
//    voltmeter.setGain(now_gain);
//  }

  voltmeter.getVoltage();
  
  volt_raw_list[raw_now_ptr] = voltmeter.adc_raw;
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
  if (now_gain == PAG_512) {
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.printf("Hope volt: %.2f mv             \r\n", page512_volt);
  } else {
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.printf("Hope volt: %.2f mv              \r\n", page4096_volt);
  }

  M5.Lcd.setCursor(10, 40);
  M5.Lcd.printf("Hope ADC: %d     \r\n", hope);

  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(10, 80);
  M5.Lcd.printf("Cal volt: %.2f mv           \r\n", adc_raw * voltmeter.resolution * voltmeter.calibration_factor);

  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(10, 110);
  M5.Lcd.printf("Cal ADC: %.0f      \r\n", adc_raw * voltmeter.calibration_factor);

  M5.Lcd.setCursor(10, 150);

  if (adc_raw <= hope * 1.001 && adc_raw >= hope * 0.999) {
    M5.Lcd.setTextColor(GREEN, BLACK);
  } else {
    M5.Lcd.setTextColor(RED, BLACK);
  }

  M5.Lcd.printf("RAW ADC: %d        \r\n", adc_raw);

  // M5.Lcd.setCursor(10, 110);
  // M5.Lcd.printf("Actual RAW: %d     \r\n", adc_raw);
  // // M5.Lcd.printf("Actual RAW: %d     \r\n", voltmeter.adc_raw);
  // M5.Lcd.setCursor(10, 140);
  // M5.Lcd.printf("Hope RAW: %d     \r\n", hope);
}
