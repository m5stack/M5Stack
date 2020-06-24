#include <Wire.h>

#include "ClosedCube_TCA9548A.h"

ClosedCube::Wired::TCA9548A::TCA9548A() {
}

ClosedCube::Wired::TCA9548A::TCA9548A(uint8_t address):_address(address) {    
}

void ClosedCube::Wired::TCA9548A::address(uint8_t address) {    
	_address = address;
}

uint8_t ClosedCube::Wired::TCA9548A::getChannel() {
	return _currentChannel;
}

uint8_t ClosedCube::Wired::TCA9548A::selectChannel(uint8_t channel) {
	uint8_t result = 0xff;
	if (channel >= 0 && channel < TCA9548A_MAX_CHANNELS) {
		Wire.beginTransmission(_address);
		Wire.write( ((uint8_t)1) << (channel));
		
		_currentChannel = channel;
		result = Wire.endTransmission();
	} 
	return result;
}

uint8_t ClosedCube::Wired::TCA9548A::nextChannel() {
	uint8_t nextChannel = _currentChannel + 1;
	if (nextChannel > (TCA9548A_MAX_CHANNELS-1)) {
		nextChannel = 0;	
	}

	return selectChannel(nextChannel);
}
 
