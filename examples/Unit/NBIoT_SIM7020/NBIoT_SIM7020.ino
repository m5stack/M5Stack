/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/unit/nbiot_global
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/unit/nbiot_global
*
* Describe: NBIoT UNIT.
* Date: 2022/01/04
* Dependent library download:
* TinyGSM:
https://m5stack.oss-cn-shenzhen.aliyuncs.com/resource/arduino/lib/TinyGSM.zip
*******************************************************************************
  Connect NBIoT UNIT to PORT C port (G16/17), connect to MQTT server for data
sending and receiving test. Before compiling, please install and visit the link
above to download the TinyGSM dependency library. 将NBIoT UNIT连接到PORT
C端口(G16/17),
连接MQTT服务器进行数据收发测试。编译前，请安装访问上方链接，下载TinyGSM依赖库。
*/

#include "M5GFX.h"
#include "M5Stack.h"

#define TINY_GSM_MODEM_SIM7020

#include <PubSubClient.h>
#include <TinyGsmClient.h>
#include <sys/time.h>
#include <time.h>

#define SerialAT         Serial1
#define SIM7020_BAUDRATE 115200

#define UNIT_SIM7020_TX 17
#define UNIT_SIM7020_RX 16

#define MQTT_BROKER            "mqtt.m5stack.com"
#define MQTT_PORT              1883
#define MQTT_USERNAME          "UNIT_NBIoT_USER_NAME"
#define MQTT_PASSWORD          "UNIT_NBIoT_PASSWORD"
#define MQTT_SYNC_TIME_D_TOPIC "UNIT_NBIoT/D"
#define MQTT_SYNC_TIME_U_TOPIC "UNIT_NBIoT/U"

#define UPLOAD_INTERVAL 10000

TinyGsm modem(SerialAT, -1);

TinyGsmClient tcpClient(modem);
PubSubClient mqttClient(MQTT_BROKER, MQTT_PORT, tcpClient);

void mqttCallback(char *topic, byte *payload, unsigned int len);
void mqttConnect(void);
void nbConnect(void);

struct tm now;
char s_time[50];

M5GFX display;
M5Canvas canvas(&display);

void log(String info) {
    canvas.println(info);
    canvas.pushSprite(0, 0);
    Serial.println(info);
}

void setup() {
    M5.begin();
    display.begin();
    canvas.setColorDepth(1);  // mono color
    canvas.setFont(&fonts::efontCN_14);
    canvas.createSprite(display.width(), display.height());
    canvas.setTextSize(2);
    canvas.setPaletteColor(1, GREEN);
    canvas.setTextScroll(true);
    canvas.println(">>UNIT NBIoT");
    canvas.println(">>MQTT TEST");
    canvas.pushSprite(0, 0);
    SerialAT.begin(SIM7020_BAUDRATE, SERIAL_8N1, UNIT_SIM7020_RX,
                   UNIT_SIM7020_TX);
    nbConnect();
    mqttClient.setBufferSize(4096);
    mqttClient.setCallback(mqttCallback);
    mqttClient.setKeepAlive(300);
}

void loop() {
    static unsigned long timer = 0;

    if (!mqttClient.connected()) {
        if (!modem.isNetworkConnected()) {
            nbConnect();
        }
        log(">>MQTT NOT CONNECTED");
        mqttConnect();
    }

    if (millis() >= timer) {
        timer = millis() + UPLOAD_INTERVAL;
        mqttClient.publish(MQTT_SYNC_TIME_U_TOPIC, "hello");  // 发送数据
    }

    mqttClient.loop();
}

void mqttCallback(char *topic, byte *payload, unsigned int len) {
    log("Message arrived [" + String(topic) + "]: ");
    String payload_str = String((char *)payload);
    log(payload_str);
}

void mqttConnect(void) {
    log("Connecting to " + String(MQTT_BROKER) + "...");

    // Connect to MQTT Broker
    String mqttid = ("MQTTID_" + String(random(65536)));
    while (!mqttClient.connect(mqttid.c_str(), MQTT_USERNAME, MQTT_PASSWORD)) {
        log(" fail");
    }
    log(" success");
    mqttClient.subscribe(MQTT_SYNC_TIME_D_TOPIC);
    while (!mqttClient.publish(MQTT_SYNC_TIME_U_TOPIC, "Device Online")) {
        delay(1000);
    }
}

void nbConnect(void) {
    unsigned long start = millis();
    log("Initializing modem...");
    while (!modem.init()) {
        log("waiting...." + String((millis() - start) / 1000) + "s");
    };

    start = millis();
    log("Waiting for network...");
    while (!modem.waitForNetwork()) {
        log("waiting...." + String((millis() - start) / 1000) + "s");
    }
    log("success");
}
