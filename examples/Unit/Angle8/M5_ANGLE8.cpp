#include "M5_ANGLE8.h"

/*! @brief Initialize the ANGLE8.
    @return True if the init was successful, otherwise false.. */
bool M5_ANGLE8::begin(uint8_t addr) {
    _wire = &Wire;
    _addr = addr;
    _wire->begin();
    delay(10);
    _wire->beginTransmission(_addr);
    uint8_t error = _wire->endTransmission();
    if (error == 0) {
        return true;
    } else {
        return false;
    }
}

/*! @brief Write a certain length of data to the specified register address.
    @return True if the write was successful, otherwise false.. */
bool M5_ANGLE8::writeBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
                           uint8_t length) {
    _wire->beginTransmission(addr);
    _wire->write(reg);
    for (uint8_t i = 0; i < length; i++) {
        _wire->write(*(buffer + i));
    }
    _wire->endTransmission();
    return true;
    //    return false;
}

/*! @brief Read a certain length of data to the specified register address.
    @return True if the read was successful, otherwise false.. */
bool M5_ANGLE8::readBytes(uint8_t addr, uint8_t reg, uint8_t *buffer,
                          uint8_t length) {
    uint8_t index = 0;
    _wire->beginTransmission(addr);
    _wire->write(reg);
    _wire->endTransmission();
    if (_wire->requestFrom(addr, length)) {
        for (uint8_t i = 0; i < length; i++) {
            buffer[index++] = _wire->read();
        }
        return true;
    }
    return false;
}

/*! @brief Set the addr of device.
    @return True if the set was successful, otherwise false.. */
bool M5_ANGLE8::setDeviceAddr(uint8_t addr) {
    if (writeBytes(_addr, ANGLE8_ADDRESS_REG, &addr, 1)) {
        _addr = addr;
        return true;
    } else {
        return false;
    }
}

/*! @brief Get the Version of Firmware.
    @return Firmware version */
uint8_t M5_ANGLE8::getVersion() {
    uint8_t data = 0;
    readBytes(_addr, ANGLE8_FW_VERSION_REG, &data, 1);
    return data;
}

/*! @brief Set the color of led lights.
    @return True if the set was successful, otherwise false.. */
bool M5_ANGLE8::setLEDColor(uint8_t ch, uint32_t color, uint8_t bright) {
    if (ch > ANGLE8_TOTAL_LED) return false;
    uint8_t data[4] = {0};
    data[0]         = (color >> 16) & 0xff;
    data[1]         = (color >> 8) & 0xff;
    data[2]         = color & 0xff;
    data[3]         = bright & 0xff;
    uint8_t reg     = ch * 4 + ANGLE8_RGB_24B_REG;
    return writeBytes(_addr, reg, data, 4);
}

/*! @brief Get digital singal input.
    @return True if the read was successful, otherwise false.. */
bool M5_ANGLE8::getDigitalInput() {
    uint8_t data;
    uint8_t reg = ANGLE8_DIGITAL_INPUT_REG;
    if (readBytes(_addr, reg, &data, 1)) {
        return data;
    }
    return 0;
}

/*! @brief Get analog singal input.
    @return True if the read was successful, otherwise false.. */
uint16_t M5_ANGLE8::getAnalogInput(uint8_t ch, angle8_analog_read_mode_t bit) {
    if (bit == _8bit) {
        uint8_t data;
        uint8_t reg = ch + ANGLE8_ANALOG_INPUT_8B_REG;
        if (readBytes(_addr, reg, &data, 1)) {
            return data;
        }
    } else {
        uint8_t data[2];
        uint8_t reg = ch * 2 + ANGLE8_ANALOG_INPUT_12B_REG;
        if (readBytes(_addr, reg, data, 2)) {
            return (data[1] << 8) | data[0];
        }
    }
    return 0;
}
