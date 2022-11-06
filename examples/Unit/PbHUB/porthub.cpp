#include "porthub.h"

PortHub::PortHub() {
}

PortHub::PortHub(uint8_t iic_addr) {
    _iic_addr = iic_addr;
}

void PortHub::begin() {
    Wire.begin();
}

uint16_t PortHub::hub_a_read_value(uint8_t reg) {
    Wire.beginTransmission(_iic_addr);
    Wire.write(reg | 0x06);
    Wire.endTransmission();

    uint8_t RegValue_L, RegValue_H;

    Wire.requestFrom(_iic_addr, 2);
    while (Wire.available()) {
        RegValue_L = Wire.read();
        RegValue_H = Wire.read();
    }

    return (RegValue_H << 8) | RegValue_L;
}

uint8_t PortHub::hub_d_read_value_A(uint8_t reg) {
    Wire.beginTransmission(_iic_addr);
    Wire.write(reg | 0x04);
    Wire.endTransmission();

    uint8_t RegValue;

    Wire.requestFrom(_iic_addr, 1);
    while (Wire.available()) {
        RegValue = Wire.read();
    }
    return RegValue;
}

uint8_t PortHub::hub_d_read_value_B(uint8_t reg) {
    Wire.beginTransmission(_iic_addr);
    Wire.write(reg | 0x05);
    Wire.endTransmission();

    uint8_t RegValue;

    Wire.requestFrom(_iic_addr, 1);
    while (Wire.available()) {
        RegValue = Wire.read();
    }
    return RegValue;
}

void PortHub::hub_d_wire_value_A(uint8_t reg, uint16_t level) {
    Wire.beginTransmission(_iic_addr);
    Wire.write(reg | 0x00);
    Wire.write(level & 0xff);
    Wire.endTransmission();
}

void PortHub::hub_d_wire_value_B(uint8_t reg, uint16_t level) {
    Wire.beginTransmission(_iic_addr);
    Wire.write(reg | 0x01);
    Wire.write(level & 0xff);
    Wire.endTransmission();
}

void PortHub::hub_a_wire_value_A(uint8_t reg, uint16_t duty) {
    Wire.beginTransmission(_iic_addr);
    Wire.write(reg | 0x02);
    Wire.write(duty & 0xff);
    Wire.endTransmission();
}

void PortHub::hub_a_wire_value_B(uint8_t reg, uint16_t duty) {
    Wire.beginTransmission(_iic_addr);
    Wire.write(reg | 0x03);
    Wire.write(duty & 0xff);
    Wire.endTransmission();
}

void PortHub::hub_wire_length(uint8_t reg, uint16_t length) {
    Wire.beginTransmission(_iic_addr);
    Wire.write(reg | 0x08);
    Wire.write(length & 0xff);
    Wire.write(length >> 8);
    Wire.endTransmission();
}

void PortHub::hub_wire_index_color(uint8_t reg, uint16_t num, uint8_t r,
                                   int8_t g, uint8_t b) {
    Wire.beginTransmission(_iic_addr);
    Wire.write(reg | 0x09);
    Wire.write(num & 0xff);
    Wire.write(num >> 8);
    Wire.write(r);
    Wire.write(g);
    Wire.write(b);
    Wire.endTransmission();
}

void PortHub::hub_wire_fill_color(uint8_t reg, uint16_t first, uint16_t count,
                                  uint8_t r, int8_t g, uint8_t b) {
    Wire.beginTransmission(_iic_addr);
    Wire.write(reg | 0x0a);
    Wire.write(first & 0xff);
    Wire.write(first >> 8);

    Wire.write(count & 0xff);
    Wire.write(count >> 8);

    Wire.write(r);
    Wire.write(g);
    Wire.write(b);
    Wire.endTransmission();
}

void PortHub::hub_wire_setBrightness(uint8_t reg, uint8_t brightness) {
    Wire.beginTransmission(_iic_addr);
    Wire.write(reg | 0x0b);
    Wire.write(brightness & 0xff);
    Wire.endTransmission();
}
