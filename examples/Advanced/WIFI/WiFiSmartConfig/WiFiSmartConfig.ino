/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/gray
*
* Describe: WIFI Smart Config.  WIFI智能配网
* Date: 2021/7/30
*******************************************************************************
*  Fill in WIFI configuration information through mobile APP to connect M5Core
*to relevant WIFI 通过手机APP填写WIFI配置信息使 M5Core连接至相关WIFI APP
*Download Address:
*https://www.espressif.com/en/products/software/esp-touch/resources APP下载地址:
*https://www.espressif.com/zh-hans/products/software/esp-touch/resources
*/
#include <M5Stack.h>

#include "WiFi.h"

void setup() {
    M5.begin();               // Init M5Core. 初始化 M5Core
    M5.Power.begin();         // Init power  初始化电源模块
    WiFi.mode(WIFI_AP_STA);   // Set the wifi mode to the mode compatible with
                              // the AP and Station, and start intelligent
                              // network configuration
    WiFi.beginSmartConfig();  // 设置wifi模式为AP 与 Station
                              // 兼容模式,并开始智能配网

    // Wait for the M5Core to receive network information from the phone
    //等待M5Core接收到来自手机的配网信息
    M5.Lcd.println(
        "\nWaiting for Phone SmartConfig.");  // Screen print format string.
                                              // 屏幕打印格式化字符串
    while (!WiFi.smartConfigDone()) {  // If the smart network is not completed.
                                       // 若智能配网没有完成
        delay(500);
        M5.Lcd.print(".");
    }
    M5.Lcd.println("\nSmartConfig received.");
    M5.Lcd.println("Waiting for WiFi");
    while (
        WiFi.status() !=
        WL_CONNECTED) {  // M5Core will connect automatically upon receipt of
                         // the configuration information, and return true if
                         // the connection is successful.
                         // 收到配网信息后M5Core将自动连接，若连接成功将返回true
        delay(500);
        M5.Lcd.print(".");
    }
    M5.Lcd.print("\nWiFi Connect To: ");
    M5.Lcd.println(WiFi.SSID());  // Output Network name.  输出网络名称
    M5.Lcd.print("IP address: ");
    M5.Lcd.println(WiFi.localIP());  // Output IP Address.  输出IP地址
    M5.Lcd.print("RSSI: ");
    M5.Lcd.println(WiFi.RSSI());  // Output signal strength.  输出信号强度
}

void loop() {
}