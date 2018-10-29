/*
    hardware : m5stack uint dac

    please install adafruit MCP4725 lib
*/
#include <Wire.h>
#include <Adafruit_MCP4725.h>

#define DAC_ADDR
Adafruit_MCP4725 dac;

void setup(void) {
    Serial.begin(115200);
    Serial.println("Hello!");

    // For Adafruit MCP4725A1 the address is 0x62 (default) or 0x63 (ADDR pin tied to VCC)
    // For MCP4725A0 the address is 0x60 or 0x61
    // For MCP4725A2 the address is 0x64 or 0x65
    dac.begin(0x60);
        
    Serial.println("Generating a triangle wave");
    dac.setVoltage(2048, false);

}

void loop(void) {
    // 12bit value , false mean not write EEPROM
    dac.setVoltage(1024, false);
    delay(1000);
    dac.setVoltage(2048, false);
    delay(1000);
}