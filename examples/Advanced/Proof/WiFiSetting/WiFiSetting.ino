#include <M5Stack.h>
#include <WiFi.h>
// #include <DNSServer.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
// #include <EEPROM.h>
#include <WebServer.h>
#include <ArduinoJson.h>          //https://github.com/bblanchon/ArduinoJson

const IPAddress apIP(192, 168, 1, 1);
const char* apSSID = "M5STACK_SETUP";
boolean settingMode;
String ssidList;
char wifi_ssid[40];
char wifi_password[40];

// DNSServer dnsServer;
WebServer webServer(80);

void writeFile(fs::FS &fs, const char * path, const char * message) {
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message)) {
    Serial.println("File written");
  } else {
    Serial.println("Write failed");
  }
}

void deleteFile(fs::FS &fs, const char * path) {
  Serial.printf("Deleting file: %s\n", path);
  if (fs.remove(path)) {
    Serial.println("File deleted");
  } else {
    Serial.println("Delete failed");
  }
}

void setup() {
  m5.begin();
  //   EEPROM.begin(512);
  delay(10);
  if (restoreConfig()) {
    if (checkConnection()) {
      settingMode = false;
      startWebServer();
      return;
    }
  }
  settingMode = true;
  setupMode();
}

void loop() {
  if (settingMode) {
    // dnsServer.processNextRequest();
  }
  webServer.handleClient();
}

boolean restoreConfig() {
  // char fsbuffer[200];
  const char* filename = "/wifi-config.json";
  Serial.printf("Reading %s\r\n", filename);
  if (SD.exists(filename)) {
    //file exists, reading and loading
    Serial.println("reading config file");
    File configFile = SD.open(filename, "r");
    if (configFile) {
      Serial.println("opened config file");
      size_t size = configFile.size();
      // Allocate a buffer to store contents of the file.
      std::unique_ptr<char[]> fsbuffer(new char[size]);

      configFile.readBytes(fsbuffer.get(), size);
      // configFile.readBytes(fsbuffer, size);
      fsbuffer[size] = 0;
      DynamicJsonBuffer jsonBuffer;
      // JsonObject& json = jsonBuffer.parseObject(fsbuffer);
      JsonObject& json = jsonBuffer.parseObject(fsbuffer.get());
      json.printTo(Serial);
      if (json.success()) {
        Serial.println("\nparsed json");
        // const char* wifi = json["WiFi_SSID"];
        // const char* passwd = json["WiFi_PASSWORD"];
        strcpy(wifi_ssid, json["WiFi_SSID"]);
        strcpy(wifi_password, json["WiFi_PASSWORD"]);
        Serial.printf("SSID:%s\r\n", wifi_ssid);
        Serial.printf("PASSWORD:%s\r\n", wifi_password);
        WiFi.begin(wifi_ssid, wifi_password);
        return true;
      } else {
        Serial.println("failed to load json config");
        return false;
      }
    }
  } else {
    Serial.printf("file isn't exists.\r\n");
    writeFile(SD, "/wifi-config.json", "{\"WiFi_SSID\":\"xMasterHax_2.4G\",\"WiFi_PASSWORD\":\"wittyercheese551\",\"AzureDeviceID\":\"test001\",\"ConnectingString\":\"m5statck-abcde\"}");
  }
  return false;
}

boolean checkConnection() {
  int count = 0;
  Serial.print("Waiting for Wi-Fi connection");
  while ( count < 30 ) {
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println();
      Serial.println("Connected!");
      return (true);
    }
    delay(500);
    Serial.print(".");
    count++;
  }
  Serial.println("Timed out.");
  return false;
}

void startWebServer() {
  if (settingMode) {
    Serial.print("Starting Web Server at ");
    Serial.println(WiFi.softAPIP());
    webServer.on("/settings", []() {
      String s = "<h1>Wi-Fi Settings</h1><p>Please enter your password by selecting the SSID.</p>";
      s += "<form method=\"get\" action=\"setap\"><label>SSID: </label><select name=\"ssid\">";
      s += ssidList;
      s += "</select><br>Password: <input name=\"pass\" length=64 type=\"password\"><input type=\"submit\"></form>";
      webServer.send(200, "text/html", makePage("Wi-Fi Settings", s));
    });
    webServer.on("/setap", []() {
      // for (int i = 0; i < 96; ++i) {
      //   EEPROM.write(i, 0);
      // }
      String ssid = urlDecode(webServer.arg("ssid"));
      Serial.print("SSID: ");
      Serial.println(ssid);
      String pass = urlDecode(webServer.arg("pass"));
      Serial.print("Password: ");
      Serial.println(pass);
      Serial.println("Writing SSID to EEPROM...");

      //----------------file--------------------
      const char* filename = "/wifi-config.json";
      if (SD.exists(filename)) {
        //file exists, reading and loading
        Serial.println("reading config file");
        File configFile = SD.open(filename, "r");
        if (configFile) {
          Serial.println("opened config file");
          size_t size = configFile.size();
          // Allocate a buffer to store contents of the file.
          std::unique_ptr<char[]> fsbuffer(new char[size]);

          configFile.readBytes(fsbuffer.get(), size);
          // configFile.readBytes(fsbuffer, size);
          fsbuffer[size] = 0;

          DynamicJsonBuffer jsonBuffer;
          JsonObject& root = jsonBuffer.parseObject(fsbuffer.get());
          root[String("WiFi_SSID")] = ssid;
          root[String("WiFi_PASSWORD")] = pass;

          String output;
          root.printTo(output);
          Serial.printf("Save Json.\r\n");
          Serial.print(output);
          configFile.close();

          File configFile = SD.open(filename, "w");
          if (!configFile) {
            Serial.println("Failed to open file for writing");
            return;
          }

          if (configFile.print(output)) {
            Serial.println("File written");
          } else {
            Serial.println("Write failed");
          }
        }
      }
      //---------------------------------------

      for (int i = 0; i < ssid.length(); ++i) {
        // EEPROM.write(i, ssid[i]);
      }
      Serial.println("Writing Password to EEPROM...");
      for (int i = 0; i < pass.length(); ++i) {
        // EEPROM.write(32 + i, pass[i]);
      }
      //   EEPROM.commit();


      Serial.println("Write EEPROM done!");
      String s = "<h1>Setup complete.</h1><p>device will be connected to \"";
      s += ssid;
      s += "\" after the restart.";
      webServer.send(200, "text/html", makePage("Wi-Fi Settings", s));
      delay(3000);
      ESP.restart();
    });
    webServer.onNotFound([]() {
      String s = "<h1>AP mode</h1><p><a href=\"/settings\">Wi-Fi Settings</a></p>";
      webServer.send(200, "text/html", makePage("AP mode", s));
    });
  }
  else {
    Serial.print("Starting Web Server at ");
    Serial.println(WiFi.localIP());
    webServer.on("/", []() {
      String s = "<h1>STA mode</h1><p><a href=\"/reset\">Reset Wi-Fi Settings</a></p>";
      webServer.send(200, "text/html", makePage("STA mode", s));
    });
    webServer.on("/reset", []() {
      deleteFile(SD, "/wifi-config.json");
      String s = "<h1>Wi-Fi settings was reset.</h1><p>Please reset device.</p>";
      webServer.send(200, "text/html", makePage("Reset Wi-Fi Settings", s));
      delay(3000);
      ESP.restart();
    });
  }
  webServer.begin();
}

void setupMode() {
  WiFi.mode(WIFI_MODE_STA);
  WiFi.disconnect();
  delay(100);
  int n = WiFi.scanNetworks();
  delay(100);
  Serial.println("");
  for (int i = 0; i < n; ++i) {
    ssidList += "<option value=\"";
    ssidList += WiFi.SSID(i);
    ssidList += "\">";
    ssidList += WiFi.SSID(i);
    ssidList += "</option>";
  }
  delay(100);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
  WiFi.softAP(apSSID);
  WiFi.mode(WIFI_MODE_AP);
  // WiFi.softAPConfig(IPAddress local_ip, IPAddress gateway, IPAddress subnet);
  // WiFi.softAP(const char* ssid, const char* passphrase = NULL, int channel = 1, int ssid_hidden = 0);
  // dnsServer.start(53, "*", apIP);
  startWebServer();
  Serial.print("Starting Access Point at \"");
  Serial.print(apSSID);
  Serial.println("\"");
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