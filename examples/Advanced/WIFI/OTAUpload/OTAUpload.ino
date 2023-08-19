/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/core/gray
*
* Describe: OTA Upload.  隔空传输程序
* Date: 2021/7/30
*******************************************************************************
  PC and M5Core can only be used on the same wifi.
电脑和M5Core需在同一wifi下才可使用 When the OTA is ready, restart the Arduino
client from Tools > Ports > Network ports to instantly transmit the program
wirelessly. OTA 准备好后重启Arduino客户端在工具->端口->网络端口,即刻无线传输程序
*/
#include <ArduinoOTA.h>
#include <M5Stack.h>
#include <WiFi.h>

// Set the name and password of the wifi to be connected.
// 配置所连接wifi的名称和密码
const char* ssid     = "M5wifi";
const char* password = "1234";

void setup() {
    M5.begin();  // Init M5Core. 初始化 M5Core
    M5.Power.begin();
    WiFi.begin(ssid, password);  // Connect wifi and return connection status.
                                 // 连接wifi并返回连接状态
    M5.lcd.print("Waiting Wifi Connect");
    while (WiFi.status() !=
           WL_CONNECTED) {  // If the wifi connection fails.  若wifi未连接成功
        delay(1000);
        M5.lcd.print(".");
    }
    M5.lcd.println("\nWiFi Connected!");
    M5.lcd.print("WiFi Connect To: ");
    M5.lcd.println(WiFi.SSID());  // Output Network name.  输出网络名称
    M5.lcd.print("IP address: ");
    M5.lcd.println(WiFi.localIP());  // Output IP Address.  输出IP地址

    ArduinoOTA.setHostname(
        "M5Core");  // Set the network port name.  设置网络端口名称
    ArduinoOTA.setPassword("666666");  // Set the network port connection
                                       // password.  设置网络端口连接的密码
    ArduinoOTA.begin();            // Initialize the OTA.  初始化OTA
    M5.lcd.println("OTA ready!");  // M5.lcd port output format string.
                                   // 串口输出格式化字符串
}

void loop() {
    ArduinoOTA.handle();  // Continuously check for update requests.
                          // 持续检测是否有更新请求
    M5.update();
    if (M5.BtnA.isPressed()) {  // if BtnA is Pressed.  如果按键A按下
        ArduinoOTA.end();       // Ends the ArduinoOTA service.  结束OTA服务
        M5.lcd.println("OTA End!");
        delay(200);
    }
}