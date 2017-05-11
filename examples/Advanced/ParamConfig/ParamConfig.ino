#include <M5Stack.h>
#include <ArduinoJson.h>

bool getParamFromTFCard(const char* filename, const char* keyword, char* strout) {
  // fs file read
  File fsFile;
  uint8_t fsbuffer[255];
  // const char* filename = "/azure-config.json";
  if(SD.exists(filename)) {
    fsFile = SD.open(filename, "r");
    if(fsFile) {
      Serial.printf("Success open file:\r\n");
      fsFile.read(fsbuffer, fsFile.size());
      Serial.printf("\r\n");
      fsbuffer[fsFile.size()] = 0;
      Serial.printf("%s", fsbuffer);
    } else {
      Serial.printf("Fail open file!\r\n");
      return false;
    }
  }

  // Memory pool for JSON object tree.
  //
  // Inside the brackets, 200 is the size of the pool in bytes,
  // If the JSON object is more complex, you need to increase that value.
  StaticJsonBuffer<200> jsonBuffer;

  // StaticJsonBuffer allocates memory on the stack, it can be
  // replaced by DynamicJsonBuffer which allocates in the heap.
  // It's simpler but less efficient.
  //
  // DynamicJsonBuffer  jsonBuffer;

  // Root of the object tree.
  //
  // It's a reference to the JsonObject, the actual bytes are inside the
  // JsonBuffer with all the other nodes of the object tree.
  // Memory is freed when jsonBuffer goes out of scope.
  JsonObject& root = jsonBuffer.parseObject(fsbuffer);

  // Test if parsing succeeds.
  if (!root.success()) {
    Serial.println("parseObject() failed");
    return false;
  }

  // Fetch values.
  //
  // Most of the time, you can rely on the implicit casts.
  // In other case, you can do root["time"].as<long>();
  const char* value = root[keyword];

  // Copy values.
  strcpy((char*)strout, value);
  Serial.printf("Keyword:%s\r\nValue:%s\r\n", keyword, strout);
  return true;
}

// the setup routine runs once when M5Stack starts up
void setup(){

  // initialize the M5Stack object
  m5.begin();
  
  // lcd display
  m5.lcd.printf("JSON test");

  char ssid[30];
  if(getParamFromTFCard("/azure-config.json", "WiFi_SSID", ssid)) {
    Serial.printf("Read Value:%s", ssid);
  }

}

// the loop routine runs over and over again forever
void loop(){

  m5.loop();
}

