/*
    hardware: M5Stack unit ADC
    connect: PORTA(21, 22) or PORTC(17, 16)
*/
#include <M5Stack.h>
#include <Wire.h>
#include "ADS1100.h"

ADS1100 ads;

void setup(void)
{
    M5.begin(true, false, false);
    Serial.begin(115200);

    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(ORANGE);

    // The address can be changed making the option of connecting multiple devices
    ads.getAddr_ADS1100(ADS1100_DEFAULT_ADDRESS);   // 0x48, 1001 000 (ADDR = GND)

    // The ADC gain (PGA), Device operating mode, Data rate
    // can be changed via the following functions

    ads.setGain(GAIN_ONE);          // 1x gain(default)
    // ads.setGain(GAIN_TWO);       // 2x gain
    // ads.setGain(GAIN_FOUR);      // 4x gain
    // ads.setGain(GAIN_EIGHT);     // 8x gain

    ads.setMode(MODE_CONTIN);       // Continuous conversion mode (default)
    // ads.setMode(MODE_SINGLE);    // Single-conversion mode

    ads.setRate(RATE_8);            // 8SPS (default)
    // ads.setRate(RATE_16);        // 16SPS
    // ads.setRate(RATE_32);        // 32SPS
    // ads.setRate(RATE_128);       // 128SPS

    ads.setOSMode(OSMODE_SINGLE);   // Set to start a single-conversion

    ads.begin();
}

void loop(void)
{
    byte error;
    int8_t address;

    address = ads.ads_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        int16_t result;

        Serial.println("Getting Differential Reading from ADS1100");
        Serial.println(" ");
        result = ads.Measure_Differential();
        Serial.print("Digital Value of Analog Input between Channel 0 and 1: ");
        Serial.println(result);
        M5.Lcd.fillScreen(BLACK);
        char data[20] = { 0 };
        sprintf(data, "%d", result);
        M5.Lcd.drawCentreString(data, 160, 100, 4);
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("ADS1100 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}