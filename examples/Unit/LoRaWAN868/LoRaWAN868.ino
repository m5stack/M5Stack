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

String waitRevice()
{
  String recvStr;
  do
  {
      recvStr = Serial2.readStringUntil('\n');
  } while (recvStr.length() == 0);
  canvas.println(recvStr);
  return recvStr;
}

uint16_t ypos = 55;

void drawLineStr(String str, uint16_t color)
{
  M5.Lcd.setTextColor(color);
  M5.Lcd.drawString(str, 10, ypos, 4);
  ypos += 32;
}


void sendATCMD(String cmdStr)
{
  Serial2.print(cmdStr);
  delay(100);
}

int sendATCMDAndRevice(String cmdStr)
{
  Serial2.print(cmdStr);
  delay(10);
  waitRevice();
  String recvStr = waitRevice();
  if (recvStr.indexOf("OK") != -1)
  {
    return 0;
  }
  else
  {
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
  display.println("LoRaWAN868");
  canvas.setColorDepth(1); // mono color
  canvas.createSprite(display.width(), display.height());
  canvas.setTextSize((float)canvas.width() / 160);
  canvas.setTextScroll(true);

  sendATCMD("AT?\r");
  delay(100);
  Serial2.flush();
  sendATCMDAndRevice("AT+ILOGLVL=0\r");
  sendATCMDAndRevice("AT+CSAVE\r");
  sendATCMDAndRevice("AT+IREBOOT=0\r");
  display.println("LoraWan Rebooting");
  delay(2000);
  display.println("LoraWan config");
  sendATCMDAndRevice("AT+CJOINMODE=0\r");
  sendATCMDAndRevice("AT+CDEVEUI=00bb9da5b97addf1\r");
  sendATCMDAndRevice("AT+CAPPEUI=70B3D57ED004247E\r");//70B3D57ED003B699
  sendATCMDAndRevice("AT+CAPPKEY=27DFE264CA33AC1957C005EB48BA4721\r");
  sendATCMDAndRevice("AT+CULDLMODE=2\r");
  sendATCMDAndRevice("AT+CCLASS=2\r");
  sendATCMDAndRevice("AT+CWORKMODE=2\r");
  sendATCMDAndRevice("AT+CNBTRIALS=0,5\r");
  sendATCMDAndRevice("AT+CNBTRIALS=1,5\r");

  // TX Freq
  // 868.1 - SF7BW125 to SF12BW125
  // 868.3 - SF7BW125 to SF12BW125 and SF7BW250
  // 868.5 - SF7BW125 to SF12BW125
  // 867.1 - SF7BW125 to SF12BW125
  // 867.3 - SF7BW125 to SF12BW125
  // 867.5 - SF7BW125 to SF12BW125
  // 867.7 - SF7BW125 to SF12BW125
  // 867.9 - SF7BW125 to SF12BW125
  // 868.8 - FSK

  sendATCMDAndRevice("AT+CFREQBANDMASK=0001\r");

  //869.525 - SF9BW125 (RX2)              | 869525000
  //sendATCMDAndRevice("AT+CRXP=0,0,869525000\r");

  sendATCMDAndRevice("AT+CSAVE\r");
  
  sendATCMDAndRevice("AT+CJOIN=1,0,10,8\r");
}

enum systemstate
{
  kIdel = 0,
  kJoined,
  kSending,
  kWaitSend,
  kEnd,
};
int system_fsm = kIdel;

int loraWanSendNUM = -1;
int loraWanSendCNT = -1;
int loraWanupLinkCNT = 0;
int loraWanupLinkReviceCNT = 0;

void loop()
{
  String recvStr = waitRevice();
  if (recvStr.indexOf("+CJOIN:") != -1)
  {
    if (recvStr.indexOf("OK") != -1)
    {
        canvas.println("[ INFO ] JOIN IN SUCCESSFUL");
        system_fsm = kJoined;
    }
    else
    {
        canvas.println("[ INFO ] JOIN IN FAIL");
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
      system_fsm = kEnd;
      char strbuff[128];
      loraWanupLinkReviceCNT ++;

      //if(( loraWanSendCNT < 5 )&&( loraWanSendNUM == 8 ))
      //{
      //    loraWanupLinkReviceCNT ++;
      //    sprintf(strbuff,"SEND CNT: %d (%d/%d)",loraWanSendCNT,loraWanupLinkReviceCNT,loraWanupLinkCNT);
      //    M5.Lcd.fillRect(0,183,320,32,TFT_BLACK);
      //    M5.Lcd.setTextColor(TFT_GREEN);
      //    M5.Lcd.drawString(strbuff, 10, 183, 4);
      //    //drawLineStr(strbuff, TFT_GREEN);
      //    
      //}
      //else
      //{
      //    sprintf(strbuff,"FAILD NUM:%d CNT:%d (%d/%d)",loraWanSendNUM,loraWanSendCNT,loraWanupLinkReviceCNT,loraWanupLinkCNT);
      //    M5.Lcd.fillRect(0,183,320,32,TFT_BLACK);
      //    M5.Lcd.setTextColor(TFT_RED);
      //    M5.Lcd.drawString(strbuff, 10, 183, 4);
      //    drawLineStr(strbuff, TFT_RED);
      //}
      delay(500);
      system_fsm = kSending;
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
  else if(recvStr.indexOf("ERR+SENT") != -1)
  {
    char strbuff[128];
    String ErrorCodeStr = recvStr.substring(9);
    sprintf(strbuff,"ERROR Code:%d (%d/%d)",ErrorCodeStr.toInt(),loraWanupLinkReviceCNT,loraWanupLinkCNT);
    canvas.println(strbuff);
    delay(500);

    system_fsm = kSending;
  }
  else if(recvStr.indexOf("+CLINKCHECK:") != -1)
  {
    String checkStr = recvStr.substring(String("+CLINKCHECK:").length());

    char strbuff[128];
    sprintf(strbuff,"%s (%d/%d)",checkStr.c_str(),loraWanupLinkReviceCNT,loraWanupLinkCNT);
    canvas.println(strbuff);

  }

  if (system_fsm == kSending)
  {
    canvas.println("LoraWan Sending");
    sendATCMD("AT+CLINKCHECK=1\r");
    loraWanupLinkCNT ++;
    system_fsm = kWaitSend;
  }
  canvas.pushSprite(0, 0);
  delay(10);
} 