/*
  This sketch is based on the sdFatTftBitmap example sketch from the ILI9341_due library.

  See: http://marekburiak.github.io/ILI9341_due/
  
  Adapted by Bodmer 20th January 2017 to incorporate the JPEGDecoder library and draw
  Jpeg images that have been stored on an SD card.

  It is compatible with the Mega and Due boards, modify the defined CS, DC pins
  for the TFT and SD connections to suit your setup.

  You can generate your own Jpeg images from digital photographs by cropping and resizing
  by using commonly available picture/image editors such as Paint or IrfanView.
  The example image used by the sketch is in the extras folder in the JPEGDecoder library,
  copy the "arduino.jpg" file to the SD card.

  See: https://github.com/Bodmer/JPEGDecoder

  The Arduino IDE's built in SD library is used:

  https://www.arduino.cc/en/reference/SD
*/

//  IMPORTANT: Edit the ILI9341_due_config.h to select:
//    #define ILI9341_SPI_MODE_NORMAL  // uses SPI library
//  this sketch will not work if the Due DMA or SPI extended mode is enabled.
//  Use the hardware SPI lines MOSI, MISO and SCK to interface with both the TFT and
//  the SD card.

//====================================================================================
//  libraries
//====================================================================================

#include <SPI.h>
#include <SD.h>  // Use the Arduino IDE built-in SD library

#include "Adafruit_ILI9341.h"
#include <JPEGDecoder.h>  // JPEG decoder library
#include <WiFiMulti.h>
#include <HTTPClient.h>

#define USE_SERIAL Serial

WiFiMulti wifiMulti;

///////////////////
#include <M5Stack.h>
#include <stdio.h>
#include <string.h>
#include "tjpgd.h"

//=====================PTC08=======================
HardwareSerial PTC08(2);

// the setup routine runs once when M5Stack starts up
// 复位指令:56 00 26 00 返回:76 00 26 00 +DSP 版 信息
const uint8_t ptc_reset[] = {0x56, 0x00, 0x26, 0x00};

// 拍照指令:56 00 36 01 00 返回:76 00 36 00 00
const uint8_t ptc_kaca[]  = {0x56, 0x00, 0x36, 0x01, 0x00};

// 清空图片缓存指令:56 00 36 01 02 返回:76 00 36 00 00
const uint8_t ptc_clean_cache[]  = {0x56, 0x00, 0x36, 0x01, 0x02};

// 读所拍图片长度指令:56 00 34 01 00 返回:76 00 34 00 04 00 00 XX YY
// XX YY -------图片数据长度，XX为高位字节，YY为低位字节
const uint8_t ptc_read_length[] = {0x56, 0x00, 0x34, 0x01, 0x00};

// 读取所拍图片数据指令:56 00 32 0C 00 0A 00 00 XX(8) XX 00 00 YY(12) YY 00 FF
// 返回:7600320000 FFD8 。FFD9 7600320000
uint8_t ptc_read_data[16] = {0x56, 0x00, 0x32, 0x0c, 0x00, 0x0a, 0x00, 0x00, 0x11, 0x11, 0x00, 0x00, 0x11, 0x11, 0x00, 0xff};

// 设置拍照图片压缩率指令:56 00 31 05 01 01 12 04 XX(8) 返回:76 00 31 00 00
// XX 一般选 36 (范围:00 ----FF)
uint8_t ptc_set_compress[] = {0x56, 0x00, 0x31, 0x05, 0x01, 0x01, 0x12, 0x04, 0x00};

// 修改串口初始波特率指令: 56 00 31 06 04 02 00 08 XX(8) YY
const uint8_t ptc_set_baud[] = {0x56, 0x00, 0x31, 0x06, 0x04, 0x02, 0x00, 0x08, 0x0d, 0xa6};

void camera_set_baud() {
  PTC08.write(ptc_set_baud, 10);
  delay(100);
  while(PTC08.available()) {
    Serial.printf("%2.2x ", PTC08.read());
  }
  Serial.printf("\r\n");
}

void camera_reset() {
  Serial.printf("PTC reset.\r\n");
  delay(1000);
  PTC08.write(ptc_clean_cache, 5);
  delay(100);
  while(PTC08.available()) {
    Serial.printf("%2.2x ", PTC08.read());
  }
  Serial.printf("\r\n");
}

void camera_clean_cache() {
  Serial.printf("PTC clean cache.\r\n");
  PTC08.write(ptc_clean_cache, 5);
  delay(100);
  while(PTC08.available()) {
    Serial.printf("%2.2x ", PTC08.read());
  }
  Serial.printf("\r\n");
}

void camera_shutter() {
  Serial.printf("PTC shutter.\r\n");
  PTC08.write(ptc_kaca, 5);
  delay(100);
  while(PTC08.available()) {
    Serial.printf("%2.2x ", PTC08.read());
  }
  Serial.printf("\r\n");
}

uint16_t camera_get_length() {
  uint8_t buffer[10];
  uint8_t idx = 0;
  Serial.printf("PTC get length.\r\n");
  PTC08.write(ptc_read_length, 5);
  delay(100);
  while(PTC08.available()) {
    buffer[idx] = PTC08.read();
    // Serial.write(buffer[idx]);
    // Serial.printf("%2.2x ", buffer[idx]);
    // Serial.write(PTC08.read());
    idx++;
  }
  return (buffer[7]<<8 | buffer[8]);
}

void camera_read_data(uint16_t addr, uint16_t length, uint8_t* dest) {
  int idx = 0;
  ptc_read_data[9] = (uint8_t)(addr&0x00ff);
  ptc_read_data[8] = (uint8_t)(addr>>8);
  ptc_read_data[13] = (uint8_t)(length&0x00ff);
  ptc_read_data[12] = (uint8_t)(length>>8);

  PTC08.write(ptc_read_data, 16);
  delay(100);
  while(PTC08.available()) {
    int ch = PTC08.read();
    // Serial.printf("%2.2x ", ch);
    if(++idx >= 6) {
      dest[idx-6] = ch;
    }
  }
}

void camera_set_compress(uint8_t val) {
  Serial.printf("PTC set compress.\r\n");
  ptc_set_compress[8] = val;
  PTC08.write(ptc_set_compress, 9);
  delay(100);
  while(PTC08.available()) {
    int ch = PTC08.read();
    Serial.printf("%2.2x ", ch);
  }
}
//------------------------------------------------------


//====================================================================================
//  defines
//====================================================================================
// For the Adafruit shield, these are the default.
#define TFT_DC    27
#define TFT_CS    14
#define TFT_MOSI  23
#define TFT_CLK   18
#define TFT_RST   2
#define TFT_MISO  0

#define LED_PIN 12
#define SD_CS 4		// Chip Select for SD card

#define TFT_BLACK 0x0000
#define TFT_WHITE 0xFFFF


const uint8_t bmp_header[] = {
0x42,0x4d,0x36,0x84,0x03,0x00,0x00,0x00,0x00,0x00,0x36,0x00,0x00,0x00,0x28, \
0x00,0x00,0x00,0x40,0x01,0x00,0x00,0xf0,0x00,0x00,0x00,0x01,0x00,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x84, \
0x03,0x00,0xc4,0x0e,0x00,0x00,0xc4,0x0e,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};

void take_picture() {
  camera_clean_cache();
  camera_shutter();

  int pic_length = camera_get_length();
  Serial.printf("picture length:%d\r\n", pic_length);
  delay(80);

  int count = 0;
  uint8_t get_buf[512];
  File file = SD.open("/camera.jpg", FILE_WRITE);
  Serial.printf("PTC read data.\r\n");
  for(int i=0; i<pic_length; i+=128) {
    if(pic_length - i > 128) {
      camera_read_data(i, 128, get_buf);
      file.write(get_buf, 128);
    } else {
      camera_read_data(i, pic_length-i, get_buf);
      file.write(get_buf, pic_length-i);
    }
  }
  file.close();
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels){
    Serial.printf("Listing directory: %s\n", dirname);

    File root = fs.open(dirname);
    if(!root){
        Serial.println("Failed to open directory");
        return;
    }
    if(!root.isDirectory()){
        Serial.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while(file){
        if(file.isDirectory()){
            Serial.print("  DIR : ");
            Serial.println(file.name());
            if(levels){
                listDir(fs, file.name(), levels -1);
            }
        } else {
            Serial.print("  FILE: ");
            Serial.print(file.name());
            Serial.print("  SIZE: ");
            Serial.println(file.size());
        }
        file = root.openNextFile();
    }
}
//====================================================================================
//  setup
//====================================================================================
// ILI9341_due tft = ILI9341_due(TFT_CS, TFT_DC, TFT_RST);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

void setup() {
  Serial.begin(115200);
  PTC08.begin(115200);

  // Make sure the SD card chip select is high before we initialise the TFT
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  // Initialise the TFT
  tft.begin();
  tft.fillScreen(TFT_BLACK);
  // tft.drawRGBBitmap(0, 0, uint16_t *pcolors, int16_t w, int16_t h);

  Serial.print(F("Initialising SD card..."));

  if (!SD.begin(SD_CS)) {
    Serial.println(F("failed!"));
    return;
  }
  Serial.println(F("OK!"));

  listDir(SD, "/", 0);

//------------------------
  wifiMulti.addAP("MasterHax_2.4G", "wittyercheese551");

  // File fsFile = SD.open("/camera2.bmp", FILE_READ);
  // uint8_t buffer[512];
  // fsFile.read(buffer, 54);
  // Serial.printf("\r\n\r\n\r\n\r\n");
  // for(int i=0; i<54; i++) {
  //   Serial.printf("0x%2.2x,", buffer[i]);
  // }
  // Serial.printf("\r\n\r\n\r\n\r\n");

  // for(int j=0; j<240; j++) {
  //   fsFile.read(buffer, 320);
  //   uint16_t *pt16 = (uint16_t*)buffer;
  //   for(int k=0; k<160; k++) {
  //     Serial.printf("%4.4x ", pt16[k]);
  //   }
  //   Serial.printf("\r\n");
  // }

  // tft.drawBitmap(0, 0, 320, 240, fsFile.flashRead());
    Serial.printf("reset.\r\n");
    camera_reset();
    delay(2500);
    
    while(PTC08.available()) {
      Serial.printf("%2.2x ", PTC08.read());
    }
    Serial.printf("\r\n");
}


//====================================================================================
//  loop
//====================================================================================

void loop() {
  // draw Arduino logo at a random position
  // drawJpeg( "/arduino.jpg", random(tft.width() - 160), random(tft.height() - 128) );
  take_picture();
  drawJpeg("/camera.jpg", 0, 0);

  File jpegFile = SD.open("/camera.jpg", FILE_READ);
  // // File jpegFile = SD.open("/pic.jpg");
  // // File jpegFile = SD.open("/EagleEye.jpg", FILE_READ);

    // wait for WiFi connection
  if((wifiMulti.run() == WL_CONNECTED)) {
      HTTPClient http;
      USE_SERIAL.print("[HTTP] begin...\n");

      // configure server and url
      // http.begin("http://api.heclouds.com/bindata?device_id=5811741&datastream_id=pic");
      http.begin("http://api.yeelink.net/v1.0/device/18607/sensor/407330/photos");
      // http.addHeader("api-key", "3siYnijpqzOtLKaqdGFIvj5PcM4=");
      http.addHeader("U-ApiKey", "e92dea44e2c4b29838fea13b9cf7ab59");
      http.sendRequest("POST", &jpegFile, jpegFile.size());
      USE_SERIAL.print("[HTTP] POST...\n");
      http.end();
  }
}

