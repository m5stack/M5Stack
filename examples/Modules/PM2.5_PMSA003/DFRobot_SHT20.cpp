#include "DFRobot_SHT20.h"

void DFRobot_SHT20::initSHT20(TwoWire &wirePort) {
    i2cPort = &wirePort;
    i2cPort->begin();
}

uint16_t DFRobot_SHT20::readValue(byte cmd) {
    i2cPort->beginTransmission(SLAVE_ADDRESS);
    i2cPort->write(cmd);
    i2cPort->endTransmission();
    byte toRead;
    byte counter;
    for (counter = 0, toRead = 0; counter < MAX_COUNTER && toRead != 3;
         counter++) {
        delay(DELAY_INTERVAL);
        toRead = i2cPort->requestFrom(SLAVE_ADDRESS, 3);
    }
    if (counter == MAX_COUNTER) {
        return (ERROR_I2C_TIMEOUT);
    }
    byte msb, lsb, checksum;
    msb               = i2cPort->read();
    lsb               = i2cPort->read();
    checksum          = i2cPort->read();
    uint16_t rawValue = ((uint16_t)msb << 8) | (uint16_t)lsb;
    if (checkCRC(rawValue, checksum) != 0) {
        return (ERROR_BAD_CRC);
    }
    return rawValue & 0xFFFC;
}

float DFRobot_SHT20::readHumidity(void) {
    uint16_t rawHumidity = readValue(TRIGGER_HUMD_MEASURE_NOHOLD);
    if (rawHumidity == ERROR_I2C_TIMEOUT || rawHumidity == ERROR_BAD_CRC) {
        return (rawHumidity);
    }
    float tempRH = rawHumidity * (125.0 / 65536.0);
    float rh     = tempRH - 6.0;
    return (rh);
}

float DFRobot_SHT20::readTemperature(void) {
    uint16_t rawTemperature = readValue(TRIGGER_TEMP_MEASURE_NOHOLD);
    if (rawTemperature == ERROR_I2C_TIMEOUT ||
        rawTemperature == ERROR_BAD_CRC) {
        return (rawTemperature);
    }
    float tempTemperature = rawTemperature * (175.72 / 65536.0);
    float realTemperature = tempTemperature - 46.85;
    return (realTemperature);
}

void DFRobot_SHT20::setResolution(byte resolution) {
    byte userRegister = readUserRegister();
    userRegister &= B01111110;
    resolution &= B10000001;
    userRegister |= resolution;
    writeUserRegister(userRegister);
}

byte DFRobot_SHT20::readUserRegister(void) {
    byte userRegister;
    i2cPort->beginTransmission(SLAVE_ADDRESS);
    i2cPort->write(READ_USER_REG);
    i2cPort->endTransmission();
    i2cPort->requestFrom(SLAVE_ADDRESS, 1);
    userRegister = i2cPort->read();
    return (userRegister);
}

void DFRobot_SHT20::writeUserRegister(byte val) {
    i2cPort->beginTransmission(SLAVE_ADDRESS);
    i2cPort->write(WRITE_USER_REG);
    i2cPort->write(val);
    i2cPort->endTransmission();
}

byte DFRobot_SHT20::checkCRC(uint16_t message_from_sensor,
                             uint8_t check_value_from_sensor) {
    uint32_t remainder = (uint32_t)message_from_sensor << 8;
    remainder |= check_value_from_sensor;
    uint32_t divsor = (uint32_t)SHIFTED_DIVISOR;
    for (int i = 0; i < 16; i++) {
        if (remainder & (uint32_t)1 << (23 - i)) {
            remainder ^= divsor;
        }
        divsor >>= 1;
    }
    return (byte)remainder;
}

void DFRobot_SHT20::showReslut(const char *prefix, int val) {
    Serial.print(prefix);
    if (val) {
        Serial.println("yes");
    } else {
        Serial.println("no");
    }
}

void DFRobot_SHT20::checkSHT20(void) {
    byte reg = readUserRegister();
    showReslut("End of battery: ", reg & USER_REGISTER_END_OF_BATTERY);
    showReslut("Heater enabled: ", reg & USER_REGISTER_HEATER_ENABLED);
    showReslut("Disable OTP reload: ", reg & USER_REGISTER_DISABLE_OTP_RELOAD);
}
