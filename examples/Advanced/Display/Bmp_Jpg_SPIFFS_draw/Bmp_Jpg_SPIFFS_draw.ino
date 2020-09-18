/* 
 *  You only need to format SPIFFS the first time you run a
   test or else use the SPIFFS plugin to create a partition
   https://github.com/me-no-dev/arduino-esp32fs-plugin 
 * put jpg or bmp file in (<project directory>/data)
 * Select Tools > ESP32 Sketch Data Upload menu item. This should start uploading the files into ESP32 flash file system.
*/

#include "FS.h"
#include "SPIFFS.h"
#include "M5Stack.h"

void setup(){
    M5.begin(true, false, false, false);
    M5.Power.begin();
    Serial.begin(115200);
    if(!SPIFFS.begin(true)){
        Serial.println("SPIFFS Mount Failed");
        return;
    }
    M5.Lcd.drawJpgFile(SPIFFS, "/m5_logo.jpg", 0, 0);
}

void loop(){
  M5.update();
  if(M5.BtnA.wasPressed()) {
    M5.Lcd.drawBmpFile(SPIFFS, "/pcd.bmp", 0, 0);
  }
  if(M5.BtnB.wasPressed()) {
    M5.Lcd.drawJpgFile(SPIFFS, "/m5_logo.jpg", 0, 0);
  }
}
