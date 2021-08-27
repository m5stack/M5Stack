/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/unit/zigbee
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/unit/zigbee
*
* describe: zigbee.
* date：2021/8/27
*******************************************************************************
  Please connect to Port C,Supports configuring the Zigbee module into 3 working modes `Coordinator`, `Router`, `End Device`
  请连接端口C,支持将Zigbee模块配置为3种工作模式“协调器”、“路由器”、“终端设备”
  You need to configure a `Coordinator` when using, and other devices can be configured as `End Device` for data sending and receiving.
  使用时需要配置一个“Coordinator”，其他设备可以配置为“End Device”，用于数据的发送和接收。
  Note: 16 and 17 of the DIP switch are set to ON. This case needs to use the Keyboard in the FACE Kit for information input
  注:拨码开关16、17为ON。这种情况需要使用FACE Kit中的键盘进行信息输入
*/


#include "M5Stack.h"
#include "byteArray.h"
#include "DRFZigbee.h"

#include <stdarg.h>
#include <initializer_list>

#include "resource.h"

DRFZigbee zigbee;

uint16_t atNow = 29999;
uint16_t iconpos[3][6] = {
  {0, 40, 120, 140, 53, 0},
  {100, 40, 120, 140, 0, 0},
  {257, 40, 63, 140, 0, 0},
};

const uint8_t *iconptrbuff[3] = {
  coordinator_jpeg_120x140,
  endDevice_jpeg_120x140,
  router_jpeg_120x140,
};
size_t iconSizeBuff[3] = {
  27537,
  24609,
  26433,
};

char asciiHexList[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

bool flushFlag = true;

int drawChatBubbles(String str,uint16_t addr)
{
  uint16_t posY = 76;
  M5.Lcd.drawJpg(chatBubblesTop, 10561, 60, 66, 229, 10);
  M5.Lcd.setTextDatum(TL_DATUM);
  M5.Lcd.setTextColor(M5.Lcd.color565(0x44, 0x44, 0x44));
  while (str.length() > 0)
  {
      M5.Lcd.fillRect(60, posY, 228, 30, M5.Lcd.color565(0x88, 0xd1, 0xff));
      String dispalyStr = str.substring(0, 24);
      M5.Lcd.drawString(dispalyStr, 75, posY + 2, 2);
      str.remove(0, 24);
      posY += 20;
  }
  M5.Lcd.drawJpg(chatBubblesBottom, 11546, 60, posY, 229, 20);

  M5.Lcd.fillEllipse(27,posY+18,23,23,M5.Lcd.color565(200, 200, 200));

  char panIDchar[5] = {0, 0, 0, 0, 0};
  for (int i = 0; i < 4; i++)
  {
      panIDchar[i] = asciiHexList[addr & 0x000f];
      addr >>= 4;
  }

  M5.Lcd.setTextDatum(CC_DATUM);
  M5.Lcd.setTextColor(M5.Lcd.color565(20, 20, 20));
  M5.Lcd.drawString(panIDchar,27,posY+20);
  return posY;
}

void setup()
{
  M5.begin();
  Serial2.begin(38400, SERIAL_8N1, 16, 17);

  zigbee.begin(Serial2);

  M5.Lcd.fillRect(0, 0, 320, 240, M5.Lcd.color565(56, 56, 56));

  M5.Lcd.drawJpg(router_jpeg_120x140, 26433, 0, 40, 120, 140, 53, 0);
  M5.Lcd.drawJpg(coordinator_jpeg_120x140, 26433, 100, 40, 120, 140, 0, 0);
  M5.Lcd.drawJpg(endDevice_jpeg_120x140, 26433, 257, 40, 63, 140, 0, 0);

  DRFZigbee::zigbee_arg_t *arg = new DRFZigbee::zigbee_arg_t;
  zigbee.linkMoudle();
  zigbee.readModuleparm(arg);

  Serial.printf("PAIN ID:%04X\r\n", arg->main_PANID);

  //drawChatBubbles(" STC!!!!!");

  /*
  xTaskCreate([](void *arg){
      uint8_t readbuff[128];
      while(1){
          int length = Serial2.available();
          if( length > 0) { Serial2.readBytes(readbuff,length);Serial.write(readbuff,length);}
          delay(10);
      }
  }, "reviceTask", 2048, nullptr, 2, nullptr);
  */

  /*
  if( zigbee.sendCMDAndWaitRevice(0xfc,ZIGBEE_CMD_LINKMODULE) != DRFZigbee::kReviceOK )
  {
      Serial.printf("Link Zigbee module faild! code:%d\r\n",zigbee.lastErrorcode);
      while(1) delay(100);
  }

  byteArray array;
  if( zigbee.sendCMDAndWaitRevice(0xfc,ZIGBEE_CMD_READPARM,&array) != DRFZigbee::kReviceOK )
  {
      Serial.printf("Read module pram faild! code:%d\r\n",zigbee.lastErrorcode);
      while(1) delay(100);
  }

  if(( array.at(0) != 0x0A )||( array.at(1) != 0x0E ))
  {
      Serial.println("Read module pram faild!");
      while(1) delay(100);
  }
  array = array.mid(2);

  array.printfArray<HardwareSerial>(&Serial);

  DRFZigbee::zigbee_arg arg;
  memcpy(arg.Wave,array.dataptr(),sizeof(DRFZigbee::zigbee_arg));

  arg.main_pointType = DRFZigbee::kCoordinator; 
  arg.main_PANID = DRFZigbee::swap<uint16_t>(0x6889);
  arg.main_channel = 20;
  arg.main_ATN = DRFZigbee::kANTEXP;
  arg.main_transmissionMode = DRFZigbee::kN2Ntransmission;
  arg.main_customID = DRFZigbee::swap<uint16_t>(0x1213);
  arg.res3 = 0x01;

  byteArray sendArray;
  sendArray += 0x07;
  sendArray += byteArray(&arg.Wave[0],16);
  sendArray += byteArray(&arg.Wave[24],16);
  sendArray += byteArray(&arg.Wave[42],6);

  sendArray.printfArray<HardwareSerial>(&Serial);

  if( zigbee.sendCMDAndWaitRevice(0xfc,sendArray,&array) != DRFZigbee::kReviceOK )
  {
      Serial.printf("Read module pram faild! code:%d\r\n",zigbee.lastErrorcode);
      while(1) delay(100);
  }
  
  if( zigbee.sendCMDAndWaitRevice(0xfc,{0x06,0x44,0x54,0x4b,0xaa,0xbb}) != DRFZigbee::kReviceOK )
  {
      Serial.printf("reboot Zigbee module faild! code:%d\r\n",zigbee.lastErrorcode);
      while(1) delay(100);
  }
  
  delay(1000);
  */

  //zigbee.sendDataP2P(DRFZigbee::kP2PShortAddrMode,0x0000,{0x01,0x02,0x03,0x04});

  delay(1000);
  /*
  zigbee.nodeList.clear();
  for (int i = 0; i < 10; i++)
  {
      zigbee.getNetworksTopology();
      delay(100);
  }
  */

  /*
  M5.Lcd.fillRect(0,0,320,240,TFT_BLACK);
  int count = 0;
  for (int i = 0; i < 20; i++)
  {
      int n = ( count < 3 ) ? 0 : (int)log2(1-(((count + 3) * (-1)/3)));
      int angle2 = 120 * pow(0.5,n);
      int angle = (n > 0) ? angle2 + ( count - 3 * pow(2,n - 1))* angle2 * 2 : ( count % 3 ) * 120;
      //int angle = ( count < 3 ) ? ( count % 3 ) * 120 : ((120 / (((count / 3) * 2))) + ( count % 3 ) * 120);
      double posX = cos(PI*angle/180) * 100 + 160;
      double posY = sin(PI*angle/180) * 100 + 120;

      Serial.printf("Point %02d:%03d,%03d\r\n",count,(int)posX,(int)posY);

      M5.Lcd.fillEllipse((int)posX,(int)posY,10,10,TFT_WHITE);
      delay(200);
      count++;
  }
  */

  /*
  if( !zigbee.nodeList.empty() )
  {
      int count = 0;
      std::map<int,DRFZigbee::node>::iterator iter;
      for( iter = zigbee.nodeList.begin(); iter != zigbee.nodeList.end();iter ++ )
      {


          iter->first;
          iter->second;
          count++;
      }
  }
  */
}

void AppCoordinator()
{
  Serial.printf("AppCoordinator\r\n");
  M5.Lcd.fillRect(0, 0, 320, 240, M5.Lcd.color565(56, 56, 56));
  M5.Lcd.drawJpg(coordinatorTitle, 21823, 0, 10, 320, 35);

  DRFZigbee::zigbee_arg_t *arg = new DRFZigbee::zigbee_arg_t;
  zigbee.readModuleparm(arg);
  arg->main_pointType = DRFZigbee::kCoordinator;

  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.setTextDatum(TC_DATUM);

  char panIDchar[5] = {0, 0, 0, 0, 0};
  uint16_t panID = DRFZigbee::swap<uint16_t>(arg->main_PANID);
  for (int i = 0; i < 4; i++)
  {
      panIDchar[i] = asciiHexList[panID & 0x000f];
      panID >>= 4;
  }

  M5.Lcd.setTextColor(M5.Lcd.color565(30, 30, 30));
  M5.Lcd.drawString(panIDchar, 165, 95, 8);
  M5.Lcd.setTextColor(TFT_WHITE);
  M5.Lcd.drawString(panIDchar, 160, 90, 8);
  zigbee.setModuleparm(*arg);
  zigbee.rebootModule();
  while (1)
  {
      M5.update();
      if (M5.BtnB.wasPressed())
          break;
  }

  delete arg;
}

void AppRouter()
{
  Serial.printf("AppRouter\r\n");
  M5.Lcd.fillRect(0, 0, 320, 240, M5.Lcd.color565(56, 56, 56));
  M5.Lcd.drawJpg(EndDeviceTitle, 17470, 0, 10, 320, 35);

  DRFZigbee::zigbee_arg_t *arg = new DRFZigbee::zigbee_arg_t;
  zigbee.readModuleparm(arg);
  arg->main_pointType = DRFZigbee::kRouter;
  arg->main_PANID = DRFZigbee::swap<uint16_t>(0x1617);
  arg->main_transmissionMode = DRFZigbee::kN2Ntransmission;
  arg->main_channel = 20;
  arg->main_ATN = DRFZigbee::kANTEXP;
  zigbee.setModuleparm(*arg);
  zigbee.rebootModule();

  pinMode(5, INPUT_PULLUP);

  while (1)
  {
    DRFZigbee::reviceData_t revice;
    if (zigbee.reviceData(&revice) == DRFZigbee::kReviceOK)
    {
      revice.array->printfArray<HardwareSerial>(&Serial);
    }
  }
}

void AppEndDevice()
{
  Serial.printf("AppEndDevice\r\n");
  M5.Lcd.fillRect(0, 0, 320, 240, M5.Lcd.color565(56, 56, 56));
  M5.Lcd.drawJpg(EndDeviceTitle, 17470, 0, 10, 320, 35);

  DRFZigbee::zigbee_arg_t *arg = new DRFZigbee::zigbee_arg_t;
  zigbee.readModuleparm(arg);
  arg->main_pointType = DRFZigbee::kEndDevice;
  arg->main_PANID = DRFZigbee::swap<uint16_t>(0x1617);
  arg->main_transmissionMode = DRFZigbee::kN2Ntransmission;
  zigbee.setModuleparm(*arg);
  zigbee.rebootModule();

  pinMode(5, INPUT_PULLUP);
  Wire.begin(21,22);

  M5.Lcd.fillRect(20, 200, 280, 30, M5.Lcd.color565(56, 56, 56));
  M5.Lcd.drawRect(20, 200, 280, 30, M5.Lcd.color565(200, 200, 200));

  uint8_t senduff[256];
  char    revicechar[256];
  uint16_t charPos = 0;

  memset(senduff,0,256);

  while (1)
  {
      DRFZigbee::reviceData_t revice;
      if (zigbee.reviceData(&revice,10) == DRFZigbee::kReviceOK)
      {
          revice.array->printfArray<HardwareSerial>(&Serial);
          M5.Lcd.fillRect(0, 66, 320, 128, M5.Lcd.color565(56, 56, 56));
          memset(revicechar,0,256);
          memcpy(revicechar,revice.array->dataptr(),revice.length);
          drawChatBubbles(String(revicechar),revice.fromAddr);
      }

      if (digitalRead(5) == LOW)
      {
          Wire.requestFrom(0x08, 1);
          while (Wire.available())
          {
              uint8_t key_val = Wire.read();
              if (key_val != 0)
              {
                  if (key_val >= 0x20 && key_val < 0x7F)
                  { 
                      senduff[charPos] = key_val;
                      Serial.print((char)key_val);
                      M5.Lcd.setTextDatum(TL_DATUM);
                      M5.Lcd.setTextColor(M5.Lcd.color565(200, 200, 200));
                      M5.Lcd.drawString(String((char*)senduff),30,206,2);
                      charPos++;
                  }
                  else if( key_val == 0x0d)
                  {
                      M5.Lcd.fillRect(20, 200, 280, 30, M5.Lcd.color565(56, 56, 56));
                      M5.Lcd.drawRect(20, 200, 280, 30, M5.Lcd.color565(200, 200, 200));
                      zigbee.sendDataP2P(DRFZigbee::kP2PShortAddrMode,0xffff,senduff,charPos);
                      memset(senduff,0,256);
                      charPos = 0;
                  }
              }
          }
      }
  }
}

void loop()
{
  if (M5.BtnA.wasPressed())
  {
      atNow = (atNow >= 60000) ? 30000 : atNow;
      atNow++;
      flushFlag = true;
  }
  else if (M5.BtnB.wasPressed())
  {
      switch ((atNow + 2) % 3)
      {
      case 0:
          AppRouter();
          break;
      case 1:
          AppCoordinator();
          break;
      case 2:
          AppEndDevice();
          break;
      }
      flushFlag = true;
  }
  else if (M5.BtnC.wasPressed())
  {
      atNow = (atNow == 0) ? 30000 : atNow;
      atNow--;
      flushFlag = true;
  }
  if (flushFlag)
  {
      flushFlag = false;
      for (int i = 0; i < 3; i++)
      {
          M5.Lcd.drawJpg(iconptrbuff[(atNow + i) % 3],
                          iconSizeBuff[(atNow + i) % 3],
                          iconpos[i][0],
                          iconpos[i][1],
                          iconpos[i][2],
                          iconpos[i][3],
                          iconpos[i][4],
                          iconpos[i][5]);
      }
  }
  M5.update();
  delay(10);
}