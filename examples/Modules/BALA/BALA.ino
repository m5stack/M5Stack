/*
    Description: This code only for mpu6886! first init need press BtnC and reboot to calibrate !
*/

#include <M5Stack.h>

#include <Wire.h>
#include <Preferences.h>

#include "imuCalibration.h"
#include "M5Bala.h"

Preferences preferences;

M5Bala m5bala(Wire);

// ================ Draw Angle Wavefrom =================
void draw_waveform() {
	#define MAX_LEN 120
	#define X_OFFSET 0
	#define Y_OFFSET 100
	#define X_SCALE 3
	static int16_t val_buf[MAX_LEN] = {0};
	static int16_t pt = MAX_LEN - 1;

	val_buf[pt] = constrain((int16_t)(m5bala.getAngle() * X_SCALE), -80, 80);
	if (--pt < 0) {
		pt = MAX_LEN - 1;
	}

	for (int i = 1; i < (MAX_LEN); i++) {
		uint16_t now_pt = (pt + i) % (MAX_LEN);
		M5.Lcd.drawLine(i, val_buf[(now_pt + 1) % MAX_LEN] + Y_OFFSET, i + 1,
							val_buf[(now_pt + 2) % MAX_LEN] + Y_OFFSET, TFT_BLACK);
		if (i < MAX_LEN - 1)
			M5.Lcd.drawLine(i, val_buf[now_pt] + Y_OFFSET, i + 1,
							val_buf[(now_pt + 1) % MAX_LEN] + Y_OFFSET, TFT_GREEN);
	}
}

// ================ GYRO offset param ==================
void auto_tune_gyro_offset() {
	M5.Speaker.tone(500, 200);
	delay(300);
	M5.update();
	M5.Lcd.println("Start IMU calculate gyro offsets");
	M5.Lcd.println("DO NOT MOVE A MPU6050...");
	delay(2000);

	imu_calcGyroOffsets();
	
	float gyroXoffset = imu_getOffsetX();

	M5.Lcd.println("Done!!!");
	M5.Lcd.print("X : ");M5.Lcd.println(gyroXoffset);
	M5.Lcd.println("Program will start after 3 seconds");
	M5.Lcd.print("========================================");

	// Save
	preferences.putFloat("gyroXoffset", gyroXoffset);
	preferences.end();
}


void setup() {
	// Power ON Stabilizing...
	M5.begin();
	M5.Power.begin();
	Wire.begin();
	Wire.setClock(400000UL);  // Set I2C frequency to 400kHz

	// Display info
	M5.Lcd.setTextFont(2);
	M5.Lcd.setTextColor(WHITE, BLACK);
	M5.Lcd.println("M5Stack Balance Mode start");

	// Init M5Bala
	m5bala.begin();
	m5bala.setAngleOffset(-2.2);


	// Loading the IMU parameters
	if (M5.BtnC.isPressed()) {
		preferences.begin("m5bala-cfg", false);
		auto_tune_gyro_offset();
	} else {
		preferences.begin("m5bala-cfg", true);
		imu_setOffsetX(preferences.getFloat("gyroXoffset"));
	}
}

void loop() {

	// LCD display
	static uint32_t print_interval = millis() + 30;
	if (millis() > print_interval) {
		print_interval = millis() + 100;
		M5.Lcd.setCursor(0, 190);
		M5.Lcd.printf("Input  Encoer0: %+4d  Encoer1: %+4d    \r\n", 
								m5bala.getSpeed0(), m5bala.getSpeed1());
		M5.Lcd.printf("Output PWM0: %+4d     PWM1: %+4d    \r\n", 
								m5bala.getOut0(), m5bala.getOut1());
		M5.Lcd.printf("AngleX: %+05.2f\r\n", m5bala.getAngle());
	}

	// Draw the waveform
	static uint32_t draw_interval = millis() + 5;
	if (millis() > draw_interval) {
		draw_interval = millis() + 20;
		draw_waveform();
	}

	// M5Bala balance run
	m5bala.run();

	// M5 Loop
	M5.update();
}