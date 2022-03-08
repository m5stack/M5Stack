/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more
information：https://docs.m5stack.com/en/unit/catm_gnss
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/module/catm_gnss
*
* describe: catm_gnss.
* date：2022/03/03
*******************************************************************************
This case will use UNIT CATM+GNSS combined with M5Core
Obtain positioning information through GNSS
Before use, connect the UNIT CATM+GNSS to PORT C(G16/17)
Libraries:
- [TinyGSM](https://github.com/vshymanskyy/TinyGSM)
*/

#include <M5Stack.h>
#include "M5GFX.h"

#define TINY_GSM_MODEM_SIM7080

// Set serial for debug console (to the Serial Monitor, default speed 115200)
#define SerialMon Serial
#define SerialAT  Serial2

uint32_t lastReconnectAttempt = 0;

#define TINY_GSM_RX_BUFFER 650

#define TINY_GSM_DEBUG SerialMon

#define MODULE_BAUD 115200

// Your GPRS credentials, if any
const char apn[]      = "YourAPN";
const char gprsUser[] = "";
const char gprsPass[] = "";

#include <TinyGsmClient.h>
#include <PubSubClient.h>

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);

M5GFX display;
M5Canvas canvas(&display);

const char* broker = "mqtt.m5stack.com";

const char* topic_up   = "cat1/up";
const char* topic_down = "cat1/down";

PubSubClient mqtt(client);

unsigned long start;

inline String time() {
    return "..." + String((millis() - start) / 1000) + 's';
}

void log(String info) {
    SerialMon.println(info);
    canvas.println(info);
    canvas.pushSprite(0, 0);
}

void mqttCallback(char* topic, byte* payload, unsigned int len) {
    log("Message arrived :");
    log(topic);
    log("payload: ");
    char _payload[len];
    memcpy(_payload, payload, len);
    _payload[len] = '\0';
    log(_payload);
}

boolean mqttConnect() {
    log("Connecting to ");
    log(broker);

    // Connect to MQTT Broker
    boolean status = mqtt.connect("GsmClientTest");

    // Or, if you want to authenticate MQTT:
    // boolean status = mqtt.connect("GsmClientName", "mqtt_user", "mqtt_pass");

    if (status == false) {
        SerialMon.println(" fail");
        return false;
    }
    SerialMon.println(" success");
    mqtt.publish(topic_up, "GsmClientTest started");
    mqtt.subscribe(topic_down);
    log("Subscribe Topic: " + String(topic_down));
    return mqtt.connected();
}

void setup() {
    M5.begin();
    display.begin();
    start = millis();
    canvas.setColorDepth(1);  // mono color
    canvas.setFont(&fonts::efontCN_14);
    canvas.createSprite(display.width(), display.height());
    canvas.setTextSize(2);
    canvas.setPaletteColor(1, GREEN);
    canvas.setTextScroll(true);

    log("Initializing modem..." + time());

    // Set GSM module baud rate
    if (TinyGsmAutoBaud(SerialAT, MODULE_BAUD, MODULE_BAUD) == 0) {
        log("UART connect error" + time());
    }
    //   modem.restart();
    modem.init();
    String modemInfo = modem.getModemInfo();
    log("Modem Info: ");
    log(modemInfo + time());
}

bool gps_flag = false;

void loop() {
    canvas.println("INIT GNSS" + time());
    canvas.pushSprite(0, 0);
    while (1) {
        DBG("Enabling GPS/GNSS/GLONASS and waiting 15s for warm-up");
        modem.enableGPS();
        delay(15000L);
        float lat2      = 0;
        float lon2      = 0;
        float speed2    = 0;
        float alt2      = 0;
        int vsat2       = 0;
        int usat2       = 0;
        float accuracy2 = 0;
        int year2       = 0;
        int month2      = 0;
        int day2        = 0;
        int hour2       = 0;
        int min2        = 0;
        int sec2        = 0;
        for (int8_t i = 15; i; i--) {
            DBG("Requesting current GPS/GNSS/GLONASS location");
            if (modem.getGPS(&lat2, &lon2, &speed2, &alt2, &vsat2, &usat2,
                             &accuracy2, &year2, &month2, &day2, &hour2, &min2,
                             &sec2)) {
                log("Latitude:" + String(lat2, 8) +
                    "\tLongitude:" + String(lon2, 8));
                log("Speed:" + String(speed2) + "\tAltitude:" + String(alt2));
                log("Visible Satellites:" + String(vsat2) +
                    "\tUsed Satellites:" + String(usat2));
                log("Accuracy:" + String(accuracy2));
                log("Year:" + String(year2) + "\tMonth:" + String(month2) +
                    "\tDay:" + String(day2));
                log("Hour:" + String(hour2) + "\tMinute:" + String(min2) +
                    "\tSecond:" + String(sec2));
                break;
            } else {
                log("Get GPS/GNSS/GLONASS location.." + time());
                DBG("Couldn't get GPS/GNSS/GLONASS location, retrying in 10s.");
                delay(10000L);
            }
        }
        // modem.disableGPS();
    }
    while (1) {
        delay(100);
    }
}
