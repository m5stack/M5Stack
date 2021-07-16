#include <M5Stack.h>
#include "RFID_command.h"

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

  RFID._debug = 1;
  Serial2.begin(115200, SERIAL_8N1, 16, 17);//16.17
  if (RFID._debug == 1)Serial.begin(115200, SERIAL_8N1, 21, 22);
  M5.Lcd.fillRect(0, 0, 340, 280, BLACK);

  RFID.Set_transmission_Power(2600);
  RFID.Set_the_Select_mode();
  RFID.Delay(100);
  RFID.Readcallback();
  RFID.clean_data();
}

void loop()
{
  M5.Lcd.fillCircle(310, 10, 6, GREEN);
  RFID.Delay(150);
  M5.Lcd.fillCircle(310, 10, 6, BLACK);
  RFID.Delay(150);

//  comd = RFID.Set_the_select_parameter_directive("30751FEB705C5904E3D50D70");
//  M5.Lcd.drawString(comd, 0, 0, 2);
//  RFID.Delay(1000);
//  RFID.clean_data();
//  M5.Lcd.fillRect(0, 0, 340, 280, BLACK);
//
  card = RFID.A_single_poll_of_instructions();
  if (card._ERROR.length() != 0)
  {
    Serial.print(card._ERROR);
  }
  else
  {
    M5.Lcd.drawString(card._RSSI, 0, 0, 2);
    M5.Lcd.drawString(card._PC, 0, 15, 2);
    M5.Lcd.drawString(card._EPC, 0, 30, 2);
    M5.Lcd.drawString(card._CRC, 0, 45, 2);
  }
  RFID.Delay(1000);
  RFID.clean_data();
  M5.Lcd.fillRect(0, 0, 340, 280, BLACK);
//
//  cards = RFID.Multiple_polling_instructions(6);
//  for (size_t i = 0; i < cards.len; i++)
//  {
//    M5.Lcd.drawString(cards.card[i]._RSSI, 200, 5 + i * 15, 2);
//    M5.Lcd.drawString(cards.card[i]._PC, 230, 5 + i * 15, 2);
//    M5.Lcd.drawString(cards.card[i]._EPC, 0, 5 + i * 15, 2);
//    M5.Lcd.drawString(cards.card[i]._CRC, 280, 5 + i * 15, 2);
//  }
//  RFID.Delay(1000);
//  RFID.clean_data();
//  M5.Lcd.fillRect(0, 0, 340, 280, BLACK);
//
//  Select = RFID.Get_the_select_parameter();
//  M5.Lcd.drawString(Select.Mask, 0, 0, 2);
//  M5.Lcd.drawString(Select.SelParam, 0, 15, 2);
//  M5.Lcd.drawString(Select.Ptr, 0, 30, 2);
//  M5.Lcd.drawString(Select.MaskLen, 0, 45, 2);
//  M5.Lcd.drawString(Select.Truncate, 0, 60, 2);
//  RFID.Delay(1000);
//  RFID.clean_data();
//  M5.Lcd.fillRect(0, 0, 340, 280, BLACK);
//
//  Cardinformation = RFID.NXP_Change_EAS(0x00000000);
//  M5.Lcd.drawString(Cardinformation._UL, 0, 0, 2);
//  M5.Lcd.drawString(Cardinformation._PC, 0, 15, 2);
//  M5.Lcd.drawString(Cardinformation._EPC, 0, 30, 2);
//  M5.Lcd.drawString(Cardinformation._Parameter, 0, 45, 2);
//  M5.Lcd.drawString(Cardinformation._ErrorCode, 0, 60, 2);
//  M5.Lcd.drawString(Cardinformation._Error, 0, 75, 2);
//  M5.Lcd.drawString(Cardinformation._Data, 0, 90, 2);
//  M5.Lcd.drawString(Cardinformation._Successful, 0, 105, 2);
//  RFID.Delay(1000);
//  RFID.clean_data();
//  M5.Lcd.fillRect(0, 0, 340, 280, BLACK);
//
//  Query = RFID.Get_the_Query_parameter();
//  M5.Lcd.drawString(Query.QueryParameter, 0, 0, 2);
//  M5.Lcd.drawString(Query.DR, 0, 15, 2);
//  M5.Lcd.drawString(Query.M, 0, 30, 2);
//  M5.Lcd.drawString(Query.TRext, 0, 45, 2);
//  M5.Lcd.drawString(Query.Sel, 0, 60, 2);
//  M5.Lcd.drawString(Query.Session, 0, 75, 2);
//  M5.Lcd.drawString(Query.Target, 0, 90, 2);
//  M5.Lcd.drawString(Query.Q, 0, 105, 2);
//  RFID.Delay(1000);
//  RFID.clean_data();
//  M5.Lcd.fillRect(0, 0, 340, 280, BLACK);
//
//
//  Read = RFID.Read_receive_demodulator_parameters();
//  M5.Lcd.drawString(Read.Region, 0, 0, 2);
//  M5.Lcd.drawString(Read.Channel_Index, 0, 15, 2);
//  M5.Lcd.drawString(Read.Pow, 0, 30, 2);
//  M5.Lcd.drawString(Read.Mixer_G, 0, 45, 2);
//  M5.Lcd.drawString(Read.IF_G, 0, 60, 2);
//  M5.Lcd.drawString(Read.Thrd, 0, 75, 2);
//  RFID.Delay(1000);
//  RFID.clean_data();
//  M5.Lcd.fillRect(0, 0, 340, 280, BLACK);
//
//  Test = RFID.Test_the_RSSI_input_signal();
//  M5.Lcd.drawString(Test.CH_L, 0, 0, 2);
//  M5.Lcd.drawString(Test.CH_H, 0, 15, 2);
//  for (size_t i = 0; i < 20; i++)
//  {
//    if (i < 10)
//    {
//      M5.Lcd.drawString(Test.Data[i], i * 20, 30, 2);
//    }
//    else
//    {
//      M5.Lcd.drawString(Test.Data[i], (i - 10) * 20, 45, 2);
//    }
//  }
//  RFID.Delay(1000);
//  RFID.clean_data();
//  M5.Lcd.fillRect(0, 0, 340, 280, BLACK);


}
