/*
  This a simple example of the aREST Library for the ESP32 WiFi chip.
  This example illustrate the cloud part of aREST that makes the board accessible from anywhere
  See the README file for more details.

  Written in 2017 by Marco Schwartz under a GPL license.
  
*/

// Import required libraries
#include <M5Stack.h>
#include <WiFi.h>
#include <WiFiMulti.h>
#include <HTTPClient.h>
#include <PubSubClient.h>
#include <Preferences.h>
#include <ArduinoJson.h>
#include "utility/aREST.h"
#include "utility/DHT12.h"
#include "bmp_mapx.h"

// Preferences
Preferences localData;
#define MAGIC_VAL 0x86
String ChipID;
M5Timer Timer;
DHT12 dht12; //Preset scale CELSIUS and ID 0x5c.
String temperature;
String humidity;

int direction = 0;
int current_floor = 1;
int dest_floor = 1;
#define LIFT_UP     1
#define LIFT_DOWN   2
#define LIFT_STAND  0

// Clients
WiFiClient espClient;
PubSubClient client(espClient);


// Unique ID to identify the device for cloud.arest.io
char* device_id = "bsz0gyocz46mghr4";

// WiFi parameters
const char* ssid = "SmarTone";
const char* password ="SmarTone0315";
const char* mqtt_server = "52.163.121.143";


// Functions
// Handles message arrived on subscribed topic(s)
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the light if an 1 was received as first character
  // if((String)topic == "/smartone/actuators/light/1") {
  //   light_demo2(1, payload[0]);

  // } else if((String)topic == "/smartone/actuators/light/2") {
  //   light_demo2(2, payload[0]);

  // } else if((String)topic == "/smartone/actuators/light/3") {
  //   light_demo2(3, payload[0]);
  // } 
  
  // if((String)topic == "/smartone/lift/call/A") {
  //   call_lift(String(payload[0]).toInt());
  // }

  if((String)topic == "/smartone/lcd/display") {
    // Allocate a Json temporary memory pool
    DynamicJsonBuffer jsonBuffer(32);
    JsonObject& root = jsonBuffer.parseObject(payload);
    
    if (!root.success()) {
      Serial.println("JSON parsing failed!");
      // return false;
    } else {
      // char floorx[5];
      // char directionx[5];
      // memcpy(floorx, root["floor"], 3);
      // memcpy(directionx, root["direction"], 2);
      // const char* floor = root["floor"];
      // root["sensor"].as<String>()
      String floorx(root["floor"].as<String>());
      String directionx(root["direction"].as<String>());

      Serial.println(floorx);
      Serial.println(directionx);
      lift_floor_display(floorx.toInt(), directionx.toInt());
      // lift_floor_display(String(floorx).toInt(), String(directionx).toInt());
    }
  }
}

void light_demo2(uint8_t light_id, char sw_char) {
  bool sw = (sw_char == '1')?1:0;
  // M5.Lcd.drawBitmap(0, 0, 320, 240, (uint16_t*)gImage_Light_BK);
  #define Y_OFFSET 40

  switch (light_id) {
    case 1: 
      if(sw) M5.Lcd.drawBitmap(1, Y_OFFSET, 100, 100, (uint16_t*)gImage_Light_On);
      else   M5.Lcd.drawBitmap(1, Y_OFFSET, 100, 100, (uint16_t*)gImage_Light_Off);
      break;
    
    case 2: 
      if(sw) M5.Lcd.drawBitmap(110, Y_OFFSET, 100, 100, (uint16_t*)gImage_Light_On);
      else   M5.Lcd.drawBitmap(110, Y_OFFSET, 100, 100, (uint16_t*)gImage_Light_Off);
      break;

    case 3: 
      if(sw) M5.Lcd.drawBitmap(220, Y_OFFSET, 100, 100, (uint16_t*)gImage_Light_On);
      else   M5.Lcd.drawBitmap(220, Y_OFFSET, 100, 100, (uint16_t*)gImage_Light_Off);
      break;
  }
}

void light_demo(uint8_t light_id, char sw_char) {
  #define R_SIZE    30
  #define Y_OFFSET  105  
  bool sw = (sw_char == '1')?1:0;
  
  M5.Lcd.setTextColor(TFT_YELLOW);

  switch (light_id) {
    case 1: 
      M5.Lcd.fillCircle(320/4-10,   Y_OFFSET, R_SIZE, RED);
      if(sw) M5.Lcd.fillCircle(320/4-10,   Y_OFFSET, R_SIZE, RED);
      else   M5.Lcd.fillCircle(320/4-10,   Y_OFFSET, R_SIZE-5, BLACK);
      M5.Lcd.drawCentreString("1", 320/4-10, Y_OFFSET-8, 4);
      break;
    
    case 2: 
      M5.Lcd.fillCircle(320/4*2,    Y_OFFSET, R_SIZE, RED);
      if(sw) M5.Lcd.fillCircle(320/4*2,   Y_OFFSET, R_SIZE, RED);
      else   M5.Lcd.fillCircle(320/4*2,   Y_OFFSET, R_SIZE-5, BLACK);
      M5.Lcd.drawCentreString("2", 320/4*2, Y_OFFSET-8, 4);
      break;

    case 3: 
      M5.Lcd.fillCircle(320/4*3+10, Y_OFFSET, R_SIZE, RED);
      if(sw) M5.Lcd.fillCircle(320/4*3+10,   Y_OFFSET, R_SIZE, RED);
      else   M5.Lcd.fillCircle(320/4*3+10,   Y_OFFSET, R_SIZE-5, BLACK);
      M5.Lcd.drawCentreString("3", 320/4*3+10, Y_OFFSET-8, 4);
      break;
  }
}

void lift_update() {
  
  if(current_floor < dest_floor) {
    current_floor++;
    direction = 1;
  } else if(current_floor > dest_floor) {
    direction = 2;
    current_floor--;
  } else if(current_floor == dest_floor) {
    direction = 0;
  }

  char send_buffer[30];
  sprintf(send_buffer, "{\"floor\":\"%s\", \"direction\":\"%s\"}", String(current_floor).c_str(), String(direction).c_str());
  client.publish("/smartone/lift/A", send_buffer);
}

void call_lift(int dest) {
  dest_floor = dest;
}

void update_sensor() {
  String temperature = String(dht12.readTemperature());
  String humidity = String(dht12.readHumidity());
  Serial.printf("temperature:%s, humidity:%s\r\n", temperature.c_str(), humidity.c_str());
  client.publish("/smartone/sensors/temperature/1", temperature.c_str());
  client.publish("/smartone/sensors/humidity/1", humidity.c_str());

  update_display(current_floor, 1, temperature.toInt(), humidity.toInt());
}

void update_display(int floorx, int light, int tmp, int hum) {
  M5.Lcd.setTextColor(BLACK, WHITE);
  M5.Lcd.setTextFont(4);
  // M5.Lcd.setCursor(150, 80);
  // M5.Lcd.print(floorx);
  char strbuff[20];
  sprintf(strbuff, "% 2d", floorx);
  String floor_str(floorx);
  String light_str(light);
  String tmp_str(tmp);
  String hum_str(hum);
  
  M5.Lcd.drawCentreString(tmp_str.c_str(), 156, 80, 4);
  M5.Lcd.drawCentreString(hum_str.c_str(), 255, 80, 4);
  // M5.Lcd.drawCentreString(light_str, 63, 160, 4);
  if(light == 1)
    M5.Lcd.drawCentreString(" ON ", 63, 160, 4);
  else 
    M5.Lcd.drawCentreString("OFF", 63, 160, 4);
  M5.Lcd.drawCentreString(strbuff, 156, 160, 4);
}

void lift_floor_display(int floorx, int dict) {

  if(dict == 0) {
    M5.Lcd.drawBitmap(220, 25, 100, 216, (uint16_t*)gImage_ELEVATOR_UPDOWN);

  } else if(dict == 1) {
    M5.Lcd.drawBitmap(220, 25, 100, 216, (uint16_t*)gImage_ELEVATOR_UP);

  } else if(dict == 2) {
    M5.Lcd.drawBitmap(220, 25, 100, 216, (uint16_t*)gImage_ELEVATOR_DOWN);
  }

  M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
  char floor_str[9];
  sprintf(floor_str, "%02d", floorx);
  M5.Lcd.drawCentreString(floor_str, 110, 100, 8);
}

void setup()
{
  // Start
  M5.begin();
  Wire.begin();

  // Get the chip id
  uint64_t chip_mac = ESP.getEfuseMac();
  ChipID = String((uint16_t)(chip_mac>>32), 16);
  ChipID += String((uint32_t)chip_mac, 16);
  // ChipID.toUpperCase();
  
  M5.Lcd.drawBitmap(0, 0, 320, 240, (uint16_t*)gImage_ELEVATOR_BK);
  lift_floor_display(27, 1);
  // M5.Lcd.drawBitmap(0, 0, 320, 240, (uint16_t*)gImage_SmarTone_BK);
  // M5.Lcd.drawBitmap(0, 0, 320, 240, (uint16_t*)gImage_Light_BK);
  
  // Display the ID in the TFT
  // M5.Lcd.fillScreen(TFT_BLACK);
  // M5.Lcd.setTextColor(TFT_YELLOW, TFT_BLACK);
  // M5.Lcd.drawCentreString(("ID: "+ChipID).c_str(), 320/2, 205, 4);
  // Serial.print("ESP32 Chip ID:");
  

  // preferences store
  localData.begin("M5Config");
  if(first_powerup() == true) {
    // write default config
    Serial.println("First Power up!");
  }

  // Set callback
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // Connect to WiFi
  WiFi.onEvent(WiFiEvent);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  WiFi.setAutoConnect(true);
  WiFi.setAutoReconnect(true);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  
  // Timer.setInterval(1000, update_sensor);
  // Timer.setInterval(1010, lift_update);
}

void loop() {
  
  // Connect to the cloud
  
  if(M5.BtnA.wasPressed()) {
    static bool sw1;
    sw1 = !sw1;
    // light_demo(1, sw1);
    client.publish("/smartone/buttons/device/1", String(sw1).c_str());
  }

  if(M5.BtnB.wasPressed()) {
    static bool sw2;
    sw2 = !sw2;
    // light_demo(2, sw2);
    client.publish("/smartone/buttons/device/2", String(sw2).c_str());
  }

  if(M5.BtnC.wasPressed()) {
    static bool sw3;
    sw3 = !sw3;
    // light_demo(3, sw3);
    client.publish("/smartone/buttons/device/3", String(sw3).c_str());
  }

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  
  M5.update();
  Timer.run();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP32Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("outTopic", "hello world");
      // ... and resubscribe
      client.subscribe("inTopic");
      // client.subscribe("/smartone/actuators/light/1");
      // client.subscribe("/smartone/actuators/light/2");
      // client.subscribe("/smartone/actuators/light/3");

      client.subscribe("/smartone/lift/call/A");
      client.subscribe("/smartone/lcd/display");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(2000);
    }
  }
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
        // M5.Lcd.drawCentreString("WiFi connected", 320/2, 110, 4);
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
        // M5.Lcd.drawCentreString("IP: "+WiFi.localIP().toString(), 320/2, 180, 2);
        randomSeed(micros());
        // mqtt_connect();
        break;
        
    case SYSTEM_EVENT_STA_DISCONNECTED:
        Serial.println("WiFi lost connection");
        WiFi.begin(ssid, password);
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