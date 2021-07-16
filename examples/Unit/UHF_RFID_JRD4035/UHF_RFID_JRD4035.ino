#include <M5Stack.h>
#include "RFID_command.h"

#include "TFTTerminal.h"
TFT_eSprite TerminalBuff = TFT_eSprite(&M5.Lcd);
TFTTerminal terminal(&TerminalBuff);

UHF_RFID RFID;


String comd = " ";
CardpropertiesInfo card;
ManyInfo cards;
SelectInfo Select;
CardInformationInfo Cardinformation;
QueryInfo Query;
ReadInfo Read;
TestInfo Test;


void setup()
{
  M5.begin();

  RFID._debug = 0;
  Serial2.begin(115200, SERIAL_8N1, 16, 17);//16.17
  if (RFID._debug == 1)Serial.begin(115200, SERIAL_8N1, 21, 22);
  M5.Lcd.fillRect(0, 0, 340, 280, BLACK);

  TerminalBuff.createSprite(280,200);
  terminal.setGeometry(20,30,300,200);
  terminal.setFontsize(1);

// UHF_RFID set UHF_RFID设置
  RFID.Set_transmission_Power(2600);
  RFID.Set_the_Select_mode();
  RFID.Delay(100);
  RFID.Readcallback();
  RFID.clean_data();
  
// Prompted to connect to UHF_RFID 提示连接UHF_RFID
  terminal.println("Please connect UHF_RFID to Port C");
  
// Determined whether to connect to UHF_RFID 判断是否连接UHF_RFID
  String soft_version;
  soft_version = RFID.Query_software_version();
  while(soft_version.indexOf("V2.3.5") == -1)
  {
    RFID.clean_data();
    M5.Lcd.fillCircle(310, 10, 6, RED);
    RFID.Delay(150);
    M5.Lcd.fillCircle(310, 10, 6, BLACK);
    RFID.Delay(150);
    soft_version = RFID.Query_software_version();
  }

// The prompt will be RFID card close 提示将RFID卡靠近
  terminal.println("Please approach the RFID card you need to use");
  
}

void loop()
{
 // breathing light 呼吸灯
 M5.Lcd.fillCircle(310, 10, 6, GREEN);
 RFID.Delay(150);
 M5.Lcd.fillCircle(310, 10, 6, BLACK);
 RFID.Delay(150);

//  A read/write operation specifies a particular card 读写操作需指定某一张卡
//  comd = RFID.Set_the_select_parameter_directive("30751FEB705C5904E3D50D70");
//  terminal.println(comd);
//  RFID.clean_data();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Query the card information once 查询一次卡的信息例子
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 card = RFID.A_single_poll_of_instructions();
 if (card._ERROR.length() != 0)
 {
   Serial.print(card._ERROR);
 }
 else
 {
  if(card._EPC.length() == 24)
  {
     terminal.println("RSSI :" + card._RSSI);
     terminal.println("PC :" + card._PC);
     terminal.println("EPC :" + card._EPC);
     terminal.println("CRC :" + card._CRC);
     terminal.println(" ");
   }
 }
   RFID.clean_data(); //Empty the data after using it 使用完数据后要将数据清空
  

/*Other feature usage examples 其他功能使用例子*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Read multiple RFID cards at once 一次读取多张RFID卡
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//  cards = RFID.Multiple_polling_instructions(6);
//  for (size_t i = 0; i < cards.len; i++)
//  {
//    if(cards.card[i]._EPC.length() == 24)
//      {
//         terminal.println("RSSI :" + cards.card[i]._RSSI);
//         terminal.println("PC :" + cards.card[i]._PC);
//         terminal.println("EPC :" + cards.card[i]._EPC);
//         terminal.println("CRC :" + cards.card[i]._CRC);
//       }
//  }
//  terminal.println(" ");  
//  RFID.clean_data();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Used to get the SELECT parameter 用于获取Select参数
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//  Select = RFID.Get_the_select_parameter();
//  if(Select.Mask.length() != 0)
//  {
//    terminal.println("Mask :" + Select.Mask);
//    terminal.println("SelParam :" + Select.SelParam);
//    terminal.println("Ptr :" + Select.Ptr);
//    terminal.println("MaskLen :" + Select.MaskLen);
//    terminal.println("Truncate :" + Select.Truncate);
//    terminal.println(" ");
//  }
//    RFID.clean_data();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Used to change the PSF bit of the NXP G2X label 用于改变 NXP G2X 标签的 PSF 位
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//  Cardinformation = RFID.NXP_Change_EAS(0x00000000);
//  if(Cardinformation._UL.length() != 0)
//  {
//    terminal.println("UL :" + Cardinformation._UL);
//    terminal.println("PC :" + Cardinformation._PC);
//    terminal.println("EPC :" + Cardinformation._EPC);
//    terminal.println("Parameter :" + Cardinformation._Parameter);
//    terminal.println("ErrorCode :" + Cardinformation._ErrorCode);
//    terminal.println("Error :" + Cardinformation._Error);
//    terminal.println("Data :" + Cardinformation._Data);
//    terminal.println("Successful :" + Cardinformation._Successful);
//    terminal.println(" ");
//   }
//    RFID.clean_data();
  

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Used to get the Query parameters 用于获取Query参数
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//  Query = RFID.Get_the_Query_parameter();
//  if(Query.QueryParameter.length() != 0)
//  {
//    terminal.println("QueryParameter :" + Query.QueryParameter);
//    terminal.println("DR :" + Query.DR);
//    terminal.println("M :" + Query.M);
//    terminal.println("TRext :" + Query.TRext);
//    terminal.println("Sel :" + Query.Sel);
//    terminal.println("Session :" + Query.Session);
//    terminal.println("Targetta :" + Query.Target);
//    terminal.println("Q :" + Query.Q);
//    terminal.println(" ");
//  }
//  RFID.clean_data();


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Used to get the Query parameters 用于读取接收解调器参数
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//  Read = RFID.Read_receive_demodulator_parameters();
//  if(Read.Mixer_G.length()!= 0)
//  {
//    terminal.println("Mixer_G :" + Read.Mixer_G);
//    terminal.println("IF_G :" + Read.IF_G);
//    terminal.println("Thrd :" + Read.Thrd);
//    terminal.println(" ");
//  }
//  RFID.clean_data();
}
