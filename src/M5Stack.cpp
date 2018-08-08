// Copyright (c) M5Stack. All rights reserved.
// Licensed under the MIT license. See LICENSE file in the project root for full license information.

#include "M5Stack.h"

M5Stack::M5Stack() : isInited(0) {

}

void M5Stack::begin(bool LCDEnable, bool SDEnable, bool SerialEnable) {

	// Correct init once
	if (isInited) return;
	else isInited = true;

	// UART
	if (SerialEnable) {
		Serial.begin(115200);
		Serial.flush();
		delay(50);
		Serial.print("M5Stack initializing...");
	}

	// LCD INIT
	if (LCDEnable) {
		Lcd.begin();
	}

	// TF Card
	if (SDEnable) {
		SD.begin(TFCARD_CS_PIN, SPI, 40000000);
	}

	// TONE
	Speaker.begin();

	// Set wakeup button
	setWakeupButton(BUTTON_A_PIN);

	// I2C init
	#ifdef M5STACK_FIRE
	Wire.begin(21, 22);
	#endif

	if (SerialEnable) {
		Serial.println("OK");
	}
}

void M5Stack::update() {

	//Button update
	BtnA.read();
	BtnB.read();
	BtnC.read();

	//Speaker update
	Speaker.update();
}

#ifdef M5STACK_FIRE
// ================ Power IC IP5306 ===================
#define IP5306_ADDR           117
#define IP5306_REG_SYS_CTL0   0x00
#define IP5306_REG_READ1      0x71
#define CHARGE_FULL_BIT       3

void M5Stack::setPowerBoostKeepOn(bool en)
{
	Wire.beginTransmission(IP5306_ADDR);
	Wire.write(IP5306_REG_SYS_CTL0);
	if (en) Wire.write(0x37); // Set bit1: 1 enable 0 disable boost keep on
	else Wire.write(0x35);    // 0x37 is default reg value
	Wire.endTransmission();
}

uint8_t M5Stack::isChargeFull()
{
	uint8_t data;
	Wire.beginTransmission(IP5306_ADDR);
	Wire.write(IP5306_REG_READ1);
	Wire.endTransmission(false);
	Wire.requestFrom(IP5306_ADDR, 1);
	data = Wire.read();
	if (data & (1 << CHARGE_FULL_BIT)) return true;
	else return false;
}
#endif

// ================== Low power mode =====================
void M5Stack::setWakeupButton(uint8_t button) {
	_wakeupPin = button;
}

void M5Stack::powerOFF() {

	#ifdef M5STACK_FIRE
	// Keep power keep boost on
	setPowerBoostKeepOn(true);
	#endif

	// power off the Lcd
	Lcd.setBrightness(0);
	Lcd.sleep();

	// ESP32 into deep sleep
	esp_sleep_enable_ext0_wakeup((gpio_num_t)_wakeupPin , LOW);

	while(digitalRead(_wakeupPin) == LOW) {
		delay(10);
	}
	esp_deep_sleep_start();
}


M5Stack M5;
