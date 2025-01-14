/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
/*
 * @Hardwares: M5Core + Unit RTC
 * @Platform Version: Arduino M5Stack Board Manager v2.1.3
 * @Dependent Library:
 * M5Stack@^0.4.6: https://github.com/m5stack/M5Stack
 * M5Unit-RTC: https://github.com/m5stack/M5Unit-RTC
 */

#include "M5Stack.h"
#include "Unit_RTC.h"

Unit_RTC RTC;

rtc_time_type RTCtime;
rtc_date_type RTCdate;

char str_buffer[64];

void setup()
{
    M5.begin();             // Init M5Stack.  初始化M5Stack
    M5.Power.begin();       // Init power  初始化电源模块
    M5.lcd.setTextSize(2);  // Set the text size to 2.  设置文字大小为2
    M5.Lcd.print("             RTC");
    RTC.begin();           // Example Initialize the RTC clock.  初始化RTC时钟
    RTCtime.Hours   = 14;  // Set the RTC clock time.  设置RTC时钟时间
    RTCtime.Minutes = 40;
    RTCtime.Seconds = 5;

    RTCdate.WeekDay = 4;  // Set the RTC clock date.  设置RTC时钟日期
    RTCdate.Month   = 7;
    RTCdate.Date    = 15;
    RTCdate.Year    = 2021;

    RTC.setTime(&RTCtime);  // Example Synchronize the set time to the RTC.
                            // 将设置的时间同步至RTC
    RTC.setDate(&RTCdate);  // Synchronize the set date to the RTC.
                            // 将设置的日期同步至RTC
}

void loop()
{
    RTC.getTime(&RTCtime);  // To get the time.  获取时间
    RTC.getDate(&RTCdate);  // Get the date.  获取日期
    M5.Lcd.setCursor(0, 20);
    M5.Lcd.printf("RTC Time Now is \n%02d:%02d:%02d\n", RTCtime.Hours, RTCtime.Minutes, RTCtime.Seconds);
    M5.Lcd.printf("RTC Date Now is \n%02d:%02d:%02d WeekDay:%02d\n", RTCdate.Year, RTCdate.Month, RTCdate.Date,
                  RTCdate.WeekDay);
    delay(1000);
    M5.Lcd.fillRect(0, 20, 320, 140, BLACK);
}
