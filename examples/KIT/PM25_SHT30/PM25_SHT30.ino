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

uint16_t CheckSum;
uint16_t CheckCode;

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
    Serial.begin(9600);
    Serial2.begin(9600, SERIAL_8N1, 16, 17);
    pinMode(13, OUTPUT);
    digitalWrite(13, 1);

    M5.Lcd.fillScreen(TFT_BLACK);
    header("P M 2.5", TFT_BLACK);
}

uint8_t Air_val[32] = {0};
int16_t p_val[16]   = {0};
uint8_t i           = 0;

#define FRONT 2

void LCD_Display_Val(void) {
    for (int i = 0, j = 0; i < 32; i++) {
        if (i % 2 == 0) {
            p_val[j] = Air_val[i];
            p_val[j] = p_val[j] << 8;
        } else {
            p_val[j] |= Air_val[i];
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
}

void TempHumRead(void) {
    if (sht30.get() == 0) {  // Obtain the data of shT30.  获取sht30的数据
        temp = sht30.cTemp;  // Store the temperature obtained from shT30.
                             // 将sht30获取到的温度存储
        humd = sht30.humidity;  // Store the humidity obtained from the SHT30.
                                // 将sht30获取到的湿度存储
    } else {
        temp = 0, humd = 0;
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

void loop() {
    if (Serial2.available()) {
        Air_val[i] = Serial2.read();
        Serial.write(Air_val[i]);
        i++;
    } else {
        i = 0;
    }

    if (i == DATA_LEN) {
        LCD_Display_Val();
        TempHumRead();
    }
}
