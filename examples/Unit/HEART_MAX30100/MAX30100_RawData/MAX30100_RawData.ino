/*
    Description: This example shows how the HEART Unit obtains the original value of the heart rate detection and displays it on the screen. The user can also use the Arduino IDE Serial Plotter to view the line graph output.
    Please install library before compiling:  
    Arduino-MAX30100: https://github.com/oxullo/Arduino-MAX30100
*/
#include <Wire.h>
#include "MAX30100.h"
#include <M5Stack.h>
// Sampling is tightly related to the dynamic range of the ADC.
// refer to the datasheet for further info
#define SAMPLING_RATE                       MAX30100_SAMPRATE_100HZ

// The LEDs currents must be set to a level that avoids clipping and maximises the
// dynamic range
#define IR_LED_CURRENT                      MAX30100_LED_CURR_24MA
#define RED_LED_CURRENT                     MAX30100_LED_CURR_27_1MA

// The pulse width of the LEDs driving determines the resolution of
// the ADC (which is a Sigma-Delta).
// set HIGHRES_MODE to true only when setting PULSE_WIDTH to MAX30100_SPC_PW_1600US_16BITS
#define PULSE_WIDTH                         MAX30100_SPC_PW_1600US_16BITS
#define HIGHRES_MODE                        true


// Instantiate a MAX30100 sensor class
MAX30100 sensor;

void setup()
{
    Serial.begin(115200);
    M5.Power.begin();
    M5.begin();
    Serial.print("Initializing MAX30100..");

    // Initialize the sensor
    // Failures are generally due to an improper I2C wiring, missing power supply
    // or wrong target chip
    while (!sensor.begin()) {
        Serial.println("Sensor not found");
        M5.Lcd.setTextFont(4);
        M5.Lcd.setCursor(50, 100, 4);
        M5.Lcd.println("Sensor not found");
        delay(1000);
    } 

    Serial.println("SUCCESS");
    

    // Set up the wanted parameters
    sensor.setMode(MAX30100_MODE_SPO2_HR);
    sensor.setLedsCurrent(IR_LED_CURRENT, RED_LED_CURRENT);
    sensor.setLedsPulseWidth(PULSE_WIDTH);
    sensor.setSamplingRate(SAMPLING_RATE);
    sensor.setHighresModeEnabled(HIGHRES_MODE);
}

void loop()
{
    uint16_t ir, red;

    sensor.update();

    while (sensor.getRawValues(&ir, &red)) {
        M5.Lcd.setTextFont(4);
        M5.Lcd.setCursor(100, 100, 4);
        M5.Lcd.printf("IR:%d               ",ir);
        M5.Lcd.setCursor(100, 130, 4);
        M5.Lcd.printf("RED:%d              ",red);

        Serial.print(ir);
        Serial.print('\t');
        Serial.println(red);
    }
}
