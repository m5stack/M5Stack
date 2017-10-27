/*
  This a simple example of the aREST Library for the ESP32 WiFi chip.
  This example illustrate the cloud part of aREST that makes the board accessible from anywhere
  See the README file for more details.

  Written in 2017 by Marco Schwartz under a GPL license.
  
*/

// Import required libraries
#include <M5Stack.h>
#include <WiFi.h>
// #include <WiFiMulti.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <Preferences.h>
#include "utility/aREST.h"
#include <ArduinoJson.h>


// Preferences
Preferences localData;
#define MAGIC_VAL 0x86
String ChipID;
M5Timer Timer;

struct DeviceData {
  uint8_t  type;
  char     token[20];
  char     name[10];
  uint64_t mac_addr;
  uint8_t  home_id;
  uint8_t  group_id;
  uint8_t  node_id;
};

// Clients
WiFiClient espClient;
PubSubClient client(espClient);

// Create aREST instance
aREST rest = aREST(client);

// Unique ID to identify the device for cloud.arest.io
char* device_id = "bsz0gyocz46mghr4";

// WiFi parameters
const char* ssid = "SmarTone";
const char* password ="SmarTone0315";

// Variables to be exposed to the API
int temperature;
int humidity;

// Declare functions to be exposed to the API
int ledControl(String command);

// Functions
void callback(char* topic, byte* payload, unsigned int length);

// Detect boot mode
enum {
  BOOT_FIRST_POWERON = 1>>1,
  BOOT_HOLD_A        = 2>>1,
  BOOT_SOFT_RESET    = 3>>1
};

uint8_t detect_boot_mode() {
  uint8_t return_val = 0;
  if(first_powerup()) {
    return_val |= BOOT_FIRST_POWERON;
  }

  pinMode(BUTTON_A_PIN, OUTPUT);
  if(!digitalRead(BUTTON_A_PIN)) {
    return_val |= BOOT_HOLD_A;
  }

  return return_val;
}

void setup()
{
  // Start
  M5.begin();
  M5.Lcd.setBrightness(50);

  // Get the chip id
  uint64_t chip_mac = ESP.getEfuseMac();
  ChipID = String((uint16_t)(chip_mac>>32), 16);
  ChipID += String((uint32_t)chip_mac, 16);
  // ChipID.toUpperCase();
  
  // Display the ID in the TFT
  M5.Lcd.fillScreen(TFT_BLACK);
  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
  M5.Lcd.drawCentreString(("ID: "+ChipID).c_str(), 320/2, 205, 4);
  Serial.print("ESP32 Chip ID:");
  Serial.println(ChipID);

  // preferences store
  localData.begin("M5Config");
  if(first_powerup() == true) {
    // write default config
    Serial.println("First Power up!");
  }

  // Set callback
  client.setCallback(callback);

  // Init variables and expose them to REST API
  temperature = 24;
  humidity = 40;
  rest.variable("temperature",&temperature);
  rest.variable("humidity",&humidity);

  // Function to be exposed
  rest.function("led",ledControl);
  rest.function("print",strPrint);

  // Give name & ID to the device (ID should be 6 characters long)
  // rest.set_id(device_id);
  rest.set_topic_io((char*)ChipID.c_str());
  rest.set_name("M5Cloud-Testman");

  // Connect to WiFi
  WiFi.onEvent(WiFiEvent);
  
  if(!digitalRead(BUTTON_A_PIN)) {
    //if press btnA power on, start the SmartConfig
    WiFi.mode(WIFI_AP_STA);
    WiFi.beginSmartConfig();

    //Wait for SmartConfig packet from mobile
    Serial.println("Waiting for SmartConfig.");
    while (!WiFi.smartConfigDone()) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("SmartConfig received.");
  } 

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
}

void loop() {
  
  // Connect to the cloud
  
  if(M5.BtnB.wasPressed()) {
    http_handle();
  }
  
  rest.handle(client);
  M5.update();
  Timer.run();
}

void http_handle() {
  HTTPClient http;
  
  Serial.print("[HTTP] begin...\n");
  // configure traged server and url
  //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
  http.begin("http://172.16.17.85:7788/device/tempcode"); //HTTP

  Serial.print("[HTTP] POST...\n");
  // start connection and send HTTP header
  String postbuf = String("mac_id="+ChipID);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  Serial.println("HTTP POST Body:");
  Serial.println(postbuf);
  int httpCode = http.POST(postbuf);

  // httpCode will be negative on error
  if(httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if(httpCode == HTTP_CODE_OK) {
          String payload = http.getString();
          Serial.println(payload);

          // Allocate a Json temporary memory pool
          DynamicJsonBuffer jsonBuffer(32);
          JsonObject& root = jsonBuffer.parseObject(payload);
          
          if (!root.success()) {
            Serial.println("JSON parsing failed!");
            M5.Lcd.drawCentreString("JSON parsing failed!", 320/2, 80, 4);
            // return false;
          } else {
            // char regcode[5];
            // memcpy(regcode, root["data"], 5);
            const char* sensor = root["data"];
        
            M5.Lcd.drawCentreString("Check Code", 160, 30, 4);
            M5.Lcd.drawCentreString(sensor, 320/2, 80, 8);
          }

      }
  } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
}


// Handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {

  rest.handle_callback(client, topic, payload, length);

}

// Custom function accessible by the API
int ledControl(String command) {

  // Get state from command
  int state = command.toInt();

  pinMode(32,OUTPUT);
  digitalWrite(32,state);
  return 1;
}

int strPrint(String command) {

  // // Get state from command
  command.remove(command.length()-1);
  command.remove(command.length()-1);

  Serial.println(command);
  M5.Lcd.fillScreen(0x0000);
  M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.setTextFont(2);
  M5.Lcd.print(command);
  return 66;
}

bool first_powerup() {
  // #define MAGIC_VAL 0x86
  if(localData.getUChar("first_mark") == MAGIC_VAL) {
    return false;
  } else {
    localData.putUChar("first_mark", MAGIC_VAL);
    return true;
  }
}

void WiFiEvent(WiFiEvent_t event)
{
    Serial.printf("[WiFi-event] event: %d\n", event);

    switch(event) {
    case SYSTEM_EVENT_STA_GOT_IP:
        Serial.println("WiFi connected");
        M5.Lcd.drawCentreString("WiFi connected", 320/2, 110, 4);
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        M5.Lcd.drawCentreString("IP: "+WiFi.localIP().toString(), 320/2, 180, 2);
        randomSeed(micros());
        // mqtt_connect();
        break;
        
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("WiFi lost connection");
        WiFi.begin();
        break;
    }
}


// bool readReponseContent(struct UserData* userData) {
//   // Compute optimal size of the JSON buffer according to what we need to parse.
//   // See https://bblanchon.github.io/ArduinoJson/assistant/
//   const size_t BUFFER_SIZE =
//       JSON_OBJECT_SIZE(8)    // the root object has 8 elements
//       + JSON_OBJECT_SIZE(5)  // the "address" object has 5 elements
//       + JSON_OBJECT_SIZE(2)  // the "geo" object has 2 elements
//       + JSON_OBJECT_SIZE(3)  // the "company" object has 3 elements
//       + MAX_CONTENT_SIZE;    // additional space for strings

//   // Allocate a temporary memory pool
//   DynamicJsonBuffer jsonBuffer(BUFFER_SIZE);

//   JsonObject& root = jsonBuffer.parseObject(client);

//   if (!root.success()) {
//     Serial.println("JSON parsing failed!");
//     return false;
//   }

//   // Here were copy the strings we're interested in
//   strcpy(userData->name, root["name"]);
//   strcpy(userData->company, root["company"]["name"]);
//   // It's not mandatory to make a copy, you could just use the pointers
//   // Since, they are pointing inside the "content" buffer, so you need to make
//   // sure it's still in memory when you read the string

//   return true;
// }

// void http_handle() {
//   HTTPClient http;
  
//   Serial.print("[HTTP] begin...\n");
//   // configure traged server and url
//   //http.begin("https://www.howsmyssl.com/a/check", ca); //HTTPS
//   http.begin("http://example.com/index.html"); //HTTP

//   Serial.print("[HTTP] GET...\n");
//   // start connection and send HTTP header
//   int httpCode = http.GET();

//   // httpCode will be negative on error
//   if(httpCode > 0) {
//       // HTTP header has been send and Server response header has been handled
//       Serial.printf("[HTTP] GET... code: %d\n", httpCode);

//       // file found at server
//       if(httpCode == HTTP_CODE_OK) {
//           String payload = http.getString();
//           Serial.println(payload);
//       }
//   } else {
//       Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
//   }

//   http.end();
// }

/*
const char* ca = \ 
"-----BEGIN CERTIFICATE-----\n" \  
"MIIEkjCCA3qgAwIBAgIQCgFBQgAAAVOFc2oLheynCDANBgkqhkiG9w0BAQsFADA/\n" \  
"MSQwIgYDVQQKExtEaWdpdGFsIFNpZ25hdHVyZSBUcnVzdCBDby4xFzAVBgNVBAMT\n" \  
"DkRTVCBSb290IENBIFgzMB4XDTE2MDMxNzE2NDA0NloXDTIxMDMxNzE2NDA0Nlow\n" \  
"SjELMAkGA1UEBhMCVVMxFjAUBgNVBAoTDUxldCdzIEVuY3J5cHQxIzAhBgNVBAMT\n" \  
"GkxldCdzIEVuY3J5cHQgQXV0aG9yaXR5IFgzMIIBIjANBgkqhkiG9w0BAQEFAAOC\n" \  
"AQ8AMIIBCgKCAQEAnNMM8FrlLke3cl03g7NoYzDq1zUmGSXhvb418XCSL7e4S0EF\n" \  
"q6meNQhY7LEqxGiHC6PjdeTm86dicbp5gWAf15Gan/PQeGdxyGkOlZHP/uaZ6WA8\n" \  
"SMx+yk13EiSdRxta67nsHjcAHJyse6cF6s5K671B5TaYucv9bTyWaN8jKkKQDIZ0\n" \  
"Z8h/pZq4UmEUEz9l6YKHy9v6Dlb2honzhT+Xhq+w3Brvaw2VFn3EK6BlspkENnWA\n" \  
"a6xK8xuQSXgvopZPKiAlKQTGdMDQMc2PMTiVFrqoM7hD8bEfwzB/onkxEz0tNvjj\n" \  
"/PIzark5McWvxI0NHWQWM6r6hCm21AvA2H3DkwIDAQABo4IBfTCCAXkwEgYDVR0T\n" \  
"AQH/BAgwBgEB/wIBADAOBgNVHQ8BAf8EBAMCAYYwfwYIKwYBBQUHAQEEczBxMDIG\n" \  
"CCsGAQUFBzABhiZodHRwOi8vaXNyZy50cnVzdGlkLm9jc3AuaWRlbnRydXN0LmNv\n" \  
"bTA7BggrBgEFBQcwAoYvaHR0cDovL2FwcHMuaWRlbnRydXN0LmNvbS9yb290cy9k\n" \  
"c3Ryb290Y2F4My5wN2MwHwYDVR0jBBgwFoAUxKexpHsscfrb4UuQdf/EFWCFiRAw\n" \  
"VAYDVR0gBE0wSzAIBgZngQwBAgEwPwYLKwYBBAGC3xMBAQEwMDAuBggrBgEFBQcC\n" \  
"ARYiaHR0cDovL2Nwcy5yb290LXgxLmxldHNlbmNyeXB0Lm9yZzA8BgNVHR8ENTAz\n" \  
"MDGgL6AthitodHRwOi8vY3JsLmlkZW50cnVzdC5jb20vRFNUUk9PVENBWDNDUkwu\n" \  
"Y3JsMB0GA1UdDgQWBBSoSmpjBH3duubRObemRWXv86jsoTANBgkqhkiG9w0BAQsF\n" \  
"AAOCAQEA3TPXEfNjWDjdGBX7CVW+dla5cEilaUcne8IkCJLxWh9KEik3JHRRHGJo\n" \  
"uM2VcGfl96S8TihRzZvoroed6ti6WqEBmtzw3Wodatg+VyOeph4EYpr/1wXKtx8/\n" \  
"wApIvJSwtmVi4MFU5aMqrSDE6ea73Mj2tcMyo5jMd6jmeWUHK8so/joWUoHOUgwu\n" \  
"X4Po1QYz+3dszkDqMp4fklxBwXRsW10KXzPMTZ+sOPAveyxindmjkW8lGy+QsRlG\n" \  
"PfZ+G6Z6h7mjem0Y+iWlkYcV4PIWL1iwBi8saCbGS5jN2p8M+X+Q7UNKEkROb3N6\n" \  
"KOqkqm57TH2H3eDJAkSnh6/DNFu0Qg==\n" \  
"-----END CERTIFICATE-----\n";
*/