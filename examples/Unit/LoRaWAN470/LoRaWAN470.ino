/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/unit/lorawan868
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/unit/lorawan868
*
* describe: LoRaWAN868.
* date：2021/8/31
*******************************************************************************
  Please connect to Port C,请连接端口C
*/
#include "M5Stack.h"
#include "freertos/queue.h"
#include <M5GFX.h>

M5GFX display;
M5Canvas canvas(&display);

String waitRevice(){
  String recvStr;
  do{
    recvStr = Serial2.readStringUntil('\n');
  } while (recvStr.length() == 0);
  canvas.println(recvStr);
  return recvStr;
}

void sendATCMD(String cmdStr){
  Serial2.print(cmdStr);
  delay(100);
}

int sendATCMDAndRevice(String cmdStr){
  Serial2.print(cmdStr);
  delay(100);
  waitRevice();
  String recvStr = waitRevice();
  if (recvStr.indexOf("OK") != -1){
    return 0;
  }else{
    return -1;
  }
}

void setup()
{
  M5.begin();
  Serial2.begin(115200, SERIAL_8N1, 16, 17);
  Serial2.flush();
  delay(100);
  display.begin();
  display.setTextSize(2);
  canvas.setColorDepth(1); // mono color
  canvas.createSprite(display.width(), display.height());
  canvas.setTextSize((float)canvas.width() / 160);
  canvas.setTextScroll(true);

  sendATCMD("AT?\r\n");
  delay(100);
  Serial2.flush();
  sendATCMDAndRevice("AT+ILOGLVL=0\r\n");
  sendATCMDAndRevice("AT+CSAVE\r\n");
  sendATCMDAndRevice("AT+IREBOOT=0\r\n");
  display.println("LoraWan Rebooting");
  delay(2000);
  display.println("LoraWan config");
  //Set Join Mode OTAA.
  sendATCMDAndRevice("AT+CJOINMODE=0\r\n");
  sendATCMDAndRevice("AT+CDEVEUI=0037CAE1FC3542B9\r\n");
  sendATCMDAndRevice("AT+CAPPEUI=70B3D57ED003E04E\r\n");
  sendATCMDAndRevice("AT+CAPPKEY=67FA4ED1075A20573BCDD7594C458698\r\n");
  sendATCMDAndRevice("AT+CULDLMODE=2\r\n");
  //Set ClassC mode
  sendATCMDAndRevice("AT+CCLASS=2\r\n");
  sendATCMDAndRevice("AT+CWORKMODE=2\r\n");

  sendATCMDAndRevice("AT+CRXP=0,0,505300000\r\n");

  // TX Freq
  // 486.3
  // 486.5
  // 486.7
  // 486.9
  // 487.1
  // 487.3
  // 487.5
  // 487.7
  //MARK 0000 0100 0000 0000 | 0x0400

  sendATCMDAndRevice("AT+CFREQBANDMASK=0400\r");

  // RX Freq
  //506.7 (RX1)
  //506.9 (RX1)
  //507.1 (RX1)
  //507.3 (RX1)
  //507.5 (RX1)
  //507.7 (RX1)
  //507.9 (RX1)
  //508.1 (RX1)
  //505.3 (RX2)| 505300000
  sendATCMDAndRevice("AT+CJOIN=1,0,10,8\r\n");
}

enum systemstate{
  kIdel = 0,
  kJoined,
  kSending,
  kWaitSend,
  kEnd,
};
int system_fsm = kIdel;

int loraWanSendNUM = -1;
int loraWanSendCNT = -1;

void loop()
{
  String recvStr = waitRevice();
  if (recvStr.indexOf("+CJOIN:") != -1)
  {
      if (recvStr.indexOf("OK") != -1)
      {
          canvas.printf("LoraWan JOIN");
          system_fsm = kJoined;
      }
      else
      {
          canvas.printf("LoraWan JOIN FAIL");
          system_fsm = kIdel;
      }
  }
  else if (recvStr.indexOf("OK+RECV") != -1)
  {
      if (system_fsm == kJoined)
      {
          system_fsm = kSending;
      }
      else if (system_fsm == kWaitSend)
      {
          system_fsm = kSending;
          char strbuff[128];
          if(( loraWanSendCNT < 5 )&&( loraWanSendNUM == 8 ))
          {
              sprintf(strbuff,"TSET OK CNT: %d",loraWanSendCNT);
              canvas.print(strbuff);
          }
          else
          {
              sprintf(strbuff,"FAILD NUM:%d CNT:%d",loraWanSendNUM,loraWanSendCNT);
              canvas.print(strbuff);
          }
      }
  }
  else if(recvStr.indexOf("OK+SEND") != -1)
  {
      String snednum = recvStr.substring(8);
      canvas.printf(" [ INFO ] SEND NUM %s \r\n",snednum.c_str());
      loraWanSendNUM = snednum.toInt();
  }
  else if(recvStr.indexOf("OK+SENT") != -1)
  {
      String snedcnt = recvStr.substring(8);
      canvas.printf(" [ INFO ] SEND CNT %s \r\n",snedcnt.c_str());
      loraWanSendCNT = snedcnt.toInt();
  }

  if (system_fsm == kSending)
  {
      canvas.println("LoraWan Sending");
      sendATCMD("AT+DTRX=1,8,8,4655434b20535443\r\n");
      system_fsm = kWaitSend;
  }
  canvas.pushSprite(0, 0);
  delay(10);
}