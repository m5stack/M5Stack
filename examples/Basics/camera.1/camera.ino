#include <M5Stack.h>

HardwareSerial Serial2(2);

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

void camera_reset() {
  delay(2500);
  Serial2.write(ptc_clean_cache, 5);
}

void camera_clean_cache() {
  Serial2.write(ptc_clean_cache, 5);
}

void camera_shutter() {
  Serial2.write(ptc_kaca, 5);
}

uint16_t camera_get_length() {
  uint8_t buffer[10];
  uint8_t idx = 0;
  Serial2.write(ptc_read_length, 5);
  delay(100);
  while(Serial2.available()) {
    buffer[idx] = Serial2.read();
    // Serial.write(buffer[idx]);
    // Serial.printf("%2.2x ", buffer[idx]);
    // Serial.write(Serial2.read());
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

  Serial2.write(ptc_read_data, 16);
  delay(100);
  while(Serial2.available()) {
    int ch = Serial2.read();
    // Serial.printf("%2.2x ", ch);
    if(++idx >= 6) {
      dest[idx-6] = ch;
    }
  }
}

uint8_t get_buf[512];
void setup() {
  // initialize the M5Stack object
  // m5.begin();
  if(!SD.begin(4)){
      Serial.println("Card Mount Failed");
      return;
  }

  Serial.begin(115200);
  Serial2.begin(38400);

  // lcd display
  // m5.lcd.printf("hello world");
  // Serial.printf("hello \r\n");
  Serial.printf("\r\n\r\n\r\n");

  // delay(2500);
  Serial.printf("reset.\r\n");
  // Serial2.write(ptc_reset, 4);
  camera_reset();
  delay(100);
  while(Serial2.available()) {
  // Serial.write(Serial2.read());
    Serial.printf("%2.2x ", Serial2.read());
  }
  Serial.printf("\r\n");
  // while(Serial2.available()) {
  //   Serial.write(Serial2.read());
  // }

  delay(100);
  Serial.printf("clean cache.\r\n");
  // 清空图片缓存指令:56 00 36 01 02 返回:76 00 36 00 00
  // Serial2.write(ptc_clean_cache, 5);
  camera_clean_cache();
  delay(100);
  while(Serial2.available()) {
    // Serial.write(Serial2.read());
    Serial.printf("%2.2x ", Serial2.read());
  }
  Serial.printf("\r\n");


  delay(100);
  Serial.printf("kaca.\r\n");
  // 拍照指令:56 00 36 01 00 返回:76 00 36 00 00
  // Serial2.write(ptc_kaca, 5);
  camera_shutter();
  delay(100);
  while(Serial2.available()) {
    // Serial.write(Serial2.read());
    Serial.printf("%2.2x ", Serial2.read());
  }
  Serial.printf("\r\n");

  Serial.printf("get length.\r\n");
  // 读所拍图片长度指令:56 00 34 01 00 返回:76 00 34 00 04 00 00 XX YY
  // XX YY -------图片数据长度，XX为高位字节，YY为低位字节
  delay(1000);
  // Serial2.write(ptc_read_length, 5);
  int pic_length = camera_get_length();
  Serial.printf("picture length:%d\r\n", pic_length);
  // delay(100);
  // while(Serial2.available()) {
  //   // Serial.write(Serial2.read());
  //   Serial.printf("%2.2x ", Serial2.read());
  // }
  Serial.printf("\r\n");

  Serial.printf("get data.\r\n");
  // 读取所拍图片数据指令:56 00 32 0C 00 0A 00 00 XXXX 0000 YYYY 00FF
  // 返回:7600320000 FFD8 。FFD9 7600320000
  File file = SD.open("/pic.jpg", FILE_WRITE);

  delay(100);
  Serial.printf("\r\n data:\r\n");
  int count = 0;
  for(int i=0; i<pic_length; i+=128) {
    if(pic_length - i > 128) {
      camera_read_data(i, 128, get_buf);
      file.write(get_buf, 128);
    } else {
      camera_read_data(i, pic_length-i, get_buf);
      file.write(get_buf, pic_length-i);
    }

    // for(int i=0; i<128; i++) {
    //   file.write(message);
    //   Serial.printf("%2.2x ", get_buf[i]);
    //   count++;
    //   if((get_buf[i] == 0xd9) && (get_buf[i-1] == 0xff))
    //     break;
    // }
    // Serial.printf("\r\n");
  }
  file.close();
  Serial.printf("\r\n length:%d\r\n", count);

  // camera_read_data(0, 128, get_buf);
  // Serial.printf("\r\n data:\r\n");
  // for(int i=0; i<128; i++) {
  //   Serial.printf("%2.2x ", get_buf[i]);
  // }
  // Serial2.write(0x56);
  // Serial2.write(0x00);
  // Serial2.write(0x32);
  // Serial2.write(0x0c);

  // Serial2.write(0x00);
  // Serial2.write(0x0a);
  // Serial2.write(0x00);
  // Serial2.write(0x00);

  // Serial2.write(0x00);
  // Serial2.write(0x00);
  // Serial2.write(0x00);
  // Serial2.write(0x00);

  // Serial2.write(0x00);
  // Serial2.write(0x0f);
  // Serial2.write(0x00);
  // Serial2.write(0xff);
  
  // delay(100);
  // while(Serial2.available()) {
  //   // Serial.write(Serial2.read());
  //   Serial.printf("%2.2x ", Serial2.read());
  // }
  // Serial.printf("\r\n");

  // delay(1000);
}

// the loop routine runs over and over again forever
void loop() {

  // if(Serial.available()) {
  //   Serial2.write(Serial.read());
  // }

  // if(Serial2.available()) {
  //   Serial.write(Serial2.read());
  // }

  // m5.loop();
}
