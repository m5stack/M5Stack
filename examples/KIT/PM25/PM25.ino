#include <M5Stack.h>
#include "Free_Fonts.h"
#include <Wire.h>
#include "M5_ENV.h"

SHT3X sht30;
float temp = 0.0;
float humd = 0.0;
#define TFT_GREY 0x7BEF

#define DATA_LEN 32

#define X_LOCAL 40
#define Y_LOCAL 30

#define X_OFFSET 160
#define Y_OFFSET 23

#define DATA_LEN 32

#define OUT_BAUD 115200
#define SENSOR_BAUD 9600

#define FIRST_START_BIT 66
#define SECOND_START_BIT 77

#define FRONT 2

bool firstStartBitIsRead;
bool secondStartBitIsRead;

uint8_t serial_bytes[DATA_LEN] = {0};
uint8_t currentByte = 0;

void resetSerial() {
    currentByte = 0;

    memset(serial_bytes, 0, sizeof(serial_bytes) / sizeof(serial_bytes[0]) * sizeof(serial_bytes[0]));

    firstStartBitIsRead = false;
    secondStartBitIsRead = false;
}


// Print the header for a display screen
void header(const char *string, uint16_t color) {
    M5.Lcd.fillScreen(color);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_MAGENTA, TFT_BLUE);
    M5.Lcd.fillRect(0, 0, 320, 30, TFT_BLUE);
    M5.Lcd.setTextDatum(TC_DATUM);
    M5.Lcd.drawString(string, 160, 3, 4);
}

void setup() {
    M5.begin();
    Serial.begin(OUT_BAUD);
    Serial2.begin(SENSOR_BAUD, SERIAL_8N1, 16, 17);
    pinMode(13, OUTPUT);
    digitalWrite(13, 1);

    M5.Lcd.fillScreen(TFT_BLACK);
    header("P M 2.5", TFT_BLACK);

    resetSerial();
}


void LCD_Display_Val(void) {
    uint16_t p_val[16] = {0};

    for (int i = 0, j = 0; i < 32; i++) {
        if (i % 2 == 0) {
            p_val[j] = serial_bytes[i];
            p_val[j] = p_val[j] << 8;
        } else {
            p_val[j] |= serial_bytes[i];
            j++;
        }
    }

    //     M5.Lcd.setTextSize(FRONT);
    M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL, FRONT);
    M5.Lcd.print("S P M");

    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);

    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET, FRONT);
    M5.Lcd.print("                     ");
    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET, FRONT);
    M5.Lcd.print("PM1.0 : ");
    M5.Lcd.print(p_val[2]);

    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET * 2, FRONT);
    M5.Lcd.print("                     ");
    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET * 2, FRONT);
    M5.Lcd.print("PM2.5 : ");
    M5.Lcd.print(p_val[3]);

    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET * 3, FRONT);
    M5.Lcd.print("                     ");
    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET * 3, FRONT);
    M5.Lcd.print("PM10  : ");
    M5.Lcd.print(p_val[4]);

    M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
    M5.Lcd.setCursor(X_LOCAL + X_OFFSET, Y_LOCAL, FRONT);
    M5.Lcd.print("A T M E");

    M5.Lcd.setCursor(X_LOCAL + X_OFFSET, Y_LOCAL + Y_OFFSET, FRONT);
    M5.Lcd.print("                     ");
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.setCursor(X_LOCAL + X_OFFSET, Y_LOCAL + Y_OFFSET, FRONT);
    M5.Lcd.print("PM1.0 : ");
    M5.Lcd.print(p_val[5]);

    M5.Lcd.setCursor(X_LOCAL + X_OFFSET, Y_LOCAL + Y_OFFSET * 2, FRONT);
    M5.Lcd.print("                     ");
    M5.Lcd.setCursor(X_LOCAL + X_OFFSET, Y_LOCAL + Y_OFFSET * 2, FRONT);
    M5.Lcd.print("PM2.5 : ");
    M5.Lcd.print(p_val[6]);

    M5.Lcd.setCursor(X_LOCAL + X_OFFSET, Y_LOCAL + Y_OFFSET * 3, FRONT);
    M5.Lcd.print("                     ");
    M5.Lcd.setCursor(X_LOCAL + X_OFFSET, Y_LOCAL + Y_OFFSET * 3, FRONT);
    M5.Lcd.print("PM10  : ");
    M5.Lcd.print(p_val[7]);

    M5.Lcd.setTextColor(TFT_RED, TFT_BLACK);
    M5.Lcd.setCursor(X_LOCAL + X_OFFSET / 4, Y_LOCAL + Y_OFFSET * 4, FRONT);
    M5.Lcd.print("Number of particles");

    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET * 5, FRONT);
    M5.Lcd.print("                     ");
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET * 5, FRONT);
    M5.Lcd.print("0.3um : ");
    M5.Lcd.print(p_val[8]);

    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET * 6, FRONT);
    M5.Lcd.print("                     ");
    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET * 6, FRONT);
    M5.Lcd.print("0.5um : ");
    M5.Lcd.print(p_val[9]);

    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET * 7, FRONT);
    M5.Lcd.print("1.0um : ");
    M5.Lcd.print(p_val[10]);

    M5.Lcd.setCursor(X_LOCAL + X_OFFSET, Y_LOCAL + Y_OFFSET * 5, FRONT);
    M5.Lcd.print("                     ");
    M5.Lcd.setCursor(X_LOCAL + X_OFFSET, Y_LOCAL + Y_OFFSET * 5, FRONT);
    M5.Lcd.print("2.5um : ");
    M5.Lcd.print(p_val[11]);

    M5.Lcd.setCursor(X_LOCAL + X_OFFSET, Y_LOCAL + Y_OFFSET * 6, FRONT);
    M5.Lcd.print("                     ");
    M5.Lcd.setCursor(X_LOCAL + X_OFFSET, Y_LOCAL + Y_OFFSET * 6, FRONT);
    M5.Lcd.print("5.0um : ");
    M5.Lcd.print(p_val[12]);

    M5.Lcd.setCursor(X_LOCAL + X_OFFSET, Y_LOCAL + Y_OFFSET * 7, FRONT);
    M5.Lcd.print("                     ");
    M5.Lcd.setCursor(X_LOCAL + X_OFFSET, Y_LOCAL + Y_OFFSET * 7, FRONT);
    M5.Lcd.print("10um  : ");
    M5.Lcd.print(p_val[13]);

    resetSerial();
}

void TempHumRead(void) {
    temp = 0, humd = 0;

    if (sht30.get() == 0) {  // Obtain the data of shT30.  获取sht30的数据
        temp = sht30.cTemp;  // Store the temperature obtained from shT30.
        // 将sht30获取到的温度存储
        humd = sht30.humidity;  // Store the humidity obtained from the SHT30.
        // 将sht30获取到的湿度存储
    }

    M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET * 8, FRONT);
    M5.Lcd.print("                     ");
    M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + Y_OFFSET * 8, FRONT);
    M5.Lcd.print("T M P : ");
    M5.Lcd.print(temp);

    M5.Lcd.setCursor(X_LOCAL + X_OFFSET, Y_LOCAL + Y_OFFSET * 8, FRONT);
    M5.Lcd.print("                     ");
    M5.Lcd.setCursor(X_LOCAL + X_OFFSET, Y_LOCAL + Y_OFFSET * 8, FRONT);
    M5.Lcd.print("H U M : ");
    M5.Lcd.print(humd);
}

void readCurrentBit() {
    uint8_t bit = Serial2.read();

    if (!firstStartBitIsRead) {
        if (bit != FIRST_START_BIT) {
            resetSerial();
            return;
        }
        firstStartBitIsRead = true;
    } else if (!secondStartBitIsRead) {
        if (bit != SECOND_START_BIT) {
            resetSerial();
            return;
        }
        secondStartBitIsRead = true;
    }

    if (currentByte < DATA_LEN) {
        serial_bytes[currentByte] = bit;
    }

    currentByte++;
}

void loop() {
    if (currentByte >= DATA_LEN) {
        LCD_Display_Val();
        TempHumRead();
    }

    if (Serial2.available()) {
        readCurrentBit();
    }
}
