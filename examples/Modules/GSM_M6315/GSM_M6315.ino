/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Module GSM
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 */

#include <M5Stack.h>

// Description: Send AT command to get current signal quality.

void IotWriteCommand(char cmd[], char date[])
{
    char buf[256] = {0};

    if (cmd == NULL)
        sprintf(buf, "AT\r\n");
    else if (date == NULL)
        sprintf(buf, "AT+%s\r\n", cmd);
    else
        sprintf(buf, "AT+%s=%s\r\n", cmd, date);

    Serial2.write(buf);
}
// AT+CSQ=?
void get_time(void)
{
    IotWriteCommand("CSQ=?", NULL);
    while (Serial2.available()) {
        uint8_t ch = Serial2.read();
        Serial.write(ch);
        M5.Lcd.write(ch);
    }
}

void setup()
{
    M5.begin();
    M5.Power.begin();

    Serial2.begin(115200, SERIAL_8N1, 16, 17);
    pinMode(5, OUTPUT);
    digitalWrite(5, 1);
}

void loop()
{
    if (M5.BtnA.wasReleased()) {
        M5.Lcd.fillScreen(TFT_BLACK);
        M5.Lcd.setCursor(60, 80, 2);
        get_time();
    }
    M5.update();
}
