#include "M5Stack.h"
#include "freertos/queue.h"

String waitRevice()
{
    String recvStr;
    do
    {
        recvStr = Serial2.readStringUntil('\n');
    } while (recvStr.length() == 0);
    Serial.println(recvStr);
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
    delay(10);
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
    //xTaskCreate(serialTask, "serialTask", 1024 * 2, (void *)0, 4, nullptr);
    
    M5.Lcd.fillRect(0, 0, 320, 240, TFT_BLACK);
    M5.Lcd.fillRect(0, 0, 320, 40, TFT_WHITE);
    M5.Lcd.setTextColor(TFT_BLACK);
    M5.Lcd.setTextDatum(TC_DATUM);
    M5.Lcd.drawString("COMX-LoraWan(868) TEST", 160, 10, 4);
    M5.Lcd.setTextDatum(TL_DATUM);
    M5.Lcd.setTextColor(TFT_WHITE);

    sendATCMD("AT?\r");
    delay(100);
    Serial2.flush();
    sendATCMDAndRevice("AT+ILOGLVL=0\r");
    sendATCMDAndRevice("AT+CSAVE\r");
    sendATCMDAndRevice("AT+IREBOOT=0\r");
    drawLineStr("LoraWan Rebooting", TFT_WHITE);
    delay(2000);
    drawLineStr("LoraWan config", TFT_WHITE);
    sendATCMDAndRevice("AT+CJOINMODE=0\r");
    sendATCMDAndRevice("AT+CDEVEUI=00BB9DA5B97ADDF6\r");
    sendATCMDAndRevice("AT+CAPPEUI=70B3D57ED004247E\r");//70B3D57ED003B699
    sendATCMDAndRevice("AT+CAPPKEY=27DFE264CA33AC1957C005EB48BA4723\r");
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
            Serial.println("[ INFO ] JOIN IN SUCCESSFUL");
            drawLineStr("LoraWan JOIN", TFT_GREEN);
            system_fsm = kJoined;
        }
        else
        {
            Serial.println("[ INFO ] JOIN IN FAIL");
            drawLineStr("LoraWan JOIN FAIL", TFT_RED);
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
            //system_fsm = kEnd;
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
        
        //Serial.printf(" [ INFO ] SEND NUM %s \r\n",snednum.c_str());
        loraWanSendNUM = snednum.toInt();
    }
    else if(recvStr.indexOf("OK+SENT") != -1)
    {
        String snedcnt = recvStr.substring(8);
        //Serial.printf(" [ INFO ] SEND CNT %s \r\n",snedcnt.c_str());
        loraWanSendCNT = snedcnt.toInt();
    }
    else if(recvStr.indexOf("ERR+SENT") != -1)
    {
        char strbuff[128];
        String ErrorCodeStr = recvStr.substring(9);
        sprintf(strbuff,"ERROR Code:%d (%d/%d)",ErrorCodeStr.toInt(),loraWanupLinkReviceCNT,loraWanupLinkCNT);
        M5.Lcd.fillRect(0,183,320,32,TFT_BLACK);
        M5.Lcd.setTextColor(TFT_RED);
        M5.Lcd.drawString(strbuff, 10, 183, 4);

        delay(500);

        system_fsm = kSending;
    }
    else if(recvStr.indexOf("+CLINKCHECK:") != -1)
    {
        String checkStr = recvStr.substring(String("+CLINKCHECK:").length());

        char strbuff[128];
        sprintf(strbuff,"%s (%d/%d)",checkStr.c_str(),loraWanupLinkReviceCNT,loraWanupLinkCNT);

        M5.Lcd.fillRect(0,183,320,32,TFT_BLACK);
        M5.Lcd.setTextColor(TFT_GREEN);
        M5.Lcd.drawString(strbuff, 10, 183, 4);

    }

    if (system_fsm == kSending)
    {
        //drawLineStr("LoraWan Sending", TFT_WHITE);
        M5.Lcd.setTextColor(TFT_WHITE);
        M5.Lcd.drawString("LoraWan Sending", 10, 151, 4);
        //sendATCMD("AT+DTRX=1,2,8,4655434b20535443\r");
        sendATCMD("AT+CLINKCHECK=1\r");
        loraWanupLinkCNT ++;
        system_fsm = kWaitSend;
    }

    //if (M5.BtnA.wasPressed())
    //{
    //    sendATCMDAndRevice("AT+CLINKCHECK=1\r");
    //    delay(100);
    //}
    //if (M5.BtnB.wasPressed())
    //{
    //    sendATCMDAndRevice("AT+DTRX=1,2,8,4655434b20535443\r");
    //    delay(100);
    //}
    //if (M5.BtnC.wasPressed())
    //{
    //    sendATCMDAndRevice("AT+DRX?\r");
    //    delay(100);
    //}
    
    delay(10);
    M5.update();
}