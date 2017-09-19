/**
 * httpUpdate.ino
 *
 *  Created on: 27.11.2015
 *
 */

#include <Arduino.h>

#include <WiFi.h>
#include <WiFiMulti.h>

#include "ESP8266HTTPClient.h"
#include "ESP8266httpUpdate.h"
#include <WiFiClientSecure.h>

#define USE_SERIAL Serial

WiFiMulti wifiMulti;

void setup() {

    USE_SERIAL.begin(115200);
    // USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();
    // Serial.print("update success!\r\n");

    for(uint8_t t = 4; t > 0; t--) {
        USE_SERIAL.printf("[SETUP] WAIT %d...\n", t);
        USE_SERIAL.flush();
        delay(100);
    }

    WiFi.begin("MasterHax_2.4G", "wittyercheese551");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // wifiMulti.addAP("MasterHax_2.4G", "wittyercheese551");
    // Serial.println("Connecting Wifi...");
    // if(wifiMulti.run() == WL_CONNECTED) {
    //     Serial.println("");
    //     Serial.println("WiFi connected");
    //     Serial.println("IP address: ");
    //     Serial.println(WiFi.localIP());
    // }
}

void loop() {
    // wait for WiFi connection
    // if((wifiMulti.run() == WL_CONNECTED)) {
    if(1) {
        Serial.printf("connect...\n");
        t_httpUpdate_return ret = ESPhttpUpdate.update("http://olcunuug8.bkt.clouddn.com/httpUpdate.ino.esp32.bin");
        //t_httpUpdate_return  ret = ESPhttpUpdate.update("https://server/file.bin");

        switch(ret) {
            case HTTP_UPDATE_FAILED:
                USE_SERIAL.printf("HTTP_UPDATE_FAILD Error (%d): %s", ESPhttpUpdate.getLastError(), ESPhttpUpdate.getLastErrorString().c_str());
                break;

            case HTTP_UPDATE_NO_UPDATES:
                USE_SERIAL.println("HTTP_UPDATE_NO_UPDATES");
                break;

            case HTTP_UPDATE_OK:
                USE_SERIAL.println("HTTP_UPDATE_OK");
                break;
        }
    }
}
