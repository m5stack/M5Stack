/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/unit/uhf_rfid
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/unit/uhf_rfid
*
* describe: uhf_rfid.
* date：2021/9/1
*******************************************************************************
*/
#include <M5Stack.h>
#include "RFID_command.h"
#include <M5GFX.h>

M5GFX display;
M5Canvas canvas(&display);

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
  display.begin();
  display.setTextSize(2);
  canvas.setColorDepth(1); // mono color
  canvas.createSprite(display.width(), display.height());
  canvas.setTextSize((float)canvas.width() / 160);
  canvas.setTextScroll(true);

  RFID._debug = 0;
  Serial2.begin(115200, SERIAL_8N1, 16, 17);//16.17
  if (RFID._debug == 1)Serial.begin(115200, SERIAL_8N1, 21, 22);
  M5.Lcd.fillRect(0, 0, 340, 280, BLACK);

// UHF_RFID set UHF_RFID设置
  RFID.Set_transmission_Power(2600);
  RFID.Set_the_Select_mode();
  RFID.Delay(100);
  RFID.Readcallback();
  RFID.clean_data();
  
// Prompted to connect to UHF_RFID 提示连接UHF_RFID
  display.println("Please connect UHF_RFID to Port C");
  
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
  display.println("Please approach the RFID card you need to use");
  
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
//  canvas.println(comd);
//  RFID.clean_data();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 Query the card information once 查询一次卡的信息例子
 ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
 card = RFID.A_single_poll_of_instructions();
 if (card._ERROR.length() != 0){
    Serial.println(card._ERROR);
 }else{
    if(card._EPC.length() == 24){
      canvas.println("RSSI :" + card._RSSI);
      canvas.println("PC :" + card._PC);
      canvas.println("EPC :" + card._EPC);
      canvas.println("CRC :" + card._CRC);
      canvas.println(" ");
    }
  }
  RFID.clean_data(); //Empty the data after using it 使用完数据后要将数据清空


/*Other feature usage examples 其他功能使用例子*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Read multiple RFID cards at once 一次读取多张RFID卡
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
/*  cards = RFID.Multiple_polling_instructions(6);
 for (size_t i = 0; i < cards.len; i++)
 {
   if(cards.card[i]._EPC.length() == 24)
     {
        canvas.println("RSSI :" + cards.card[i]._RSSI);
        canvas.println("PC :" + cards.card[i]._PC);
        canvas.println("EPC :" + cards.card[i]._EPC);
        canvas.println("CRC :" + cards.card[i]._CRC);
      }
 }
 canvas.println(" ");  
 RFID.clean_data();
 */
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Used to get the SELECT parameter 用于获取Select参数
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//  Select = RFID.Get_the_select_parameter();
//  if(Select.Mask.length() != 0)
//  {
//    canvas.println("Mask :" + Select.Mask);
//    canvas.println("SelParam :" + Select.SelParam);
//    canvas.println("Ptr :" + Select.Ptr);
//    canvas.println("MaskLen :" + Select.MaskLen);
//    canvas.println("Truncate :" + Select.Truncate);
//    canvas.println(" ");
//  }
//    RFID.clean_data();

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Used to change the PSF bit of the NXP G2X label 用于改变 NXP G2X 标签的 PSF 位
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//  Cardinformation = RFID.NXP_Change_EAS(0x00000000);
//  if(Cardinformation._UL.length() != 0)
//  {
//    canvas.println("UL :" + Cardinformation._UL);
//    canvas.println("PC :" + Cardinformation._PC);
//    canvas.println("EPC :" + Cardinformation._EPC);
//    canvas.println("Parameter :" + Cardinformation._Parameter);
//    canvas.println("ErrorCode :" + Cardinformation._ErrorCode);
//    canvas.println("Error :" + Cardinformation._Error);
//    canvas.println("Data :" + Cardinformation._Data);
//    canvas.println("Successful :" + Cardinformation._Successful);
//    canvas.println(" ");
//   }
//    RFID.clean_data();
  

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Used to get the Query parameters 用于获取Query参数
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//  Query = RFID.Get_the_Query_parameter();
//  if(Query.QueryParameter.length() != 0)
//  {
//    canvas.println("QueryParameter :" + Query.QueryParameter);
//    canvas.println("DR :" + Query.DR);
//    canvas.println("M :" + Query.M);
//    canvas.println("TRext :" + Query.TRext);
//    canvas.println("Sel :" + Query.Sel);
//    canvas.println("Session :" + Query.Session);
//    canvas.println("Targetta :" + Query.Target);
//    canvas.println("Q :" + Query.Q);
//    canvas.println(" ");
//  }
//  RFID.clean_data();


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Used to get the Query parameters 用于读取接收解调器参数
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
//  Read = RFID.Read_receive_demodulator_parameters();
//  if(Read.Mixer_G.length()!= 0)
//  {
//    canvas.println("Mixer_G :" + Read.Mixer_G);
//    canvas.println("IF_G :" + Read.IF_G);
//    canvas.println("Thrd :" + Read.Thrd);
//    canvas.println(" ");
//  }
//  RFID.clean_data();
}
