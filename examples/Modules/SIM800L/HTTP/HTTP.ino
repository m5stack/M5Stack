#include "M5Stack.h"
#include "TFTTerminal.h"
TFT_eSprite TerminalBuff = TFT_eSprite(&M5.Lcd);
TFTTerminal terminal(&TerminalBuff);

String waitRevice()
{
    String recvStr;
    do
    {
        recvStr = Serial2.readStringUntil('\n');
    } while (recvStr.length() == 0);
    Serial.println(recvStr);
    terminal.println(recvStr);
    return recvStr;
}

void sendATCMD(String cmdStr)
{
    Serial2.print(cmdStr);
    delay(100);
}

int sendATCMDAndRevice(String cmdStr)
{
    delay(1000);
    Serial2.print(cmdStr);
    delay(100);
    waitRevice();
    String recvStr = waitRevice();
    delay(100);
    if (recvStr.indexOf("OK") != -1)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

void GET() {
    terminal.println("GET Request");
    sendATCMD("AT?\r\n");
    delay(100);
    Serial2.flush();
    sendATCMDAndRevice("AT+CGATT?\r\n");
    sendATCMDAndRevice("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n");
    sendATCMDAndRevice("AT+SAPBR=3,1,\"APN\",\"CMNET\"\r\n");
    sendATCMDAndRevice("AT+SAPBR=0,1\r\n");
    sendATCMDAndRevice("AT+SAPBR=1,1\r\n");
    sendATCMDAndRevice("AT+HTTPINIT\r\n");
    sendATCMDAndRevice("AT+HTTPPARA=\"URL\",\"http://api.m5stack.com/v1\"\r\n");
    sendATCMDAndRevice("AT+HTTPACTION=0\r\n");
    // while(!Serial2.available());
    delay(5000);
    sendATCMDAndRevice("AT+HTTPREAD\r\n");
    delay(1000);
    String recvStr;
    while(Serial2.available()){
      recvStr+= Serial2.readString();
    }
    Serial.println(recvStr);
    terminal.println(recvStr);
    sendATCMDAndRevice("AT+HTTPTERM\r\n");
}

void POST() {
    terminal.println("POST Request");
    sendATCMD("AT?\r\n");
    delay(100);
    Serial2.flush();
    sendATCMDAndRevice("AT+CGATT?\r\n");
    sendATCMDAndRevice("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"\r\n");
    sendATCMDAndRevice("AT+SAPBR=3,1,\"APN\",\"CMNET\"\r\n");
    sendATCMDAndRevice("AT+SAPBR=0,1\r\n");
    sendATCMDAndRevice("AT+SAPBR=1,1\r\n");
    sendATCMDAndRevice("AT+HTTPINIT\r\n");
    sendATCMDAndRevice("AT+HTTPPARA=\"CID\",1\r\n");
    sendATCMDAndRevice("AT+HTTPPARA=\"CONTENT\",\"application/x-www-form-urlencoded\"\r\n");
    sendATCMDAndRevice("AT+HTTPPARA=\"URL\",\"http://header.json-json.com/\"\r\n");
    sendATCMD("AT+HTTPDATA=10,3000\r\n");
    sendATCMD("M5STACK,GO\r\n");
    sendATCMDAndRevice("AT+HTTPACTION=1\r\n");
    delay(5000);
    sendATCMDAndRevice("AT+HTTPREAD\r\n");
    delay(1000);
    String recvStr;
    while(Serial2.available()){
      recvStr+= Serial2.readString();
    }
    Serial.println(recvStr);
    terminal.println(recvStr);
    sendATCMDAndRevice("AT+HTTPTERM\r\n");
}

void setup()
{
    M5.begin();
    Serial2.begin(115200, SERIAL_8N1, 16, 17);
    Serial2.flush();
    delay(1000);
    M5.Lcd.fillRect(0, 0, 320, 240, TFT_BLACK);
    M5.Lcd.fillRect(0, 0, 320, 40, TFT_WHITE);
    M5.Lcd.setTextColor(TFT_BLACK);
    M5.Lcd.setTextDatum(TC_DATUM);
    M5.Lcd.drawString("SIM800L HTTP GET/POST", 160, 10, 4);
    M5.Lcd.setTextDatum(TL_DATUM);
    M5.Lcd.setTextColor(TFT_WHITE);
    TerminalBuff.createSprite(240,200);
    terminal.setGeometry(20,55,300,200);
    terminal.setFontsize(1);
    terminal.println("Press Btn A GET Request");
    terminal.println("Press Btn B POST Request");
};


void loop()
{
  M5.update();
  if(M5.BtnA.wasPressed()) GET();
  if(M5.BtnB.wasPressed()) POST();
  if(M5.BtnC.wasPressed()) {
    String recvStr;
    recvStr = Serial2.readStringUntil('\n');
    Serial.println(recvStr);
    terminal.println(recvStr);
  };
  delay(10);
}

