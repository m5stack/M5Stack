/*
*******************************************************************************
* Copyright (c) 2022 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more
information: https://docs.m5stack.com/en/module/comx_cat1
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/module/comx_cat1
*
* Describe: comx_cat1.
* Date: 2022/01/11
*******************************************************************************
This case will use COM.CAT1 Module combined with M5Core to implement MQTT
Client. After successfully connecting to MQTT, press button B to realize data
publishing. Before use, adjust the DIP switch of the module base to G16/17 ON
Libraries:
- [TinyGSM](https://github.com/vshymanskyy/TinyGSM)
- [PubSubClient](https://github.com/knolleary/pubsubclient.git)
*/

#include <M5Stack.h>

#include "M5GFX.h"

// Compatible with SIM76XX series.
#define TINY_GSM_MODEM_SIM7600

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

#include <PubSubClient.h>
#include <TinyGsmClient.h>

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
    while (!modem.getSimStatus()) {
        log("not sim card" + time());
    }
}

void loop() {
    log("Waiting for network...." + time());
    if (!modem.waitForNetwork()) {
        log("fail" + time());
        delay(10000);
        return;
    }
    if (modem.isNetworkConnected()) {
        log("Network connected" + time());
    }
    log("GPRS connect..." + time());
    if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
        log("fail");
        delay(10000);
        return;
    }
    if (modem.isGprsConnected()) {
        log("GPRS connected");
    }

    String ccid = modem.getSimCCID();
    log("CCID: ");
    log(ccid);

    String imei = modem.getIMEI();
    log("IMEI: " + imei);

    String imsi = modem.getIMSI();
    log("IMSI: " + imsi);

    String cop = modem.getOperator();
    log("Operator: " + cop);

    IPAddress local = modem.localIP();
    log("REMOTE IP: " + local.toString());

    int csq = modem.getSignalQuality();
    log("RSSI:" + String(csq) + time());
    log("IP:" + local.toString() + time());

    // MQTT Broker setup
    mqtt.setServer(broker, 1883);
    mqtt.setCallback(mqttCallback);

    while (true) {
        M5.update();
        if (!mqtt.connected()) {
            log("=== MQTT NOT CONNECTED ===");
            // Reconnect every 10 seconds
            uint32_t t = millis();
            if (t - lastReconnectAttempt > 3000L) {
                lastReconnectAttempt = t;
                if (mqttConnect()) {
                    lastReconnectAttempt = 0;
                    log("mqtt.m5stack.com" + time());
                    log("MQTT Connected!" + time());
                    log("Press Btn B to Publish");
                    log("Topic: " + String(topic_up));
                }
            }
            delay(100);
        } else {
            mqtt.loop();
            M5.update();
            if (M5.BtnB.wasPressed()) {
                log("Publish:" + String(topic_up));
                mqtt.publish(topic_up, "Hello From COM.X CAT1");
            }
        }
    }
}
