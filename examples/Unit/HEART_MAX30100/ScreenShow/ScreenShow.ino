/*
    Description: This example shows how the HEART Unit obtains the original
   value of the heart rate detection and displays the line graph on the screen.
   Before the program runs, put the finger to the sensor detection position.
    Please install library before compiling:
    Arduino-MAX30100: https://github.com/oxullo/Arduino-MAX30100
*/

#include <M5Stack.h>
#include <math.h>

#include "MAX30100.h"

#define SAMPLING_RATE MAX30100_SAMPRATE_50HZ

// The LEDs currents must be set to a level that avoids clipping and maximises
// the dynamic range
#define IR_LED_CURRENT  MAX30100_LED_CURR_11MA
#define RED_LED_CURRENT MAX30100_LED_CURR_11MA

// The pulse width of the LEDs driving determines the resolution of
// the ADC (which is a Sigma-Delta).
// set HIGHRES_MODE to true only when setting PULSE_WIDTH to
// MAX30100_SPC_PW_1600US_16BITS
#define PULSE_WIDTH  MAX30100_SPC_PW_400US_14BITS
#define HIGHRES_MODE true

TFT_eSprite Disbuff = TFT_eSprite(&M5.Lcd);
MAX30100 sensor;

uint64_t realTime[4], time_count = 0;
bool k_ready = false;

void setup() {
    // put your setup code here, to run once:
    M5.begin();
    M5.Power.begin();
    M5.Lcd.setSwapBytes(false);
    Disbuff.createSprite(160, 160);
    Disbuff.setSwapBytes(true);
    Disbuff.createSprite(160, 160);

    if (!sensor.begin()) {
        Serial.println("FAILED");
        for (;;)
            ;
    } else {
        Serial.println("SUCCESS");
    }

    sensor.setMode(MAX30100_MODE_SPO2_HR);
    sensor.setLedsCurrent(IR_LED_CURRENT, RED_LED_CURRENT);
    sensor.setLedsPulseWidth(PULSE_WIDTH);
    sensor.setSamplingRate(SAMPLING_RATE);
    sensor.setHighresModeEnabled(HIGHRES_MODE);
    sensor.resetFifo();
}

uint16_t line[2][320] = {0};
int16_t k_number[320] = {0};
int16_t raw_data[512];
int16_t k_min;
int16_t k_threshold;
uint32_t k_pos_buff[32];
double k_pos_time[32];
double k_standard_deviation, k_sumdata;
uint32_t k_pos, k_pos_count = 0;

uint32_t led_pos = 0, ir_Pos = 0;
uint16_t ir_max = 0, led_max = 0, ir_min = 0, led_min = 0, ir_last = 0,
         led_last    = 0;
int16_t k_number_min = 0, k_number_max = 0, k_last = 0;
uint16_t ir_last_raw = 0, led_last_raw = 0, k_last_raw = 0;
uint16_t ir_disdata, led_disdata, k_disdata;
uint16_t Alpha     = 0.3 * 256;
uint16_t count_min = 0, count_max = 0;

void loop() {
    // put your main code here, to run repeatedly:
    uint16_t ir, red;

    // Serial.printf("raed fifo Length :%d\n", read_length);
    while (sensor.getRawValues(&ir, &red) == false) {
        delay(10);
        sensor.update();
    }

    while (1) {
        line[0][(led_pos + 160) % 320] =
            (led_last_raw * (256 - Alpha) + red * Alpha) / 256;
        line[1][(ir_Pos + 160) % 320] =
            (ir_last_raw * (256 - Alpha) + ir * Alpha) / 256;

        k_number[(led_pos + 160) % 320] =
            line[0][(led_pos + 160) % 320] - led_last_raw;

        led_last_raw = line[0][(led_pos + 160) % 320];
        ir_last_raw  = line[1][(led_pos + 160) % 320];
        led_pos++;
        ir_Pos++;
        if (sensor.getRawValues(&ir, &red) == false) {
            break;
        }
    }

    sensor.resetFifo();

    for (int i = 0; i < 160; i++) {
        if (i == 0) {
            led_max = led_min = line[0][(led_pos + i) % 320];
            ir_max = ir_min = line[1][(ir_Pos + i) % 320];
            k_number_min = k_number_max = k_number[(ir_Pos + i) % 320];
        } else {
            led_max = (line[0][(led_pos + i) % 320] > led_max)
                          ? line[0][(led_pos + i) % 320]
                          : led_max;
            led_min = (line[0][(led_pos + i) % 320] < led_min)
                          ? line[0][(led_pos + i) % 320]
                          : led_min;

            ir_max = (line[1][(ir_Pos + i) % 320] > ir_max)
                         ? line[1][(ir_Pos + i) % 320]
                         : ir_max;
            ir_min = (line[1][(ir_Pos + i) % 320] < ir_min)
                         ? line[1][(ir_Pos + i) % 320]
                         : ir_min;

            k_number_max = (k_number[(ir_Pos + i) % 320] > k_number_max)
                               ? k_number[(ir_Pos + i) % 320]
                               : k_number_max;
            k_number_min = (k_number[(ir_Pos + i) % 320] < k_number_min)
                               ? k_number[(ir_Pos + i) % 320]
                               : k_number_min;
        }
    }

    Disbuff.fillRect(0, 0, 160, 160, BLACK);

    for (int i = 0; i < 160; i++) {
        led_disdata =
            map(line[0][(led_pos + i) % 320], led_max, led_min, 0, 160);
        ir_disdata = map(line[1][(ir_Pos + i) % 320], ir_max, ir_min, 0, 160);
        k_disdata  = map(k_number[(ir_Pos + i) % 320], k_number_max,
                        k_number_min, 0, 160);
        {
            Disbuff.drawLine(i, led_last, i + 1, led_disdata, WHITE);
            Disbuff.drawLine(i, ir_last, i + 1, ir_disdata, RED);
            Disbuff.drawLine(i, k_last, i + 1, k_disdata, GREEN);
        }
        ir_last  = ir_disdata;
        led_last = led_disdata;
        k_last   = k_disdata;
    }

    Disbuff.setTextSize(1);
    Disbuff.setTextColor(WHITE);

    Disbuff.setCursor(5, 5);
    Disbuff.printf("led:%d,%d,%d", led_max, led_min, led_max - led_min);
    Disbuff.setCursor(5, 15);
    Disbuff.printf("ir:%d,%d,%d", ir_max, ir_min, ir_max - ir_min);

    Disbuff.pushSprite(0, 0);

    delay(10);
}