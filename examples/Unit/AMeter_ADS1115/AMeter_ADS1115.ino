/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN
*
* describe: Ameter_ADS1115.  电流计
* date：2021/8/19
*******************************************************************************
  Please connect to Port A,Measure current and display in the screen.
  请连接端口A,测量电流并显示到屏幕上
  Pay attention: EEPROM (0x51) has built-in calibration parameters when leaving the factory.
  Please do not write to the EEPROM, otherwise the calibration data will be overwritten and the measurement results will be inaccurate.
  注意: EEPROM (0x51)在出厂时具有内置的校准参数。请不要写入EEPROM，否则校准数据会被覆盖，测量结果会不准确。
*/

#include "M5Stack.h"
#include "M5_ADS1115.h"

Ammeter ammeter;

float page512_volt = 2000.0F;

int16_t volt_raw_list[10];
uint8_t raw_now_ptr = 0;
int16_t adc_raw = 0;

int16_t hope = 0.0;

ammeterGain_t now_gain = PAG_512;

void setup(void) {
  M5.begin(); //Init M5Stack.  初始化M5Stack
  M5.Power.begin(); //Init power  初始化电源模块

  Wire.begin();

  ammeter.setMode(SINGLESHOT);                /* | PAG      | Max Input Voltage(V) | */
  ammeter.setRate(RATE_8);                    /* | PAG_6144 |        128           | */
  ammeter.setGain(PAG_512);                   /* | PAG_4096 |        64            | */
  hope = page512_volt / ammeter.resolution;   /* | PAG_2048 |        32            | */
                                              /* | PAG_512  |        16            | */
                                              /* | PAG_256  |        8             | */
  M5.Lcd.setTextFont(4);  //Set font to 4 point font.  设置字体为4号字体
  M5.Lcd.setCursor(52, 210);  //Set the cursor at (52,210).  将光标设置在(52, 210)
  M5.Lcd.printf("2A                            SAVE");
}

void loop(void) {
  M5.update();  //Check the status of the key.  检测按键的状态
  if (M5.BtnA.wasPressed()) {
    ammeter.setMode(SINGLESHOT);  //Set the mode.  设置模式
    ammeter.setRate(RATE_8);  //Set the rate.  设置速率
    ammeter.setGain(PAG_512);
    now_gain = PAG_512;
    hope = page512_volt / ammeter.resolution;

    for (uint8_t i = 0; i < 10; i++) {
      volt_raw_list[i] = 0;
    }
  }

  if (M5.BtnC.wasPressed()) {
    bool success = ammeter.saveCalibration2EEPROM(now_gain, hope, adc_raw);
    M5.Lcd.setCursor(224, 210);
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