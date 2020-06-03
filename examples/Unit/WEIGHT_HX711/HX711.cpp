/**
 *
 * HX711 library for Arduino
 * https://github.com/bogde/HX711
 *
 * MIT License
 * (c) 2018 Bogdan Necula
 *
**/
#include <Arduino.h>
#include "HX711.h"

// TEENSYDUINO has a port of Dean Camera's ATOMIC_BLOCK macros for AVR to ARM Cortex M3.
#define HAS_ATOMIC_BLOCK (defined(ARDUINO_ARCH_AVR) || defined(TEENSYDUINO))

// Whether we are running on either the ESP8266 or the ESP32.
#define ARCH_ESPRESSIF (defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_ESP32))

// Whether we are actually running on FreeRTOS.
#define IS_FREE_RTOS defined(ARDUINO_ARCH_ESP32)

// Define macro designating whether we're running on a reasonable
// fast CPU and so should slow down sampling from GPIO.
#define FAST_CPU \
    ( \
    ARCH_ESPRESSIF || \
    defined(ARDUINO_ARCH_SAM)     || defined(ARDUINO_ARCH_SAMD) || \
    defined(ARDUINO_ARCH_STM32)   || defined(TEENSYDUINO) \
    )

#if HAS_ATOMIC_BLOCK
// Acquire AVR-specific ATOMIC_BLOCK(ATOMIC_RESTORESTATE) macro.
#include <util/atomic.h>
#endif

#if FAST_CPU
// Make shiftIn() be aware of clockspeed for
// faster CPUs like ESP32, Teensy 3.x and friends.
// See also:
// - https://github.com/bogde/HX711/issues/75
// - https://github.com/arduino/Arduino/issues/6561
// - https://community.hiveeyes.org/t/using-bogdans-canonical-hx711-library-on-the-esp32/539
/*
uint8_t shiftInSlow(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
    uint8_t value = 0;
    uint8_t i;

    for(i = 0; i < 8; ++i) {
        digitalWrite(clockPin, HIGH);
        delayMicroseconds(1);
        
        digitalWrite(clockPin, LOW);
        delayMicroseconds(1);
        if(bitOrder == LSBFIRST)
            value |= digitalRead(dataPin) << i;
        else
            value |= digitalRead(dataPin) << (7 - i);
    }
    return value;
}
*/
uint8_t shiftInSlow(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
    uint8_t value = 0;
    uint8_t i;

    for(i = 0; i < 8; ++i) {
        digitalWrite(clockPin, HIGH);
        delayMicroseconds(1);
        if(bitOrder == LSBFIRST)
            value |= digitalRead(dataPin) << i;
        else
            value |= digitalRead(dataPin) << (7 - i);
        digitalWrite(clockPin, LOW);
        delayMicroseconds(1);
    }
    return value;
}


#define SHIFTIN_WITH_SPEED_SUPPORT(data,clock,order) shiftInSlow(data,clock,order)
#else
#define SHIFTIN_WITH_SPEED_SUPPORT(data,clock,order) shiftIn(data,clock,order)
#endif


HX711::HX711() {
}

HX711::~HX711() {
}

void HX711::begin(byte dout, byte pd_sck, byte gain) {
	PD_SCK = pd_sck;
	DOUT = dout;

	pinMode(PD_SCK, OUTPUT);
	pinMode(DOUT, INPUT_PULLUP);

	set_gain(gain);
}

bool HX711::is_ready() {
	return digitalRead(DOUT) == LOW;
}

void HX711::set_gain(byte gain) {
	switch (gain) {
		case 128:		// channel A, gain factor 128
			GAIN = 1;
			break;
		case 64:		// channel A, gain factor 64
			GAIN = 3;
			break;
		case 32:		// channel B, gain factor 32
			GAIN = 2;
			break;
	}

	//digitalWrite(PD_SCK, LOW);
	if(is_ready())
	    read();
	
	
}

long HX711::read() {

	// Wait for the chip to become ready.
	wait_ready();

	// Define structures for reading data into.
	unsigned long value = 0;
	uint8_t data[3] = { 0 };
	uint8_t filler = 0x00;

	// Protect the read sequence from system interrupts.  If an interrupt occurs during
	// the time the PD_SCK signal is high it will stretch the length of the clock pulse.
	// If the total pulse time exceeds 60 uSec this will cause the HX711 to enter
	// power down mode during the middle of the read sequence.  While the device will
	// wake up when PD_SCK goes low again, the reset starts a new conversion cycle which
	// forces DOUT high until that cycle is completed.
	//
	// The result is that all subsequent bits read by shiftIn() will read back as 1,
	// corrupting the value returned by read().  The ATOMIC_BLOCK macro disables
	// interrupts during the sequence and then restores the interrupt mask to its previous
	// state after the sequence completes, insuring that the entire read-and-gain-set
	// sequence is not interrupted.  The macro has a few minor advantages over bracketing
	// the sequence between `noInterrupts()` and `interrupts()` calls.
	#if HAS_ATOMIC_BLOCK
	ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {

	#elif IS_FREE_RTOS
	// Begin of critical section.
	// Critical sections are used as a valid protection method
	// against simultaneous access in vanilla FreeRTOS.
	// Disable the scheduler and call portDISABLE_INTERRUPTS. This prevents
	// context switches and servicing of ISRs during a critical section.
	portMUX_TYPE mux = portMUX_INITIALIZER_UNLOCKED;
	portENTER_CRITICAL(&mux);

	#else
	// Disable interrupts.
	noInterrupts();
	#endif

	// Pulse the clock pin 24 times to read the data.
    data[2] = shiftIn(DOUT, PD_SCK, MSBFIRST);
    data[1] = shiftIn(DOUT, PD_SCK, MSBFIRST);
    data[0] = shiftIn(DOUT, PD_SCK, MSBFIRST);
    /*
	data[2] = SHIFTIN_WITH_SPEED_SUPPORT(DOUT, PD_SCK, MSBFIRST);
	data[1] = SHIFTIN_WITH_SPEED_SUPPORT(DOUT, PD_SCK, MSBFIRST);
	data[0] = SHIFTIN_WITH_SPEED_SUPPORT(DOUT, PD_SCK, MSBFIRST);
    */
	// Set the channel and the gain factor for the next reading using the clock pin.
	for (unsigned int i = 0; i < GAIN; i++) {
		digitalWrite(PD_SCK, HIGH);
		#if ARCH_ESPRESSIF
		delayMicroseconds(1);
		#endif
		digitalWrite(PD_SCK, LOW);
		#if ARCH_ESPRESSIF
		delayMicroseconds(1);
		#endif
	}

	#if IS_FREE_RTOS
	// End of critical section.
	portEXIT_CRITICAL(&mux);

	#elif HAS_ATOMIC_BLOCK
	}

	#else
	// Enable interrupts again.
	interrupts();
	#endif

	// Replicate the most significant bit to pad out a 32-bit signed integer
	if (data[2] & 0x80) {
		filler = 0xFF;
	} else {
		filler = 0x00;
	}

	// Construct a 32-bit signed integer
	value = ( static_cast<unsigned long>(filler) << 24
			| static_cast<unsigned long>(data[2]) << 16
			| static_cast<unsigned long>(data[1]) << 8
			| static_cast<unsigned long>(data[0]) );

	return static_cast<long>(value);
}

void HX711::wait_ready(unsigned long delay_ms) {
	// Wait for the chip to become ready.
	// This is a blocking implementation and will
	// halt the sketch until a load cell is connected.
	while (!is_ready()) {
		// Probably will do no harm on AVR but will feed the Watchdog Timer (WDT) on ESP.
		// https://github.com/bogde/HX711/issues/73
		delay(delay_ms);
	}
}

bool HX711::wait_ready_retry(int retries, unsigned long delay_ms) {
	// Wait for the chip to become ready by
	// retrying for a specified amount of attempts.
	// https://github.com/bogde/HX711/issues/76
	int count = 0;
	while (count < retries) {
		if (is_ready()) {
			return true;
		}
		delay(delay_ms);
		count++;
	}
	return false;
}

bool HX711::wait_ready_timeout(unsigned long timeout, unsigned long delay_ms) {
	// Wait for the chip to become ready until timeout.
	// https://github.com/bogde/HX711/pull/96
	unsigned long millisStarted = millis();
	while (millis() - millisStarted < timeout) {
		if (is_ready()) {
			return true;
		}
		delay(delay_ms);
	}
	return false;
}

long HX711::read_average(byte times) {
	long sum = 0;
	for (byte i = 0; i < times; i++) {
		sum += read();
		// Probably will do no harm on AVR but will feed the Watchdog Timer (WDT) on ESP.
		// https://github.com/bogde/HX711/issues/73
		delay(0);
	}
	return sum / times;
}

double HX711::get_value(byte times) {
	return read_average(times) - OFFSET;
}

float HX711::get_units(byte times) {
	return get_value(times) / SCALE;
}

void HX711::tare(byte times) {
	double sum = read_average(times);
	set_offset(sum);
}

void HX711::set_scale(float scale) {
	SCALE = scale;
}

float HX711::get_scale() {
	return SCALE;
}

void HX711::set_offset(long offset) {
	OFFSET = offset;
}

long HX711::get_offset() {
	return OFFSET;
}

void HX711::power_down() {
	digitalWrite(PD_SCK, LOW);
	digitalWrite(PD_SCK, HIGH);
}

void HX711::power_up() {
	digitalWrite(PD_SCK, LOW);
}