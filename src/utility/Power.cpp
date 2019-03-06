/*----------------------------------------------------------------------*
 * M5Stack Bettery/Power Control Library v1.0                           *
 *                                                                      *
 * This work is licensed under the GNU Lesser General Public            *
 * License v2.1                                                         *
 * https://www.gnu.org/licenses/old-licenses/lgpl-2.1.en.html           *
 *----------------------------------------------------------------------*/
#include "Power.h"
#include "../M5Stack.h"

// ================ Power IC IP5306 ===================
#define IP5306_ADDR           (117)
#define IP5306_REG_SYS_CTL0   (0x00)
#define IP5306_REG_READ0      (0x70)
#define IP5306_REG_READ1      (0x71)
#define CHARGE_FULL_BIT       (   3)
#define BOOST_OUT_BIT         (0x02)
#define LIGHT_LOAD_BIT 		  (0x20)
#define CHARGE_OUT_BIT        (0x10)
#define BOOST_ENABLE_BIT      (0x80)

extern M5Stack M5;

POWER::POWER()
{
}

/*
	default: false
	false: when the current is too small, ip5306 will automatically shut down
*/
bool POWER::setKeepLightLoad(bool en) {
	uint8_t data;
	Wire.beginTransmission(IP5306_ADDR);
	Wire.write(IP5306_REG_SYS_CTL0);
	Wire.endTransmission();

	if(Wire.requestFrom(IP5306_ADDR, 1))
	{
		data = Wire.read();

		Wire.beginTransmission(IP5306_ADDR);
		Wire.write(IP5306_REG_SYS_CTL0);
		if (!en) Wire.write(data |  LIGHT_LOAD_BIT); 
		else Wire.write(data &(~LIGHT_LOAD_BIT));  
		Wire.endTransmission();
		return true;
	}
	return false;
}

#ifdef M5STACK_FIRE
// true: Output normally open
bool POWER::setPowerBoostKeepOn(bool en){
	uint8_t data;
	Wire.beginTransmission(IP5306_ADDR);
	Wire.write(IP5306_REG_SYS_CTL0);
	Wire.endTransmission();

	if(Wire.requestFrom(IP5306_ADDR, 1))
	{
		data = Wire.read();

		Wire.beginTransmission(IP5306_ADDR);
		Wire.write(IP5306_REG_SYS_CTL0);
		if (en) Wire.write(data |  BOOST_OUT_BIT); 
		else Wire.write(data &(~BOOST_OUT_BIT));  
		Wire.endTransmission();
		return true;
	}
	return false;
}
#endif

// if charge full,try set charge enable->disable->enable,can be recharged
bool POWER::setCharge(bool en){
	uint8_t data;
	Wire.beginTransmission(IP5306_ADDR);
	Wire.write(IP5306_REG_SYS_CTL0);
	Wire.endTransmission();

	if(Wire.requestFrom(IP5306_ADDR, 1))
	{
		data = Wire.read();

		Wire.beginTransmission(IP5306_ADDR);
		Wire.write(IP5306_REG_SYS_CTL0);
		if (en) Wire.write(data |  CHARGE_OUT_BIT);
		else Wire.write(data &(~CHARGE_OUT_BIT));
		Wire.endTransmission();
		return true;
	}
	return false;
}

// full return true, else return false
bool POWER::isChargeFull(){
	uint8_t data;
	Wire.beginTransmission(IP5306_ADDR);
	Wire.write(IP5306_REG_READ1);
	Wire.endTransmission(false);
	if(Wire.requestFrom(IP5306_ADDR, 1))
	{
		data = Wire.read();
		if (data & (1 << CHARGE_FULL_BIT)) return true;
		else return false;
	}
	return false;
}

// test if ip5306 is an i2c version
bool POWER::canControl(){
	uint8_t data;
	Wire.beginTransmission(IP5306_ADDR);
	Wire.write(IP5306_REG_READ0);
	return(Wire.endTransmission()==0);
}

//true:charge, false:discharge
bool POWER::isCharging(){
	uint8_t data;
	Wire.beginTransmission(IP5306_ADDR);
	Wire.write(IP5306_REG_READ0);
	Wire.endTransmission(false);
	if(Wire.requestFrom(IP5306_ADDR, 1))
	{
		data = Wire.read();
		if (data & (1 << CHARGE_FULL_BIT)) return true;
		else return false;
	}
	return false;
}

void POWER::setWakeupButton(uint8_t button) {
	_wakeupPin = button;
}

void POWER::reset() {
	esp_restart();
}

/* caution: if use off()function on running battery ,
   M5stack can't restart from Power switch.
   (Switch pull up power has down by hard power down)*/
//bool POWER::batteryMode(bool en){
//
//	uint8_t data;
//	Wire.beginTransmission(IP5306_ADDR);
//	Wire.write(IP5306_REG_READ1);
//	Wire.endTransmission();
//
//	if(Wire.requestFrom(IP5306_ADDR, 1))
//	{
//		data = Wire.read();
//
//		Wire.beginTransmission(IP5306_ADDR);
//		Wire.write(IP5306_REG_READ1);
//		if (en) Wire.write(data |  BOOST_ENABLE_BIT);
//		else Wire.write(data &(~BOOST_ENABLE_BIT));
//		Wire.endTransmission();
//		return true;
//	}
//	return false;
//}

void POWER::deepSleep(){

	#ifdef M5STACK_FIRE
	// Keep power keep boost on
	setPowerBoostKeepOn(true);
	#endif

	// power off the Lcd
	M5.Lcd.setBrightness(0);
	M5.Lcd.sleep();

	// ESP32 into deep sleep
	esp_sleep_enable_ext0_wakeup((gpio_num_t)_wakeupPin , LOW);

	while(digitalRead(_wakeupPin) == LOW) {
		delay(10);
	}
	esp_deep_sleep_start();
}

