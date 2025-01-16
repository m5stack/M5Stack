/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Module COMX GPS
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 * TinyGPSPlus: https://github.com/mikalhart/TinyGPSPlus
 */

#include <M5Stack.h>
#include <TinyGPS++.h>

// Use COM.GPS Module to get the coordinate data and time of the current
// location.使用 COM.GPS 模块获取当前位置的坐标数据和时间。

// A sample NMEA stream.
const char *gpsStream =
    "$GPRMC,045103.000,A,3014.1984,N,09749.2872,W,0.67,161.46,030913,,,A*"
    "7C\r\n";

// The TinyGPS++ object
TinyGPSPlus gps;

void setup()
{
    M5.begin();
    Serial2.begin(9600, SERIAL_8N1, 5, 13);
    M5.Lcd.setTextColor(GREEN, BLACK);

    //  while (*gpsStream)
    //    if (gps.encode(*gpsStream++))
    //      displayInfo();
}

void loop()
{
    displayInfo();
    smartDelay(1000);
}

static void smartDelay(unsigned long ms)
{
    unsigned long start = millis();
    do {
        while (Serial2.available() > 0) gps.encode(Serial2.read());
    } while (millis() - start < ms);
    M5.Lcd.clear();
}

void displayInfo()
{
    M5.Lcd.setCursor(0, 40, 4);
    M5.Lcd.print(F("Latitude:    "));
    if (gps.location.isValid()) {
        M5.Lcd.print(gps.location.lat(), 6);

    } else {
        M5.Lcd.print(F("INVALID"));
    }

    M5.Lcd.println();
    M5.Lcd.print(F("Longitude:    "));
    if (gps.location.isValid()) {
        M5.Lcd.print(gps.location.lng(), 6);
    } else {
        M5.Lcd.print(F("INVALID"));
    }

    M5.Lcd.println();
    M5.Lcd.print(F("Altitude:    "));
    if (gps.altitude.isValid()) {
        M5.Lcd.print(gps.altitude.meters());
    } else {
        M5.Lcd.print(F("INVALID"));
    }

    M5.Lcd.println();
    M5.Lcd.print(F("Satellites:    "));
    if (gps.satellites.isValid()) {
        M5.Lcd.print(gps.satellites.value());
    } else {
        M5.Lcd.print(F("INVALID"));
    }

    M5.Lcd.println();
    M5.Lcd.print(F("Date: "));
    if (gps.date.isValid()) {
        M5.Lcd.print(gps.date.month());
        M5.Lcd.print(F("/"));
        M5.Lcd.print(gps.date.day());
        M5.Lcd.print(F("/"));
        M5.Lcd.print(gps.date.year());
    } else {
        M5.Lcd.print(F("INVALID"));
    }

    M5.Lcd.println();
    M5.Lcd.print(F("Time: "));
    if (gps.time.isValid()) {
        if (gps.time.hour() < 10) M5.Lcd.print(F("0"));
        M5.Lcd.print(gps.time.hour());
        M5.Lcd.print(F(":"));
        if (gps.time.minute() < 10) M5.Lcd.print(F("0"));
        M5.Lcd.print(gps.time.minute());
        M5.Lcd.print(F(":"));
        if (gps.time.second() < 10) M5.Lcd.print(F("0"));
        M5.Lcd.print(gps.time.second());
        M5.Lcd.print(F("."));
        if (gps.time.centisecond() < 10) M5.Lcd.print(F("0"));
        M5.Lcd.print(gps.time.centisecond());
    } else {
        M5.Lcd.print(F("INVALID"));
    }
}
