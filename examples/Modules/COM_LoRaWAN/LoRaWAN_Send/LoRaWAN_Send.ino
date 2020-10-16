/*
  Description: Press ButtonA to send "Hello" to 868Mhz Frequency
*/

#include <M5Stack.h>
#include "TFTTerminal.h"

TFT_eSprite Disbuff = TFT_eSprite(&M5.Lcd);
TFT_eSprite TerminalBuff = TFT_eSprite(&M5.Lcd);
TFTTerminal terminal(&TerminalBuff);

void ATCommand(char cmd[],char date[], uint32_t timeout = 300)
{
  char buf[256] = {0};
  if(date == NULL)
  {
    sprintf(buf,"AT+%s",cmd);
  }
  else 
  {
    sprintf(buf,"AT+%s=%s",cmd,date); 
  }
  Serial2.write(buf);
  terminal.println(buf);
  ReceiveAT(timeout);
}

bool ReceiveAT(uint32_t timeout)
{
  uint32_t nowtime = millis();
  while(millis() - nowtime < timeout){
    if (Serial2.available() !=0) {
      String str = Serial2.readString();
      if (str.indexOf("+OK") != -1 || str.indexOf("+ERROR") != -1) {
//        terminal.println(str);
        Serial.println(str);
        return true;
      }else {
        Serial.println("Syntax Error");
        break;
      }
    }
  }
  Serial.println("Timeout");
  return false;
}

void setup() 
{
  M5.begin();
  Serial.begin(115200);
  Serial2.begin(115200, SERIAL_8N1, 15, 13);

  TerminalBuff.createSprite(250,220);
  TerminalBuff.fillRect(0,0,250,220,BLACK);
  TerminalBuff.setTextColor(GREEN);
  TerminalBuff.pushSprite(0,0);
  terminal.setFontsize(2);
  terminal.setGeometry(0,0,250,220);
  ATCommand("LORAWAN", "0");
  ATCommand("FREQ", "868000000");
  ATCommand("PrintMode", "0");
//  ATCommand("RX", "0");
}

void loop() 
{
  if(M5.BtnA.wasPressed()) {
    ATCommand("SendStr", "Hello");    
  }
  
//  if(Serial2.available() > 0) {
//    String receive_data = Serial2.readString();
//    Serial.println(receive_data);
//    terminal.println(receive_data);
//    ATCommand("RX", "0");
//  } 
  M5.update();
}
