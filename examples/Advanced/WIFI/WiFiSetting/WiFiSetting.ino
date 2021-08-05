/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/gray
*
* describe：WiFi connect.  wifi连接
* date：2021/7/27
*******************************************************************************
*/
#include <M5Stack.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include "WebServer.h"
#include <Preferences.h>

boolean restoreConfig();
boolean checkConnection();
void startWebServer();
void setupMode();
String makePage(String title, String contents);
String urlDecode(String input);

const IPAddress apIP(192, 168, 4, 1); //Define the address of the wireless AP.  定义无线AP的地址
const char* apSSID = "M5STACK_SETUP"; //Define the name of the created hotspot.  定义创建热点的名称
boolean settingMode;
String ssidList;
String wifi_ssid; //Store the name of the wireless network.  存储无线网络的名称
String wifi_password; //Store the password of the wireless network.  存储无线网络的密码

// DNSServer dnsServer;.  webServer的类
WebServer webServer(80);

// wifi config store.  wifi配置存储的类
Preferences preferences;

void setup() {
  M5.begin(); //Init M5Stack.  初始化M5Stack
  M5.Power.begin(); //Init power  初始化电源模块
  M5.Lcd.setTextColor(YELLOW);  //Set the font color to yellow.  设置字体颜色为黄色
  preferences.begin("wifi-config");
  delay(10);
  if (restoreConfig()) {  //Check if wifi configuration information has been stored.  检测是否已存储wifi配置信息
    if (checkConnection()) {  //Check wifi connection.  检测wifi连接情况
      settingMode = false;  //Turn off setting mode.  关闭设置模式
      startWebServer(); //Turn on network service.  开启网络服务
      return; //Exit setup().  退出setup()
    }
  }
  settingMode = true; //If there is no stored wifi configuration information, turn on the setting mode.  若没有已存储的wifi配置信息,则开启设置模式
  setupMode();
}

void loop() {
  if (settingMode) {
  }
  webServer.handleClient(); //检查有没有设备通过网络向M5Stack网络服务器发送请求
}

boolean restoreConfig() { /* Check whether there is wifi configuration information storage, if there is 1 return, if no return 0.
                          检测是否有wifi配置信息存储,若有返回1,无返回0 */
  wifi_ssid = preferences.getString("WIFI_SSID");
  wifi_password = preferences.getString("WIFI_PASSWD");
  M5.Lcd.printf("WIFI-SSID: %s\n",wifi_ssid);  //Screen print format string.  屏幕打印格式化字符串
  M5.Lcd.printf("WIFI-PASSWD: %s\n",wifi_password);
  WiFi.begin(wifi_ssid.c_str(), wifi_password.c_str());

  if(wifi_ssid.length() > 0) {
    return true;
} else {
    return false;
  }
}

boolean checkConnection() { //Check wifi connection.  检测wifi连接情况
  int count = 0;  //count.  计数
  M5.Lcd.print("Waiting for Wi-Fi connection");
  while ( count < 30 ) {  /* If you fail to connect to wifi within 30*350ms (10.5s), return false; otherwise return true.
                          若在30*500ms(15s)内未能连上wifi,返回false;否则返回true */
    if (WiFi.status() == WL_CONNECTED) {
      M5.Lcd.printf("\nConnected!\n");
      return (true);
    }
    delay(350);
    M5.Lcd.print(".");
    count++;
  }
  M5.Lcd.println("Timed out.");
  return false;
}

void startWebServer() { //Open the web service.  打开Web服务
  if (settingMode) {  //If the setting mode is on.  如果设置模式处于开启状态
    M5.Lcd.print("Starting Web Server at: ");
    M5.Lcd.print(WiFi.softAPIP());  //Output AP address (you can change the address you want through apIP at the beginning).  输出AP地址(可通过开头的apIP更改自己想要的地址)
    webServer.on("/settings", []() {  //AP web interface settings.  AP网页界面设置
      String s = "<h1>Wi-Fi Settings</h1><p>Please enter your password by selecting the SSID.</p>";
      s += "<form method=\"get\" action=\"setap\"><label>SSID: </label><select name=\"ssid\">";
      s += ssidList;
      s += "</select><br>Password: <input name=\"pass\" length=64 type=\"password\"><input type=\"submit\"></form>";
      webServer.send(200, "text/html", makePage("Wi-Fi Settings", s));
    });
    webServer.on("/setap", []() {
      String ssid = urlDecode(webServer.arg("ssid"));
      M5.Lcd.printf("SSID: %s\n",ssid);
      String pass = urlDecode(webServer.arg("pass"));
      M5.Lcd.printf("Password: %s\n\nWriting SSID to EEPROM...\n",pass);

      // Store wifi config.  存储wifi配置信息
      M5.Lcd.println("Writing Password to nvr...");
      preferences.putString("WIFI_SSID", ssid);
      preferences.putString("WIFI_PASSWD", pass);

      M5.Lcd.println("Write nvr done!");
      String s = "<h1>Setup complete.</h1><p>device will be connected to \"";
      s += ssid;
      s += "\" after the restart.";
      webServer.send(200, "text/html", makePage("Wi-Fi Settings", s));
      delay(2000);
      ESP.restart();  //Restart MPU.  重启MPU
    });
    webServer.onNotFound([]() {
      String s = "<h1>AP mode</h1><p><a href=\"/settings\">Wi-Fi Settings</a></p>";
      webServer.send(200, "text/html", makePage("AP mode", s));
    });
  }
  else {  //If the setting mode is off.  如果设置模式处于关闭状态
    M5.Lcd.print("Starting Web Server at ");
    M5.Lcd.println(WiFi.localIP());
    webServer.on("/", []() {  //AP web interface settings.  AP网页界面设置
      String s = "<h1>STA mode</h1><p><a href=\"/reset\">Reset Wi-Fi Settings</a></p>";
      webServer.send(200, "text/html", makePage("STA mode", s));
    });
    webServer.on("/reset", []() {
      // reset the wifi config
      preferences.remove("WIFI_SSID");
      preferences.remove("WIFI_PASSWD");
      String s = "<h1>Wi-Fi settings was reset.</h1><p>Please reset device.</p>";
      webServer.send(200, "text/html", makePage("Reset Wi-Fi Settings", s));
      delay(2000);
      ESP.restart();
    });
  }
  webServer.begin();  //Start web service.  开启web服务
}

void setupMode() {
  WiFi.mode(WIFI_MODE_STA); //Set Wi-Fi mode to WIFI_MODE_STA.  设置Wi-Fi模式为WIFI_MODE_STA
  WiFi.disconnect();  //Disconnect wifi connection.  断开wifi连接
  delay(100);
  int n = WiFi.scanNetworks();  //Store the number of wifi scanned into n.  将扫描到的wifi个数存储到n中
  delay(100);
  M5.Lcd.println("");
  for (int i = 0; i < n; ++i) { //Save each wifi name scanned to ssidList.  将扫描到的每个wifi名称保存到ssidList中
    ssidList += "<option value=\"";
    ssidList += WiFi.SSID(i);
    ssidList += "\">";
    ssidList += WiFi.SSID(i);
    ssidList += "</option>";
  }
  delay(100);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(apSSID);  //Turn on Ap mode.  开启Ap模式
  WiFi.mode(WIFI_MODE_AP);  //Set WiFi to soft-AP mode.  设置WiFi为soft-AP模式
  startWebServer(); //Open the web service.  打开Web服务
  M5.Lcd.printf("\nStarting Access Point at \"%s\"\n\n",apSSID);
}

String makePage(String title, String contents) {
  String s = "<!DOCTYPE html><html><head>";
  s += "<meta name=\"viewport\" content=\"width=device-width,user-scalable=0\">";
  s += "<title>";
  s += title;
  s += "</title></head><body>";
  s += contents;
  s += "</body></html>";
  return s;
}

String urlDecode(String input) {
  String s = input;
  s.replace("%20", " ");
  s.replace("+", " ");
  s.replace("%21", "!");
  s.replace("%22", "\"");
  s.replace("%23", "#");
  s.replace("%24", "$");
  s.replace("%25", "%");
  s.replace("%26", "&");
  s.replace("%27", "\'");
  s.replace("%28", "(");
  s.replace("%29", ")");
  s.replace("%30", "*");
  s.replace("%31", "+");
  s.replace("%2C", ",");
  s.replace("%2E", ".");
  s.replace("%2F", "/");
  s.replace("%2C", ",");
  s.replace("%3A", ":");
  s.replace("%3A", ";");
  s.replace("%3C", "<");
  s.replace("%3D", "=");
  s.replace("%3E", ">");
  s.replace("%3F", "?");
  s.replace("%40", "@");
  s.replace("%5B", "[");
  s.replace("%5C", "\\");
  s.replace("%5D", "]");
  s.replace("%5E", "^");
  s.replace("%5F", "-");
  s.replace("%60", "`");
  return s;
}