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
Connect to the CoAP server and send a request.
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

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);

M5GFX display;
M5Canvas canvas(&display);

unsigned long start;

typedef enum { GET = 1, POST, PUT, DELETE } coap_method_t;

inline String time() {
    return "..." + String((millis() - start) / 1000) + 's';
}

void log(String info) {
    SerialMon.println(info);
    canvas.println(info);
    canvas.pushSprite(0, 0);
}

String waitMsg(unsigned long time) {
    String restr        = "";
    unsigned long start = millis();
    while (1) {
        if (SerialAT.available() || (millis() - start) < time) {
            String str = SerialAT.readString();
            restr += str;
        } else {
            break;
        }
    }
    return restr;
}

bool craeteCoAPPacket(String url, String url_query, coap_method_t method,
                      String payload) {
    log("Create CoAPPacket");
    // SerialAT.println("AT+CCOAPPARA=\"CODE\",4,uri-path,0,\"/m5stack-get\",uri-query,0,\"catm_test=1\",payload,0,\"hello
    // world\"");
    SerialAT.println("AT+CCOAPPARA=\"CODE\"," + String(method) +
                     ",uri-path,0,\"" + url + "\",uri-query,0,\"" + url_query +
                     "\",payload,0,\"" + payload + "\"");
    if (waitMsg(2000).indexOf("OK") != -1) {
        return true;
    } else {
        return false;
    }
}

bool connectCoAP(String ip, String port) {
    SerialAT.println("AT+CNACT=0,0");
    log(waitMsg(500));
    SerialAT.println("AT+CNACT=0,1");
    log(waitMsg(500));
    SerialAT.println("AT+CCOAPINIT");
    log(waitMsg(500));
    SerialAT.println("AT+CCOAPURL=\"coap://120.77.157.90:5683\"");
    String result = waitMsg(1000);
    log(result);
    if (result.indexOf("ERROR") != -1) {
        SerialAT.println("AT+CCOAPTERM");
        String result = waitMsg(1000);
        return false;
    } else {
        return true;
    }
}

String sendCoAPRequest() {
    SerialAT.println("AT+CCOAPACTION");
    String result = waitMsg(5000);
    log(result);
    if (result.indexOf("+CCOAPRECV") != -1) {
        String str1 = result.substring(result.indexOf("+CCOAPRECV") + 11);
        String str2 = str1.substring(1, str1.indexOf(","));
        return str2;
    } else {
        return "ERROR";
    }
}

String getCoAPreceive(String msgid) {
    SerialAT.println("AT+CCOAPACTION=4");
    log(waitMsg(2000));
    // Read the packet header with messageidof 1and print
    SerialAT.println("AT+CCOAPHEAD=" + msgid + ",1");
    log(waitMsg(2000));
    // Read the receive packet payload with messageid of 1
    SerialAT.println("AT+CCOAPREAD=" + msgid);
    log(waitMsg(2000));
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
        log(waitMsg(1000));

        return;
    }
    if (modem.isNetworkConnected()) {
        log("Network connected" + time());
    }

    IPAddress local = modem.localIP();
    log("REMOTE IP: " + local.toString());

    int csq = modem.getSignalQuality();
    log("RSSI:" + String(csq) + time());
    log("IP:" + local.toString() + time());

    if (connectCoAP("120.77.157.90", "5683")) {
        log("connect successful, click button a to send request");
    }
    while (1) {
        M5.update();
        if (M5.BtnA.wasPressed()) {
            craeteCoAPPacket("/m5stack-get", "catm_test=1", GET, "hello world");
            String msgid = sendCoAPRequest();
            log(msgid);
            if (msgid != "ERROR") {
                getCoAPreceive(msgid);
            }
        }
        String result = waitMsg(0);
        if (result != "") {
            log(result);
        }
    }
}
