#ifndef _RFID_COMMAND_H
#define _RFID_COMMAND_H

#include "Arduino.h"

#define UBYTE   uint8_t
#define UWORD   uint16_t
#define UDOUBLE uint32_t


struct CardpropertiesInfo
{
  String _RSSI;
  String _PC;
  String _EPC;
  String _CRC;
  String _ERROR;
};

struct ManyInfo
{
  int len;
  CardpropertiesInfo *card;
};

struct SelectInfo
{
  String Mask;
  String SelParam;
  String Ptr;
  String MaskLen;
  String Truncate;
};

struct CardInformationInfo
{
  String _UL;
  String _PC;
  String _EPC;
  String _Parameter;
  String _ErrorCode;
  String _Error;
  String _Data;
  String _Successful;
};

struct QueryInfo
{
  String  QueryParameter;
  String  DR;
  String  M;
  String  TRext;
  String  Sel;
  String  Session;
  String  Target;
  String  Q;
};

struct ReadInfo
{
  String Region;
  String Channel_Index;
  String Pow;
  String Mixer_G;
  String IF_G;
  String Thrd;

};

struct TestInfo
{
  String CH_L;
  String CH_H;
  String Data[20];
};




class UHF_RFID
{

  public:
    UBYTE _debug;
    
    void Sendcommand(UBYTE com_nub);
    void Send_the_modified_command();
    void Readcallback();
    void clean_data();
    UBYTE Return_to_convert(UBYTE mod);
    UDOUBLE String_to_command_frame(String str_epc);
    void Warningmessage(UBYTE nub);
    void Delay(unsigned long xms);
    UBYTE DelayScanwarning();
    void Copy_command_library( UBYTE com_nub);
    void Check_bit_accumulation();
    UWORD ToHex(UDOUBLE parameters, UBYTE MSB, UBYTE LSB);
    UBYTE Verify_the_return(UBYTE a[] , UBYTE size_v) ;
    UBYTE EPC_string_to_command_frame(String str_epc, UBYTE MSB, UBYTE LSB);
    CardInformationInfo Access_Password_is_incorrect();
    CardInformationInfo EPC_Gen2_error_code();
    CardInformationInfo Operation_is_successful();
    CardInformationInfo UI_PC_EPC();

    String Query_hardware_version();
    String Query_software_version();
    String Inquire_manufacturer();
    CardpropertiesInfo A_single_poll_of_instructions();
    ManyInfo Multiple_polling_instructions(UWORD cycle_nub);
    String Stop_the_multiple_poll_directive();
    String Set_the_select_parameter_directive(String str_epc, UBYTE SelParam = 0x01, UDOUBLE Ptr = 0x20, UBYTE MaskLen = 0x60, UBYTE Truncate = 0x0);
    SelectInfo Get_the_select_parameter();
    String Set_the_Select_mode(UBYTE Select_mode = 0x01);
    CardInformationInfo Read_the_label_data_store(UDOUBLE Access_Password , UBYTE MemBank = 0x03, UWORD SA = 0x0000, UWORD DL = 0x0002);
    CardInformationInfo The_label_store_writes_data(UDOUBLE Access_Password , UBYTE MemBank = 0x03, UWORD SA = 0x0000, UWORD DL = 0x0002, UDOUBLE DT = 0x12345678);
    CardInformationInfo Lock_the_label_data_store(UDOUBLE Access_Password  , UBYTE Action_nub = 1, UBYTE Action = 0b00);
    CardInformationInfo Inactivated_label(UDOUBLE Kill_Password);
    UBYTE Set_the_communication_baud_rate(UWORD Pow = 0x480);
    QueryInfo Get_the_Query_parameter();
    String set_the_Query_parameter(UBYTE Sel = 0b00, UBYTE Session = 0b00, UBYTE Target = 0b0, UWORD Q = 0b0100);
    String Set_up_work_area(UBYTE Region = 01);
    ReadInfo Read_working_area();
    String Set_up_working_channel(UBYTE CH_Index = 0x01);
    ReadInfo Read_working_channel();
    String Set_up_automatic_frequency_modulation(UWORD Parameter = 0xff);
    String Insert_working_channel(UBYTE CH_Cnt = 0x05, UBYTE CH_list_1 = 0x01, UBYTE CH_list_2 = 0x02, UBYTE CH_list_3 = 0x03, UBYTE CH_list_4 = 0x04, UBYTE CH_list_5 = 0x05);
    ReadInfo Read_transmitting_power();
    String Set_transmission_Power(UWORD Pow = 2000);
    String Sets_to_transmit_a_continuous_carrier(UWORD Parameter = 0xFF);
    ReadInfo Read_receive_demodulator_parameters();
    String Sets_the_receiv_demodulator_parameters(UBYTE Mixer_G = 0x03, UBYTE IF_G = 0x06, UWORD Thrd = 0x01B0);
    TestInfo Test_the_RF_input_blocking_signal();
    TestInfo Test_the_RSSI_input_signal();
    String Set_module_hibernation();
    String Set_Sleep_Time(UWORD Parameter = 0x00);
    String Set_the_ILDE_mode(UBYTE Enter = 0x00, UBYTE IDLE_Time = 0x00);
    CardInformationInfo NXP_ReadProtect_ResetReadProtect(UDOUBLE Access_Password, UBYTE ReadProtect = 0x00);
    CardInformationInfo NXP_Change_EAS(UDOUBLE Access_Password, UBYTE PSF = 0x01);
    String NXP_EAS_Alarm();
    CardInformationInfo NXP_Change_Config(UDOUBLE Access_Password, UWORD Config_Word = 0x0000);
    CardInformationInfo Impinj_Monza_QT(UDOUBLE Access_Password, UBYTE Read_Write = 0x00, UBYTE Persistence = 0x01, UWORD Payload = 0x4000);
    CardInformationInfo BlockPermalock(UDOUBLE Access_Password, UBYTE Read_Lock = 0x00, UBYTE MemBank = 0x03, UWORD BlockPtr = 0x00, UBYTE BlockRange = 0x01, UWORD Mask = 0x0700);


  private:
    CardpropertiesInfo *card;
    String DATA_Str_M5led = "";
    String DATA_Str_Serial = "";
    UBYTE DATA_I[4096] = {0,};  //Used to store distance data
    UWORD DATA_I_NUB = 0;
    UBYTE DATA_Interim_order[40] = {0,};
    UBYTE DATA_Interim_b = 0;
};



const UWORD RFID_cmdnub[39][26] =
{
  {0xBB, 0x00, 0x03, 0x00, 0x01, 0x00, 0x04, 0x7E,},       //0. Hardware version 0.硬件版本
  {0xBB, 0x00, 0x03, 0x00, 0x01, 0x01, 0x05, 0x7E,},       //1. Software version 1.软件版本
  {0xBB, 0x00, 0x03, 0x00, 0x01, 0x02, 0x06, 0x7E,},       //2.manufacturers  2.制造商
  {0xBB, 0x00, 0x22, 0x00, 0x00, 0x22, 0x7E,},             //3. Single polling instruction 3.单次轮询指令
  {0xBB, 0x00, 0x27, 0x00, 0x03, 0x22, 0x27, 0x10, 0x83, 0x7E,}, //4. Multiple polling instructions 4.多次轮询指令
  {0xBB, 0x00, 0x28, 0x00, 0x00, 0x28, 0x7E,},             //5. Stop multiple polling instructions 5.停止多次轮询指令
  { 0xBB, 0x00, 0x0C, 0x00, 0x13, 0x01, 0x00, 0x00, 0x00, 0x20,
    0x60, 0x00, 0x30, 0x75, 0x1F, 0xEB, 0x70, 0x5C, 0x59, 0x04,
    0xE3, 0xD5, 0x0D, 0x70, 0xAD, 0x7E,
  },                        //6. Set the SELECT parameter instruction 6.设置Select参数指令
  {0xBB, 0x00, 0x0B, 0x00, 0x00, 0x0B, 0x7E,},              //7. Get the SELECT parameter 7.获取Select参数
  {0xBB, 0x00, 0x12, 0x00, 0x01, 0x01, 0x14, 0x7E,},        //8. Set the SELECT mode 8.设置Select模式
  { 0xBB, 0x00, 0x39, 0x00, 0x09, 0x00, 0x00, 0xFF, 0xFF, 0x03,
    0x00, 0x00, 0x00, 0x02, 0x45, 0x7E,
  },                      //9. Read label data storage area 9.读标签数据储存区
  { 0xBB, 0x00, 0x49, 0x00, 0x0D, 0x00, 0x00, 0xFF, 0xFF, 0x03,
    0x00, 0x00, 0x00, 0x02, 0x12, 0x34, 0x56, 0x78, 0x6D, 0x7E,
  },   //10. Write the label data store 10.写标签数据存储区
  { 0xBB, 0x00, 0x82, 0x00, 0x07, 0x00, 0x00, 0xFF,
    0xFF, 0x02, 0x00, 0x80, 0x09, 0x7E,
  },                       //11. Lock the LOCK label data store 11.锁定Lock 标签数据存储区
  { 0xBB, 0x00, 0x65, 0x00, 0x04, 0x00, 0x00, 0xFF, 0xFF, 0x67,
    0x7E,
  },                                                //12. Inactivate the kill tag 12.灭活kill标签
  {0xBB, 0x00, 0x11, 0x00, 0x02, 0x00, 0xC0, 0xD3, 0x7E,}, //13. Set communication baud rate 13.设置通信波特率
  {0xBB, 0x00, 0x0D, 0x00, 0x00, 0x0D, 0x7E,},            //14. Get parameters related to the Query command 14.获取Query命令相关参数
  {0xBB, 0x00, 0x0E, 0x00, 0x02, 0x10, 0x20, 0x40, 0x7E,}, //15. Set the Query parameter 15.设置Query参数
  {0xBB, 0x00, 0x07, 0x00, 0x01, 0x01, 0x09, 0x7E,},      //16. Set up work area 16.设置工作地区
  {0xBB, 0x00, 0x08, 0x00, 0x00, 0x08, 0x7E,},            //17. Acquire work locations 17.获取工作地区
  {0xBB, 0x00, 0xAB, 0x00, 0x01, 0x01, 0xAC, 0x7E,},      //18. Set up working channel 18.设置工作信道
  {0xBB, 0x00, 0xAA, 0x00, 0x00, 0xAA, 0x7E,},            //19. Get the working channel 19.获取工作信道
  {0xBB, 0x00, 0xAD, 0x00, 0x01, 0xFF, 0xAD, 0x7E,},      //20. Set to automatic frequency hopping mode 20.设置为自动跳频模式
  { 0xBB, 0x00, 0xA9, 0x00, 0x06, 0x05, 0x01, 0x02,
    0x03, 0x04, 0x05, 0xC3, 0x7E,
  },                             //21. Insert the working channel 21.插入工作信道
  {0xBB, 0x00, 0xB7, 0x00, 0x00, 0xB7, 0x7E,},            //22. Acquire transmitting power 22.获取发射功率
  {0xBB, 0x00, 0xB6, 0x00, 0x02, 0x07, 0xD0, 0x8F, 0x7E,}, //23. Set the transmitting power 23.设置发射功率
  {0xBB, 0x00, 0xB0, 0x00, 0x01, 0xFF, 0xB0, 0x7E,},      //24. Set up transmitting continuous carrier 24.设置发射连续载波
  {0xBB, 0x00, 0xF1, 0x00, 0x00, 0xF1, 0x7E,},            //25. Gets the receiving demodulator parameters 25.获取接收解调器参数
  {0xBB, 0x00, 0xF0, 0x00, 0x04, 0x03, 0x06, 0x01, 0xB0, 0xAE, 0x7E,}, //26. Set the receiving demodulator parameters 26.设置接收解调器参数
  {0xBB, 0x00, 0xF2, 0x00, 0x00, 0xF2, 0x7E,},            //27. Test the RF input block signal 27.测试射频输入端阻塞信号
  {0xBB, 0x00, 0xF3, 0x00, 0x00, 0xF3, 0x7E,},            //28. Test the RSSI signal at the RF input 28.测试射频输入端 RSSI 信号
  {0x00},
  {0xBB, 0x00, 0x17, 0x00, 0x00, 0x17, 0x7E,},            //30. Module hibernation 30.模块休眠
  {0xBB, 0x00, 0x1D, 0x00, 0x01, 0x02, 0x20, 0x7E,},      //31. Idle hibernation time of module 31.模块空闲休眠时间
  {0xBB, 0x00, 0x04, 0x00, 0x03, 0x01, 0x01, 0x03, 0x0C, 0x7E,}, //32. The IDLE mode 32. IDLE 模式
  {0xBB, 0x00, 0xE1, 0x00, 0x05, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0xE4, 0x7E,}, //33.NXP G2X label supports ReadProtect/Reset ReadProtect command 33.NXP G2X 标签支持 ReadProtect/Reset ReadProtect 指令
  {0xBB, 0x00, 0xE3, 0x00, 0x05, 0x00, 0x00, 0xFF, 0xFF, 0x01, 0xE7, 0x7E,}, //34. The NXP G2X label supports the CHANGE EAS directive 34.NXP G2X 标签支持 Change EAS 指令
  {0xBB, 0x00, 0xE4, 0x00, 0x00, 0xE4, 0x7E,},            //35. The NXP G2X tag supports the EAS_ALARM directive 35.NXP G2X 标签支持 EAS_Alarm 指令
  {0xBB, 0x00, 0xE0, 0x00, 0x06, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00, 0xE4, 0x7E,}, //36.NXP G2X label 16bits config-word 36.NXP G2X 标签的 16bits Config-Word
  { 0xBB, 0x00, 0xE5, 0x00, 0x08, 0x00, 0x00, 0xFF,
    0xFF, 0x01, 0x01, 0x40, 0x00, 0x2D, 0x7E,
  },                   //37.Impinj Monza 4 Qt tags support Qt instructions 37.Impinj Monza 4 QT 标签支持 QT 指令
  { 0xBB, 0x00, 0xD3, 0x00, 0x0B, 0x00, 0x00, 0xFF,
    0xFF, 0x01, 0x03, 0x00, 0x00, 0x01, 0x07, 0x00, 0xE8, 0x7E,
  },   //38.The BlockPermalock directive permanently locks blocks of a user's Block 38.BlockPermalock 指令可以永久锁定用户区的某几个 Block

};








#endif
