#include <Arduino.h>
#include "RFID_command.h"
#include <string>


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to ask about the hardware version
  用于询问硬件版本
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

String UHF_RFID::Query_hardware_version()
{
  Sendcommand(0);
  Delay(20);
  Readcallback();

  if (DelayScanwarning())
  {
    return DATA_Str_M5led;
  }
  else
  {
    Return_to_convert(0);

    return DATA_Str_M5led.substring(6, 21);
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to ask about software versions
  用于询问软件版本
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
String UHF_RFID::Query_software_version()
{
  Sendcommand(1);
  Delay(50);
  Readcallback();

  if (DelayScanwarning())
  {
    return DATA_Str_M5led;
  }
  else
  {
    Return_to_convert(0);
    Serial.print(DATA_Str_M5led);
    return DATA_Str_M5led.substring(6, 12);
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to ask for RFID manufacturer information
  用于询问RFID制造商信息
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
String UHF_RFID::Inquire_manufacturer()
{
  Sendcommand(2);
  Delay(20);
  Readcallback();

  if (DelayScanwarning())
  {
    return DATA_Str_M5led;
  }
  else
  {
    Return_to_convert(0);

    return DATA_Str_M5led.substring(6, 13);
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used for a single polling instruction
  用于单次轮询指令
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
CardpropertiesInfo UHF_RFID::A_single_poll_of_instructions()
{
  UBYTE a[5] = {0xBB, 0x02, 0x22, 0x00, 0x11};
  CardpropertiesInfo card;

  Sendcommand(3);
  Delay(50);
  Readcallback();

  if (DelayScanwarning())
  {
    card._RSSI = "";
    card._PC = "";
    card._EPC = "";
    card._CRC = "";
    card._ERROR = DATA_Str_Serial;
    return card;
  }
  else
  {
    if (Verify_the_return(a, 5))
    {
      Return_to_convert(1);

      card._RSSI = DATA_Str_M5led.substring(10, 12);
      card._PC = DATA_Str_M5led.substring(12, 16);
      card._EPC = DATA_Str_M5led.substring(16, 40);
      card._CRC = DATA_Str_M5led.substring(40, 44);
      card._ERROR = "";

      return card;
    }
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Used to poll instructions multiple times
  Cycle_nub: It is best to do between 0 and 100 reps, with a maximum of 65535 reps but too many reps will cause memory overflow

  用于多次轮询指令
  cycle_nub: 最好在0-100次内，最大是65535次但是次数太多会造成内存溢出
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
ManyInfo UHF_RFID::Multiple_polling_instructions(UWORD cycle_nub)
{
  free(this->card);

  UWORD a = 0x00;
  UWORD b = 0x00;
  UBYTE d = 0;
  UWORD i_nub = 0;
  String data_mp = "";
  String data_tpy = "";

  Copy_command_library(4);
  ToHex(cycle_nub, 6, 7);
  Check_bit_accumulation();
  Send_the_modified_command();

  for (int i = 0; i != cycle_nub; i++)
  {
    Readcallback();

    i_nub = DATA_I_NUB + 1;

    Return_to_convert(1);

    while (i_nub > 0)
    {
      if (DATA_I[i_nub - 1] == 0x7E)
      {
        b = i_nub - 1;

      }
      if (DATA_I[i_nub - 1] == 0xBB && DATA_I[i_nub] == 0x02)
      {
        a = i_nub - 1;

      }
      if (b - a == 0x17   )
      {
        if (data_mp.indexOf(DATA_Str_M5led.substring(a * 2 + 16, a * 2 + 40)) == -1)
        {
          data_tpy = DATA_Str_M5led.substring(a * 2 + 10, a * 2 + 44);
          if (data_tpy.indexOf("bb01") == -1 )   //To prevent the mixing of EPC errors, should only be limited to the beginning of 0xbb command 防止混入错误的EPC，应bb仅限命令开头
          {
            data_mp += DATA_Str_M5led.substring(a * 2 + 10, a * 2 + 44);
            d += 1;
          }
        }
        a = 0; b = 0;
      }
      i_nub--;
    }
    a = 0; b = 0;
  }

  this->card = (CardpropertiesInfo *)calloc(d, sizeof(CardpropertiesInfo));

  for (int i = 0; i != d; i++ )
  {
    this->card[i]._RSSI = data_mp.substring(i * 34 + 0, i * 34 + 2);
    this->card[i]._PC = data_mp.substring(i * 34 + 2, i * 34 + 6);
    this->card[i]._EPC = data_mp.substring(i * 34 + 6, i * 34 + 30);
    this->card[i]._CRC = data_mp.substring(i * 34 + 30, i * 34 + 34);
    this->card[i]._ERROR = "";
  }
  ManyInfo Cards = {d, this->card};

  return Cards;

}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to stop multiple polling instructions
  用于停止多次轮询指令
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
String UHF_RFID::Stop_the_multiple_poll_directive()
{
  UBYTE a[8] = {0xBB, 0x01, 0x28, 0x00, 0x01, 0x00, 0x2A, 0x7E};

  Sendcommand(5);
  Delay(100);
  Readcallback();

  if (DelayScanwarning())
  {
    return "";
  }
  else
  {

    if (Verify_the_return(a, 8))
    {
      return "Stop OK";
    }
    else
    {
      return "Stop Unsuccess";
    }
  }
}
/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Use to set the SELECT parameter instruction
  Str_epc: Example of the EPC string: "30751FEB705C5904E3D50D70"
  SelParam: SelParam contains 1 Byte, in which Target contains the highest 3 bits, Action contains the middle 3 bits,
  Membank occupies the last 2 bits.
  Membank means as follows:
  2 'B00: Label RFU data store
  2 'B01: Label EPC data store
  2 'B10: Tag TID data store
  2 'b11: label User data store
    See the EPC Gen2 protocol for the full meaning of Target and Action.
  PTR: 0x00000020(in bits, not words) starts from the EPC storage bit
  Mask: Length Masklen: 0x60(6 words, 96bits)
  Truncate: 0x00(0x00 is Disable truncation, 0x80 is Enable truncation)


  用于设置Select参数指令

  str_epc：EPC的字符串例："30751FEB705C5904E3D50D70"
  SelParam：SelParam 共 1 个 Byte，其中 Target 占最高 3 个 bits，Action 占中间 3 个 bits，
  MemBank 占最后 2 个 bits。
            MemBank 含义如下：
              2’b00:	标签 RFU 数据存储区
              2’b01:	标签 EPC 数据存储区
              2’b10:	标签 TID 数据存储区
              2’b11:	标签 User 数据存储区
                Target 和 Action 详细含义请参见 EPC Gen2 协议。
  Ptr:	0x00000020(以 bit 为单位，非 word) 从 EPC 存储位开始
  Mask： 长度 MaskLen:	0x60(6 个 word，96bits)
  Truncate:	0x00(0x00 是 Disable truncation，0x80 是 Enable truncation)
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
String UHF_RFID::Set_the_select_parameter_directive(String str_epc, UBYTE SelParam, UDOUBLE Ptr, UBYTE MaskLen, UBYTE Truncate)
{
  UBYTE b[8] = {0xBB, 0x01, 0x0C, 0x00, 0x01, 0x00, 0x0E, 0x7E};

  Copy_command_library(6);
  ToHex(SelParam, 5, 5);//0x01
  ToHex(Ptr, 6, 9);//0x20
  ToHex(MaskLen, 10, 10);//0x30
  ToHex(Truncate, 11, 11);//0x00

  EPC_string_to_command_frame(str_epc, 12, 23);

  Check_bit_accumulation();
  Send_the_modified_command();
  Delay(100);
  Readcallback();


  if (DelayScanwarning())
  {
    return "";
  }
  else
  {
    if (Verify_the_return(b, 8))
    {
      return "Set the select OK";
    }
    else
    {
      return "Set the select Unsuccess";
    }
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to get the SELECT parameter
  用于获取Select参数
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
SelectInfo UHF_RFID::Get_the_select_parameter()
{
  UBYTE a[4] = {0xBB, 0x01, 0x0B, 0x00};

  SelectInfo Select;

  Sendcommand(7);
  Delay(100);
  Readcallback();


  if (DelayScanwarning())
  {
    Select.Mask = "";
    Select.SelParam = "";
    Select.Ptr = "";
    Select.MaskLen = "";
    Select.Truncate = "";

    return Select;
  }
  else
  {
    if (Verify_the_return(a, 4))
    {
      Return_to_convert(1);

      Select.Mask = DATA_Str_M5led.substring( 24,  48);
      Select.SelParam = DATA_Str_M5led.substring( 10,  12);
      Select.Ptr = DATA_Str_M5led.substring( 12,  20);
      Select.MaskLen = DATA_Str_M5led.substring( 20,  22);
      Select.Truncate = DATA_Str_M5led.substring( 22,  24);

      return Select;
    }

  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to set the SELECT mode
  Select Mode Mode
  0x00: The Select instruction is pre-sent to Select a particular label before any operation on the label.
  0x01: The SELECT instruction is not sent until the label is operated on.
  0x02: Send a SELECT instruction only before a label operation other than polling Inventory, as in
    Read, Write, Lock, and Kill are preceded by Select for a specific tag.

  用于设置Select模式

  Select 模式 Mode 含义：
    0x00:	在对标签的所有操作之前都预先发送 Select 指令选取特定的标签。
    0x01:	在对标签操作之前不发送 Select 指令。
    0x02:	仅对除轮询 Inventory 之外的标签操作之前发送 Select 指令，如在
          Read，Write，Lock，Kill 之前先通过 Select 选取特定的标签。
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
String UHF_RFID::Set_the_Select_mode(UBYTE Select_mode)
{
  UBYTE b[8] = {0xBB, 0x01, 0x12, 0x00, 0x01, 0x00, 0x14, 0x7E};

  Copy_command_library(8);
  ToHex(Select_mode, 5, 5);
  Check_bit_accumulation();
  Send_the_modified_command();
  Delay(20);
  Readcallback();


  if (DelayScanwarning())
  {
    return "";
  }
  else
  {

    if (Verify_the_return(b, 8))
    {
      return "Set the select mode OK";
    }
    else
    {
      return "Set the select mode Unsuccess";
    }
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   For a single tag, read the tag data store

  Tag data store MemBank: 0x00(RFU area) 0x01(EPC area) 0x02(TID area) 0x03(USER area)
  Read label data area address offset SA: 0x0000
  Read label data area address length DL: 0x0002

  用于对单个标签，读取标签数据存储区

  标签数据存储区 MemBank:  0x00(RFU 区) 0x01(EPC 区) 0x02(TID 区) 0x03(User 区)
  读标签数据区地址偏移 SA:  0x0000
  读标签数据区地址长度 DL:  0x0002

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
CardInformationInfo UHF_RFID::Read_the_label_data_store(UDOUBLE Access_Password, UBYTE MemBank, UWORD SA, UWORD DL)
{
  UBYTE b[6] = {0xBB, 0x01, 0x39, 0x00, 0x13, 0x0E};
  UBYTE e;

  CardInformationInfo Cardinformation;

  Copy_command_library(9);

  ToHex(Access_Password, 5, 8);
  ToHex(MemBank, 9, 9);
  ToHex(SA, 10, 11);
  ToHex(DL, 12, 13);

  Check_bit_accumulation();
  Send_the_modified_command();
  Delay(50);
  Readcallback();


  if (e = DelayScanwarning())
  {

    if (e == 0x16)
    {
      Return_to_convert(1);

      Cardinformation = Access_Password_is_incorrect();
    }

    else if (e / 0x10 == 0xA )
    {
      Return_to_convert(1);

      Cardinformation = EPC_Gen2_error_code();
    }

    return Cardinformation;
  }
  else
  {
    if (Verify_the_return(b, 6))
    {
      Return_to_convert(1);

      Cardinformation = UI_PC_EPC();
      Cardinformation._Data = DATA_Str_M5led.substring( 40,  48);


      return Cardinformation;
    }


  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   For a single tag, write to the tag data store Memory Bank at the specified address and length.
  The length of the data to be written to the label data store, DT, should not exceed 32 words, or 64 bytes /512Bit bits.

  Label data store MEMBANK: 0x03
  Tag data area address offset SA: 0x0000
  Data length DL: 0x0002
  Write data DT: 0x12345678

  用于对单个标签，写入标签数据存储区 Memory Bank 中指定地址和长度的数据。
  写入标签数据存储区的数据长度 DT 应不超过 32 个 word，即 64Byte 字节/512Bit 位.
                                  改写密码Access_Password
  标签数据存储区 MemBank:  0x03   (0x00)
  标签数据区地址偏移 SA:  0x0000  (0x02)
  数据长度 DL:  0x0002            (0x02)
  写入数据 DT:  0x12345678
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
CardInformationInfo UHF_RFID::The_label_store_writes_data(UDOUBLE Access_Password, UBYTE MemBank, UWORD SA, UWORD DL, UDOUBLE DT)
{

  CardInformationInfo Cardinformation;

  UBYTE b[6] = {0xBB, 0x01, 0x49, 0x00, 0x10, 0x0E};
  UBYTE e;

  Copy_command_library(10);

  ToHex(Access_Password, 5, 8);
  ToHex(MemBank, 9, 9);
  ToHex(SA, 10, 11);
  ToHex(DL, 12, 13);
  ToHex(DT, 14, 17);

  Check_bit_accumulation();
  Send_the_modified_command();
  Delay(50);
  Readcallback();

  if (e = DelayScanwarning())
  {
    if (e == 0x16)
    {
      Return_to_convert(1);

      Cardinformation = Access_Password_is_incorrect();
    }

    else if (e / 0x10 == 0xB )
    {
      Return_to_convert(1);

      Cardinformation = EPC_Gen2_error_code();
    }


    return Cardinformation;
  }
  else
  {
    if (Verify_the_return(b, 6))
    {
      Return_to_convert(1);

      Cardinformation = Operation_is_successful();
      Cardinformation._Successful = "Write to successful";

      return Cardinformation;
    }

  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to Lock the LOCK label data store
  For a single tag, Lock or Unlock the data store for that tag.
  ACTION_NUB has 0: kill 1: Access 2: EPC 3: TID 4: User
  Action has 2 bits, 00~11, corresponding to open, permanent open, lock, permanent lock

  用于锁定Lock标签数据存储区

  对单个标签，锁定 Lock 或者解锁 Unlock 该标签的数据存储区。
  Action_nub 有 0：kill  1：Access  2：EPC  3：TID  4：User
  Action 有 2 bits，00~11，依次对应为 开放，永久开放，锁定，永久锁定
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
CardInformationInfo UHF_RFID::Lock_the_label_data_store(UDOUBLE Access_Password , UBYTE Action_nub, UBYTE Action )
{
  UDOUBLE Action_Data = 0;
  UBYTE a[6] = {0xBB, 0x01, 0x82, 0x00, 0x10, 0x0E};
  UBYTE e = 0;

  CardInformationInfo Cardinformation;

  for (UBYTE i = 0; i < 10; i++)
  {
    if (i == Action_nub)
    {
      Action_Data = Action_Data * 0b100 + 0b10;
    }
    else if (i == Action_nub + 5)
    {
      Action_Data = Action_Data * 0b100 + Action;
    }
    else
    {
      Action_Data = Action_Data * 0b100;
    }
  }

  Copy_command_library(11);

  ToHex(Access_Password, 5, 8);
  ToHex(Action_Data, 9, 11);


  Check_bit_accumulation();
  Send_the_modified_command();
  Delay(50);
  Readcallback();

  if (e = DelayScanwarning())
  {
    if (e == 0x16)
    {
      Return_to_convert(1);
      Cardinformation = Access_Password_is_incorrect();
    }

    else if (e / 0x10 == 0xC )
    {
      Return_to_convert(1);
      Cardinformation = EPC_Gen2_error_code();
    }
    return Cardinformation;
  }
  else
  {

    if (Verify_the_return(a, 6))
    {

      Return_to_convert(1);

      Cardinformation = Operation_is_successful();
      if (DATA_Str_M5led.substring( 40,  42) == "00")
      {
        if (Action == 0b01 || Action == 0b00)
        {
          Cardinformation._Successful = "unlock successful";
        }
        else if (Action == 0b10 || Action == 0b11)
        {
          Cardinformation._Successful = "Lock successful";
        }
      }
      return Cardinformation;
    }


  }

}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to inactivate the Kill tag
  This directive should be preceded by setting the SELECT parameter to Select the specified label for the Kill inactivation operation.The inactivation of a single label.

  用于灭活Kill标签

  这条指令之前应先设置 Select 参数，以便选择指定的标签进行灭活 Kill 操作。对单标签的灭活操作。
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
CardInformationInfo UHF_RFID::Inactivated_label(UDOUBLE Kill_Password)
{
  UBYTE a[6] = {0xBB, 0x01, 0x65, 0x00, 0x10, 0x0E};
  UBYTE e = 0;

  CardInformationInfo Cardinformation;

  Copy_command_library(12);

  ToHex(Kill_Password, 5, 8);

  Check_bit_accumulation();
  Send_the_modified_command();

  Readcallback();

  if (e = DelayScanwarning())
  {
    if (e / 0x10 == 0xD )
    {
      Return_to_convert(1);
      Cardinformation = EPC_Gen2_error_code();
    }
    return Cardinformation;
  }
  else
  {

    if (Verify_the_return(a, 6))
    {
      Return_to_convert(1);

      Cardinformation = Operation_is_successful();
      if (DATA_Str_M5led.substring( 40,  42) == "00")
      {
        Cardinformation._Successful = "Kill label successful";
      }
      return Cardinformation;
    }

  }

}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to set the communication baud rate
  Response frame defines that this instruction has no response frame.After the reader executes the command to set the communication baud rate, the reader communicates with the host computer using the new baud rate.
  The host machine needs to reconnect the reader with the new baud rate.
  Power parameter PoW: 0x00C0(in hexadecimal system of baud rate /100, such as 19200, that is 19200/100=192=0xC0)

  用于设置通信波特率

  响应帧定义 该指令没有响应帧。读写器执行完设置通信波特率指令后，读写器就会用新的波特率与上位机通信，
  上位机需 要用新的波特率重新连接读写器。

  功率参数 Pow:  0x00C0(波特率/100 的 16 进制，比如 19200，就是 19200/100=192=0xC0)
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
UBYTE UHF_RFID::Set_the_communication_baud_rate(UWORD Pow)
{

  Copy_command_library(13);

  ToHex(Pow, 5, 6);

  Check_bit_accumulation();
  Send_the_modified_command();

}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to get the Query parameters

  用于获取Query参数
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

QueryInfo UHF_RFID::Get_the_Query_parameter()
{
  String QueryStr = "";
  UBYTE a[5] = {0xBB, 0x01, 0x0D, 0x00, 0x02};
  UBYTE c = 0;
  UBYTE d = 0;
  UBYTE e = 0;
  UBYTE i = 0;
  UBYTE x = 0;

  QueryInfo Query;

  Sendcommand(14);

  Readcallback();

  if (DelayScanwarning())
  {
    Query.QueryParameter = "";
    Query.DR = "";
    Query.M = "";
    Query.TRext = "";
    Query.Sel = "";
    Query.Session = "";
    Query.Target = "";
    Query.Q = "";

    return Query;
  }
  else
  {
    if (Verify_the_return(a, 5))
    {
      for (int b = 5; b <= 6; b++)
      {
        i = 0;
        d = DATA_I[b];

        while ( i < 8)
        {
          c = d % 0b10;
          if (c != 0b0)
          {
            x = i;
          }
          d = d / 0b10;
          i++;

        }
        for (i = 1; i < 8 - x; i++)
        {
          QueryStr += "0";
        }
        QueryStr += String(DATA_I[b], BIN);
      }
      Return_to_convert(1);

      Query.QueryParameter = DATA_Str_M5led.substring(10, 14);
      Query.DR = QueryStr.substring(0, 1);
      Query.M = QueryStr.substring(1, 3);
      Query.TRext = QueryStr.substring(3, 4);
      Query.Sel = QueryStr.substring(4, 6);
      Query.Session = QueryStr.substring(6, 8);
      Query.Target = QueryStr.substring(8, 9);
      Query.Q = QueryStr.substring(9, 13);

      return Query;
    }

  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Use to set the Query parameter
  DR (1 -) : DR = 8 (1 'b0), DR = 64/3 b1 (1').Only DR=8 modes are supported
  M (2 -) : M = 1 (2 'b00), M = 2 (2' b01), M = 4 (2 'b10), M = 8 bl1 (2 ").Only M=1 modes are supported
  Trext (1 bit): No Pilot Tone (1 'B0), Use Pilot Tone (1' B1).Only Use Pilot Tone (1 'B1) mode is supported
  Sel (2 -) : ALL (2 'b01 b00/2 "), ~ SL (2' b10), SL (2 'b11)
  The Session (2 -) : S0 b00 (2 '), S1 (2 'b01), S2 (2' b10), S3 (2 'b11)
  The Target (1 -) : A (1 'b0), B (1' b1)
  Q: (bit 4) 4 'b0000-4' b1111

  用于设置Query参数

  DR(1 bit):	DR=8(1’b0), DR=64/3(1’b1). 只支持 DR=8 的模式
  M(2 bit):	M=1(2’b00), M=2(2’b01), M=4(2’b10), M=8(2’b11). 只支持 M=1 的模式
  TRext(1 bit):	No pilot tone(1’b0), Use pilot tone(1’b1). 只支持 Use pilot tone(1’b1)模式
  Sel(2 bit):	ALL(2’b00/2’b01), ~SL(2’b10), SL(2’b11)
  Session(2 bit):	S0(2’b00), S1(2’b01), S2(2’b10), S3(2’b11)
  Target(1 bit):	A(1’b0), B(1’b1)
  Q(4 bit):	4’b0000-4’b1111
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

String UHF_RFID::set_the_Query_parameter(UBYTE Sel, UBYTE Session, UBYTE Target, UWORD Q)
{
  UDOUBLE para = 0b1;
  UBYTE a[8] = {0xBB, 0x01, 0x0E, 0x00, 0x01, 0x00, 0x10,};


  Copy_command_library(15);

  para =  para * 0b100 + Sel;
  para =  para * 0b100 + Session;
  para =  para * 0b10 + Target;
  para =  para * 0b10000 + Q;
  para =  para * 0b1000;

  ToHex(para, 5, 6);

  Check_bit_accumulation();
  Send_the_modified_command();
  Delay(50);

  Readcallback();

  if (DelayScanwarning())
  {
    return "";
  }
  else
  {
    if (Verify_the_return(a, 7))
    {
      return "Set the Query OK ";
    }
    else
    {
      return "Set the Query Unsuccess ";
    }


  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to set the reader work area
  China's 900 MHZ 0 x01
  China's 800 MHZ 0 x04
  The 0 x02
  European 0 x03
  South Korea 0 x06

  用于设置读写器工作地区

  中国 900MHz	0x01
  中国 800MHz	0x04
  美国	0x02
  欧洲	0x03
  韩国	0x06
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
String UHF_RFID::Set_up_work_area(UBYTE Region)
{
  UBYTE a[8] = {0xBB, 0x01, 0x07, 0x00, 0x01, 0x00, 0x09, 0x7E};

  Copy_command_library(16);

  ToHex(Region, 6, 6);

  Check_bit_accumulation();
  Send_the_modified_command();
  Delay(20);
  Readcallback();

  if (DelayScanwarning())
  {
    return "";
  }
  else
  {

    if (Verify_the_return(a, 8))
    {
      return "Set up work area OK ";
    }
    return "";
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Used to read the reader work area
    China's 900 MHZ 0 x01
    China's 800 MHZ 0 x04
    The 0 x02
    European 0 x03
    South Korea 0 x06

  用于读取读写器工作地区
  中国 900MHz  0x01
  中国 800MHz 0x04
  美国  0x02
  欧洲  0x03
  韩国  0x06
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
ReadInfo UHF_RFID::Read_working_area()
{
  UBYTE a[5] = {0xBB, 0x01, 0x08, 0x00, 0x01};

  ReadInfo Read;

  Sendcommand(17);
  Delay(20);
  Readcallback();

  if (DelayScanwarning())
  {
    Read.Region = "";
    Read.Channel_Index = "";
    Read.Pow = "";
    Read.Mixer_G =  "";
    Read.IF_G =  "";
    Read.Thrd =  "";
    return Read;
  }
  else
  {
    if (Verify_the_return(a, 5))
    {
      Return_to_convert(1);
      Read.Region = DATA_Str_M5led.substring(10, 12);
      Read.Channel_Index = "";
      Read.Pow = "";
      Read.Mixer_G =  "";
      Read.IF_G =  "";
      Read.Thrd =  "";

      return Read;
    }

  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  Used to set the reader working channel
  China 900MHz channel parameter calculation formula, FREQ_CH is the channel frequency:
  CH_Index = (Freq_CH to 920.125 M) / 0.25 M
  China 800MHz channel parameter calculation formula, Freq_CH is the channel frequency:
  CH_Index = (Freq_CH to 840.125 M) / 0.25 M
  American channel parameter calculation formula, Freq_CH is the channel frequency:
  CH_Index = (Freq_CH to 902.25 M) / 0.5 M
  European channel parameter calculation formula, Freq_CH is the channel frequency:
  CH_Index = (Freq_CH to 865.1 M) / 0.2 M
  Korean channel parameter calculation formula, Freq_CH is the channel frequency:
  CH_Index = (Freq_CH to 917.1 M) / 0.2 M
  If it is a Chinese 900MHz frequency band, set the reader working channel 920.375MHz, CH Index = 0x01

  用于设置读写器工作信道
  中国 900MHz 信道参数计算公式，Freq_CH 为信道频率：
  CH_Index = (Freq_CH-920.125M)/0.25M

  中国 800MHz 信道参数计算公式，Freq_CH 为信道频率：
  CH_Index = (Freq_CH-840.125M)/0.25M

  美国信道参数计算公式，Freq_CH 为信道频率：
  CH_Index = (Freq_CH-902.25M)/0.5M

  欧洲信道参数计算公式，Freq_CH 为信道频率：
  CH_Index = (Freq_CH-865.1M)/0.2M

  韩国信道参数计算公式，Freq_CH 为信道频率：
  CH_Index = (Freq_CH-917.1M)/0.2M
  如果是中国 900MHz 频段，设置读写器工作信道 920.375MHz，CH Index = 0x01
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
String UHF_RFID::Set_up_working_channel(UBYTE CH_Index)
{
  UBYTE a[8] = {0xBB, 0x01, 0xAB, 0x00, 0x01, 0x00, 0xAD, 0x7E};

  Copy_command_library(18);

  ToHex(CH_Index, 6, 6);

  Check_bit_accumulation();
  Send_the_modified_command();
  Delay(20);
  Readcallback();

  if (DelayScanwarning())
  {
    return "";
  }
  else
  {

    if (Verify_the_return(a, 8))
    {

      return "Set up working channel OK ";
    }
    return "";
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to read the reader working channel
  China 900MHz channel parameter calculation formula, FREQ_CH is the channel frequency:
  FREQ_CH = CH_INDEX * 0.25M + 920.125M
  China 800MHz channel parameter calculation formula, Freq_CH is the channel frequency:
  FREQ_CH = CH_INDEX * 0.25M + 840.125M
  American channel parameter calculation formula, Freq_CH is the channel frequency:
  FREQ_CH = CH_INDEX * 0.5m + 902.25m
  European channel parameter calculation formula, Freq_CH is the channel frequency:
  FREQ_CH = CH_INDEX * 0.2m + 865.1m
  Korean channel parameter calculation formula, Freq_CH is the channel frequency:
  FREQ_CH = CH_INDEX * 0.2m + 917.1m

  用于读取读写器工作信道

  中国 900MHz 信道参数计算公式，Freq_CH 为信道频率：
  Freq_CH = CH_Index * 0.25M + 920.125M

  中国 800MHz 信道参数计算公式，Freq_CH 为信道频率：
  Freq_CH = CH_Index * 0.25M + 840.125M

  美国信道参数计算公式，Freq_CH 为信道频率：
  Freq_CH = CH_Index * 0.5M + 902.25M

  欧洲信道参数计算公式，Freq_CH 为信道频率：
  Freq_CH = CH_Index * 0.2M + 865.1M

  韩国信道参数计算公式，Freq_CH 为信道频率：
  Freq_CH = CH_Index * 0.2M + 917.1M

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
ReadInfo UHF_RFID::Read_working_channel()
{
  UBYTE a[5] = {0xBB, 0x01, 0xAA, 0x00, 0x01};

  ReadInfo Read;

  Sendcommand(19);
  Delay(20);
  Readcallback();

  if (DelayScanwarning())
  {
    Read.Region = "";
    Read.Channel_Index = "";
    Read.Pow = "";
    Read.Mixer_G =  "";
    Read.IF_G =  "";
    Read.Thrd =  "";
    return Read;
  }
  else
  {
    if (Verify_the_return(a, 5))
    {
      Return_to_convert(1);
      Read.Region = "";
      Read.Channel_Index = DATA_Str_M5led.substring(10, 12);
      Read.Pow = "";
      Read.Mixer_G =  "";
      Read.IF_G =  "";
      Read.Thrd =  "";
      return Read;
    }

  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to set up automatic frequency modulation
  Set the command frame definition to auto-hopping mode or cancel auto-hopping mode.In auto-hopping mode, if the user executes the insert working channel instruction,
  The reader randomly selects the channel hopping from the list of channels set by the user, otherwise it randomly selects the channel hopping from the internal preset list of channels.
  Parameter: 0xFF(0xFF means to set automatic frequency hopping, 0x00 means to cancel automatic frequency hopping)

  用于设置自动调频

  命令帧定义 设置为自动跳频模式或者取消自动跳频模式。在自动跳频模式下，如果用户执行了插入工作信道指令,
  则读写 器从用户设置的信道列表中随机选择信道跳频，否则按照内部预设的信道列表随机选择信道跳频。

  Parameter:	0xFF(0xFF 为设置自动跳频，0x00 为取消自动跳频)
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
String UHF_RFID::Set_up_automatic_frequency_modulation(UWORD Parameter)
{
  UBYTE a[8] = {0xBB, 0x01, 0xAD, 0x00, 0x01, 0x00, 0xAF, 0x7E};

  Copy_command_library(20);

  ToHex(Parameter, 6, 6);

  Check_bit_accumulation();
  Send_the_modified_command();
  Delay(20);
  Readcallback();

  if (DelayScanwarning())
  {
    return "";
  }
  else
  {

    if (Verify_the_return(a, 8))
    {
      if (Parameter == 0xff)
      {
        return "Set up aut FM Up ";
      }
      else if (Parameter == 0x00)
      {
        return "Set up aut FM Down";
      }

    }
    return "";
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used for plugging into a working channel
  Inserting a working channel allows the user to set the list of frequency hopping channels. After executing this command, the reader will randomly select from the list of channels set by the user
  Selective channel frequency hopping
  CH CNT: 0x05 (if 0, the list of frequency hopping channels is cleared, and the reader hops frequency randomly from all available channels)
  CH list: channel list (with CH Index): 0x01 0x02 0x03 0x04 0x05

  用于插入工作信道

  插入工作信道可以让用户自主设置跳频的信道列表，执行此命令后，读写器将从用户设置的信道列表中随机选
  择信道跳频

  CH Cnt:	0x05 (如果为 0，则清除跳频信道列表，读写器从全部可用信道中随机跳频)
  CH list : 信道列表(用 CH Index 表示):	0x01 0x02 0x03 0x04 0x05
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
String UHF_RFID::Insert_working_channel(UBYTE CH_Cnt, UBYTE CH_list_1, UBYTE CH_list_2, UBYTE CH_list_3, UBYTE CH_list_4, UBYTE CH_list_5)
{
  UBYTE a[8] = {0xBB, 0x01, 0xA9, 0x00, 0x01, 0x00, 0xAB, 0x7E};

  Copy_command_library(21);

  ToHex(CH_Cnt, 5, 5);
  ToHex(CH_list_1, 6, 6);
  ToHex(CH_list_2, 7, 7);
  ToHex(CH_list_3, 8, 8);
  ToHex(CH_list_4, 9, 9);
  ToHex(CH_list_5, 10, 10);

  Check_bit_accumulation();
  Send_the_modified_command();
  Delay(20);
  Readcallback();

  if (DelayScanwarning())
  {
    return "";
  }
  else
  {

    if (Verify_the_return(a, 8))
    {

      return "Insert working channel OK";
    }
    return "";

  }
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to obtain the current reader emission power

  用于获取当前读写器发射功率
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
ReadInfo UHF_RFID::Read_transmitting_power()
{
  UBYTE a[5] = {0xBB, 0x01, 0xB7, 0x00, 0x02};

  ReadInfo Read;

  Sendcommand(22);
  Delay(20);
  Readcallback();

  if (DelayScanwarning())
  {
    Read.Region =  "";
    Read.Channel_Index = "";
    Read.Pow = "";
    Read.Mixer_G =  "";
    Read.IF_G =  "";
    Read.Thrd =  "";
    return Read;
  }
  else
  {
    if (Verify_the_return(a, 5))
    {
      Return_to_convert(1);
      Read.Region =  "";
      Read.Channel_Index = "";
      Read.Pow = DATA_Str_M5led.substring(10, 14);
      Read.Mixer_G =  "";
      Read.IF_G =  "";
      Read.Thrd =  "";

      return Read;
    }

  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to set the current reader emission power
  Power parameter PoW: 0x07D0(current power is decimal 2000, i.e. 20dBm)

  用于设置当前读写器发射功率

  功率参数 Pow:  0x07D0(当前功率为十进制 2000，即 20dBm)
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
String UHF_RFID::Set_transmission_Power(UWORD Pow)
{
  UBYTE a[8] = {0xBB, 0x01, 0xB6, 0x00, 0x01, 0x00, 0xB8, 0x7E};

  Copy_command_library(23);

  ToHex(Pow, 5, 6);

  Check_bit_accumulation();
  Send_the_modified_command();
  Delay(20);
  Readcallback();

  if (DelayScanwarning())
  {
    return "";
  }
  else
  {
    if (Verify_the_return(a, 8))
    {
      Return_to_convert(1);

      return "Set transmission Power OK";
    }
    return "";
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to set transmitting continuous carrier or to turn off continuous carrier
  Parameter: 0xFF(0xFF is to turn on CW, 0x00 is to turn off CW)

  用于设置发射连续载波或者关闭连续载波

  指令参数 Parameter:	0xFF(0xFF 为打开连续波，0x00 为关闭连续波)
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
String UHF_RFID::Sets_to_transmit_a_continuous_carrier(UWORD Parameter)
{
  UBYTE a[8] = {0xBB, 0x01, 0xB0, 0x00, 0x01, 0x00, 0xB2, 0x7E};

  Copy_command_library(24);

  ToHex(Parameter, 5, 5);

  Check_bit_accumulation();
  Send_the_modified_command();
  Delay(20);
  Readcallback();

  if (DelayScanwarning())
  {
    return "";
  }
  else
  {
    if (Verify_the_return(a, 8))
    {
      Return_to_convert(1);

      if (Parameter == 0xff)
      {
        return "Sets up TCC OK";
      }
      else if (Parameter == 0x00)
      {
        return "Turn off TCC OK";
      }

    }
    return "";
  }
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to read receive demodulator parameters
  Gets the current reader receive demodulator parameters.The demodulator parameters are Mixer gain, IF AMP gain and signal demodulation threshold
  Mixer gain Mixer_G: 0x03(Mixer Mixer gain is 9dB)
  Gain IF_G: 0x06(IF AMP Gain: 36dB)
  Signal demodulation threshold THRD: 0x01B0(the smaller the signal demodulation threshold, the lower the return RSSI of the label that can be demodulated, but the more unstable,
  Not demodulated at all below a certain value;On the contrary, the larger the threshold is, the larger the RSSI of the label return signal can be demodulated, and the closer the distance is, the more stable it is.
  0x01B0 is the recommended minimum.)

    Mixer gain meter
    Type    Mixer_G (dB)
    0x00    0
    0x01    3
    0x02    6
    0x03    9
    0x04    12
    0x05    15
    0x06    16

    Gain meter of IF AMP
    Type    IF_G (dB)
    0x00    12
    0x01    18
    0x02    21
    0x03    24
    0x04    27
    0x05    30
    0x06    36
    0x07    40


  用于读取接收解调器参数

  获取当前读写器接收解调器参数。解调器参数有 Mixer 增益，中频放大器 IF AMP 增益和信号解调阈值

    混频器增益 Mixer_G:           0x03(混频器 Mixer 增益为 9dB)
  中频放大器增益 IF_G:          0x06(中频放大器 IF AMP 增益为 36dB)
  信号解调阈值 Thrd:              0x01B0(信号解调阈值越小能解调的标签返回 RSSI 越低，但越不稳定，
  低于一定值完全不能解调；相反阈值越大能解调的标签返回信号 RSSI 越大，距离越近，越稳定。
  0x01B0 是推荐的 最小值)

  混频器 Mixer 增益表       中频放大器 IF AMP 增益表
    Type	Mixer_G(dB)             Type	IF_G(dB)
    0x00	0                        0x00	12
    0x01	3                        0x01	18
    0x02	6                        0x02	21
    0x03	9                        0x03	24
    0x04	12                       0x04	27
    0x05	15                       0x05	30
    0x06	16                       0x06	36
                                   0x07	40
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
ReadInfo UHF_RFID::Read_receive_demodulator_parameters()
{
  UBYTE a[5] = {0xBB, 0x01, 0xF1, 0x00, 0x04};

  ReadInfo Read;

  Sendcommand(25);
  Delay(20);
  Readcallback();

  if (DelayScanwarning())
  {
    Read.Region =  "";
    Read.Channel_Index = "";
    Read.Pow = "";
    Read.Mixer_G = "";
    Read.IF_G =  "";
    Read.Thrd = "";
    return Read;
  }
  else
  {
    if (Verify_the_return(a, 5))
    {
      Return_to_convert(1);

      Read.Region =  "";
      Read.Channel_Index = "";
      Read.Pow = "";
      Read.Mixer_G =  DATA_Str_M5led.substring(10, 12);
      Read.IF_G =  DATA_Str_M5led.substring(12, 14);
      Read.Thrd =  DATA_Str_M5led.substring(14, 18);

      return Read;
    }

  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to set the receiving demodulator parameters
  Mixer gain Mixer_G: 0x03(Mixer Mixer gain is 9dB)
  Gain IF_G: 0x06(IF AMP Gain: 36dB)
  Signal demodulation threshold THRD: 0x01B0(The smaller the signal demodulation threshold, the lower the return RSSI of the label can be demodulated, but the more unstable it is
  , below a fixed value can not be demodulated at all;On the contrary, the larger the threshold is, the larger the RSSI of the label return signal can be demodulated, and the closer the distance is, the more stable it is.
  0x01B0 is the recommended minimum.)

    Mixer gain meter           Gain meter of IF AMP
   Type   Mixer_G (dB)           Type   IF_G (dB)
   0x00   0                      0x00   12
   0x01   3                      0x01   18
   0x02   6                      0x02   21
   0x03   9                      0x03   24
   0x04   12                     0x04   27
   0x05   15                     0x05   30
   0x06   16                     0x06   36

                                 0x07   40

  用于设置接收解调器参数

  混频器增益 Mixer_G:           0x03(混频器 Mixer 增益为 9dB)
  中频放大器增益 IF_G:          0x06(中频放大器 IF AMP 增益为 36dB)
  信号解调阈值 Thrd:              0x01B0(信号解调阈值越小能解调的标签返回 RSSI 越低，但越不稳定
  ，低于一 定值完全不能解调；相反阈值越大能解调的标签返回信号 RSSI 越大，距离越近，越稳定。
  0x01B0 是推荐的 最小值)

  混频器 Mixer 增益表       中频放大器 IF AMP 增益表
    Type	Mixer_G(dB)             Type	IF_G(dB)
    0x00	0                        0x00	12
    0x01	3                        0x01	18
    0x02	6                        0x02	21
    0x03	9                        0x03	24
    0x04	12                       0x04	27
    0x05	15                       0x05	30
    0x06	16                       0x06	36
                                   0x07	40
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
String UHF_RFID::Sets_the_receiv_demodulator_parameters(UBYTE Mixer_G, UBYTE IF_G, UWORD Thrd)
{
  UBYTE a[8] = {0xBB, 0x01, 0xF0, 0x00, 0x01, 0x00, 0xF2, 0x7E};

  Copy_command_library(26);

  ToHex(Mixer_G, 5, 5);
  ToHex(IF_G, 6, 6);
  ToHex(Thrd, 7, 8);

  Check_bit_accumulation();
  Send_the_modified_command();
  Delay(20);
  Readcallback();

  if (DelayScanwarning())
  {
    return "";
  }
  else
  {
    if (Verify_the_return(a, 8))
    {
      Return_to_convert(1);
      return "Sets up RDP OK";
    }
    return "";
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to test RF input block signal
  Channel blocking signal JMR: 0 xf2f1f0efeceae8eaeceef0f1f5f5f5f6f5f5f5f5 (blocking signals of each channel
  JMRs are all represented by a signed Byte, where 0xF2 is -14dbm.

  用于测试射频输入端阻塞信号

  信道阻塞信号 JMR:  0xF2 F1 F0 EF EC EA E8 EA EC EE F0 F1 F5 F5 F5 F6 F5 F5 F5 F5(每个信道的阻塞信号
  JMR 都用一个有符号的 Byte 表示，其中 0xF2 为-14dBm)
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
TestInfo UHF_RFID::Test_the_RF_input_blocking_signal()
{
  UBYTE a[5] = {0xBB, 0x01, 0xF2, 0x00, 0x16};
  UBYTE b = 0;
  UBYTE c = 0;
  UBYTE d = 0;
  UBYTE e = 0;

  TestInfo Test;

  Sendcommand(27);

  Readcallback();

  if (DelayScanwarning())
  {
    Test.CH_L = "";
    Test.CH_H = "";
    for (size_t i = 0; i < 20; i++)
    {
      Test.Data[i] = "";
    }

    return Test;
  }
  else
  {

    if (Verify_the_return(a, 5))
    {
      Return_to_convert(1);

      Test.CH_L = DATA_Str_M5led.substring(10, 12);
      Test.CH_H = DATA_Str_M5led.substring(12, 14);
      for (size_t i = 0; i < 20; i++)
      {
        Test.Data[i] = DATA_Str_M5led.substring(14 + (i) * 2, 16 + (i) * 2);
      }

      return Test;
    }

  }
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to test the RF input RSSI signal size
  Channel RSSI signal strength: 0 xbabababababababababababababababababababa (each channel
  RSSI is represented by a signed Byte, where 0xBA is -70dBm, which is the minimum RSSI that the reader can detect.

  用于测试射频输入端 RSSI 信号大小

  信道信号强度 RSSI:  0xBA BA BA BA BA BA BA BA BA BA BA BA BA BA BA BA BA BA BA BA(每个信道的
  RSSI 用一个有符号的 Byte 表示，其中 0xBA 为-70dBm，为读写器能检测的 RSSI 的最小值)
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
TestInfo UHF_RFID::Test_the_RSSI_input_signal()
{
  UBYTE a[5] = {0xBB, 0x01, 0xF3, 0x00, 0x16};
  UBYTE b = 0;
  UBYTE c = 0;
  UBYTE d = 0;
  UBYTE e = 0;

  TestInfo Test;

  Sendcommand(28);

  Readcallback();

  if (DelayScanwarning())
  {
    Test.CH_L = "";
    Test.CH_H = "";
    for (size_t i = 0; i < 20; i++)
    {
      Test.Data[i] = "";
    }

    return Test;
  }
  else
  {

    if (Verify_the_return(a, 5))
    {
      Return_to_convert(1);

      Test.CH_L = DATA_Str_M5led.substring(10, 12);
      Test.CH_H = DATA_Str_M5led.substring(12, 14);
      for (size_t i = 0; i < 20; i++)
      {
        Test.Data[i] = DATA_Str_M5led.substring(14 + (i) * 2, 16 + (i) * 2);
      }

      return Test;
    }
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used for module sleep
  The module sleep instruction can keep the module in low power sleep mode.After the module sleeps, it can wake up the module by sending any byte through the serial port.
  But the byte will be discarded, and the first instruction received after the module sleeps will have no response because the first character of the first instruction will be discarded.
  This command will reset the M100/QM100 chip when the power is off. After waking up, the module will immediately download the firmware to the M100/QM100 chip and reset it
  Put some parameters into the module (these parameters include pre-sleep configuration power, frequency, frequency hopping mode, sleep time, receive demodulator parameters, not included
  Select mode, Select parameters, etc.), so some parameters may need to be reset.

  用于模块休眠

  模块休眠指令可以让模块保持低功耗的休眠模式。模块休眠后，通过串口发送任意的字节即可唤醒模块，
  但该 字节会被抛弃掉，模块休眠后接收到的第一条指令会没有响应，因为第一条指令的第一个字符会被抛弃掉。
  该 指令会让 M100/QM100 芯片掉电重置，模块唤醒后会立刻重新下载固件到 M100/QM100 芯片中，并重新设
  置一些参数到模块中（这些参数包括休眠前配置的功率，频率，跳频模式，休眠时间，接收解调器参数，不包
  括 Select 模式，Select 参数等），因此有些参数可能需要重新设置。
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
String UHF_RFID::Set_module_hibernation()
{
  UBYTE a[8] = {0xBB, 0x01, 0x17, 0x00, 0x01, 0x00, 0x19, 0x7E};

  Sendcommand(30);
  Delay(20);
  Readcallback();

  if (DelayScanwarning())
  {
    return "";
  }
  else
  {

    if (Verify_the_return(a, 8))
    {
      return "The module of dormancy OK";
    }
    return "";
  }
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used for the idle sleep time of the module
  This command can set how long the module will automatically enter the sleep state after no operation.After module sleep, send arbitrary via serial port
  The character can wake up the module.The first instruction received by the module after hibernation will not respond because the first character of the first instruction will be discarded.
  This command will reset the M100/QM100 chip. After waking up, the module will immediately download the firmware to the M100/QM100 chip and reset it
  Some parameters to the module (these parameters include pre-sleep configuration of power, frequency, frequency hopping mode, sleep time, receiving demodulator parameters, not included
  Select mode, Select parameters, etc.), so some parameters may need to be reset.
  Parameter: 0x02(Sleep after 2 minutes without operation, range 1 to 30 minutes, 0x00 represents no automatic sleep)

  用于模块空闲休眠时间

  令帧定义 该指令可以设置模块经过多长时间没有操作后自动进入休眠状态。模块休眠后，通过串口发送任意
  的字符即可 唤醒模块。模块休眠后接收到的第一条指令会没有响应，因为第一条指令的第一个字符会被抛弃掉。
  该指令会 让 M100/QM100 芯片重置，模块唤醒后会立刻重新下载固件到 M100/QM100 芯片中，并重新设置
  一些参数到模块中（这些参数包括休眠前配置的功率，频率，跳频模式，休眠时间，接收解调器参数，不包括
  Select 模式，Select 参数等），因此有些参数可能需要重新设置。

  指令参数 Parameter:	0x02(2 分钟无操作之后休眠，范围 1~30 分钟，0x00 代表不自动休眠)
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
String UHF_RFID::Set_Sleep_Time(UWORD Parameter)
{
  UBYTE a[5] = {0xBB, 0x01, 0x1D, 0x00, 0x01};

  Copy_command_library(31);

  ToHex(Parameter, 6, 6);

  Check_bit_accumulation();
  Send_the_modified_command();
  Delay(20);
  Readcallback();

  if (DelayScanwarning())
  {
    return "";
  }
  else
  {

    if (Verify_the_return(a, 5))
    {
      Return_to_convert(1);
      return "sleep time :" + DATA_Str_M5led.substring(10, 12) + "Set the sleep time OK" ;
    }
    return "";
  }
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used to set the module ILDE mode
  This instruction can make the module enter the IDLE working mode. In the IDLE mode, except for the digital part and the communication interface, all the analog and RF parts are left
  The power supply is turned off to reduce power consumption when not working.After the module enters IDLE mode, it can still communicate with the module normally, and the parameters have been set
  The module is still saved and can respond to the instructions of the host computer.After entering IDLE mode, the label data is read and written for the first time
  Such instructions that need to interact with the label will restore the module to its normal state, but the first inventory may be due to the unstable state of the power supply in the RF part
  As a result, the success rate decreases, and the subsequent inventory and other operations can be restored to normal.
  Enter: 0x01(Enter IDLE mode, 0x00: exit IDLE mode)
  Reserved, fixed at 0x01
  Idle Time: 0x03(3 minutes without operation automatically into IDLE mode, range 0-30 minutes,
  0 x00 said don't automatically into IDLE mode)

  用于设置模块ILDE 模式

  该指令可以让模块进入 IDLE 工作模式，IDLE 模式下除了数字部分和通信接口，其余所有模拟和射频部分
  电源均被关闭，以减少不工作情况下的功耗。模块进入 IDLE 模式后，与模块仍可正常通信，已设置的参数
  仍然 被保存，模块可以正常响应上位机的指令。进入 IDLE 模式后，第一次盘点（或读取，写入标签数据
  此类需要与标签交互的指令）会让模块恢复到正常状态，但第一次盘点可能由于射频部分电源状态不稳定导
  致成功率下降，后续的盘点和其他操作即可恢复正常。

  是否进入 IDLE 模式 Enter:	0x01(进入 IDLE 模式，0x00:退出 IDLE 模式)
  指令参数 Reserved:	0x01(保留，固定为 0x01)
  IDLE 模式空闲时间 IDLE Time:	0x03(3 分钟无操作自动进入 IDLE 模式，范围 0-30 分钟，
                                0x00 表示不自动 进入 IDLE 模式)
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
String UHF_RFID::Set_the_ILDE_mode(UBYTE Enter, UBYTE IDLE_Time)
{
  UBYTE a[8] = {0xBB, 0x01, 0x04, 0x00, 0x01, 0x00, 0x06, 0x7E};

  Copy_command_library(32);

  ToHex(Enter, 5, 5);
  ToHex(IDLE_Time, 7, 7);

  Check_bit_accumulation();
  Send_the_modified_command();
  Delay(20);
  Readcallback();

  if (DelayScanwarning())
  {
    return "";
  }
  else
  {

    if (Verify_the_return(a, 8))
    {
      Return_to_convert(1);
      return "ILDE time :" + String(DATA_Interim_order[7]) + "Set ILDE OK";
    }
    return "";
  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   For NXP ReadProtect/Reset ReadProtect
  The NXP G2X label supports ReadProtect/Reset ReadProtect commands.When the tag executes the ReadProtect instruction successfully,
  The ProtectEPC and ProtectTID bits of the tag will be set to '1' and the tag will enter the data protection state.If you let the tag
  To return from the data protection state to the normal state, you need to execute the Reset ReadProtect command.
  This directive should be preceded by setting the SELECT parameter to Select the specified TAB to operate on.
  ReadProtect: 0x00(0x00 represents the execution of ReadProtect,
  0x01 means to execute Reset ReadProtect)

  用于NXP ReadProtect/Reset ReadProtect

  NXP G2X 标签支持 ReadProtect/Reset ReadProtect 指令。当标签执行 ReadProtect 指令成功，
  标签的 ProtectEPC和ProtectTID 位将会被设置为’1’，标签会进入到数据保护的状态。如果让标签
  从数据保护状态 回到正常状态，需要执行Reset ReadProtect 指令。
  这条指令之前应先设置 Select 参数，以便选择指定的标 签进行操作。

  ReadProtect/Reset ReadProtect:	0x00(0x00 代表执行 ReadProtect，
                                  0x01 代表执行 Reset ReadProtect)
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
CardInformationInfo UHF_RFID::NXP_ReadProtect_ResetReadProtect(UDOUBLE Access_Password, UBYTE ReadProtect)
{
  UBYTE a[5] = {0xBB, 0x01, 0xE1, 0x00, 0x10};
  UBYTE b[5] = {0xBB, 0x01, 0xE2, 0x00, 0x10};
  UBYTE e = 0;

  CardInformationInfo Cardinformation;

  Copy_command_library(33);

  ToHex(Access_Password, 5, 8);
  ToHex(ReadProtect, 9, 9);

  Check_bit_accumulation();
  Send_the_modified_command();
  Delay(50);
  Readcallback();

  if (e = DelayScanwarning())
  {

    if (e == 0x16)
    {
      Return_to_convert(1);
      Cardinformation = Access_Password_is_incorrect();
    }
    return Cardinformation;
  }
  else
  {

    if (Verify_the_return(a, 5))
    {
      Return_to_convert(1);

      Cardinformation = Operation_is_successful();
      Cardinformation._Successful = "NXP ReadProtect OK";

      return Cardinformation;
    }
    else if (Verify_the_return(b, 5))
    {
      Return_to_convert(1);

      Cardinformation = Operation_is_successful();
      Cardinformation._Successful = "NXP Reset ReadProtect OK";

      return Cardinformation;
    }

  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used for NXP Change EAS instructions
  The NXP G2X label supports the CHANGE EAS directive.When the tag executes the CHANGE EAS directive successfully, the tag's PSF bit will correspond
  It becomes a '1' or a '0'.When the tag's PSF position is' 1 ', the tag will respond to the EAS_ALARM instruction, otherwise the tag will not respond
  EAS_Alarm instructions.This directive should be preceded by setting the SELECT parameter to Select the specified label to operate on.
  PSF: 0x01(0x01 represents setting PSF bit to '1', 0x00 represents setting PSF bit to '0')

  用于NXP Change EAS 指令

  NXP G2X 标签支持 Change EAS 指令。当标签执行 Change EAS 指令成功，标签的 PSF 位将会相应的
  变成’1’ 或者’0’。当标签的 PSF 位置为’1’的时候，标签将响应 EAS_Alarm 指令，否则标签不响应
  EAS_Alarm 指令。 这条指令之前应先设置 Select 参数，以便选择指定的标签进行操作。

  PSF:	0x01(0x01 代表设置 PSF 位为’1’，0x00 代表设置 PSF 位为’0’)

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
CardInformationInfo UHF_RFID::NXP_Change_EAS(UDOUBLE Access_Password, UBYTE PSF)
{
  UBYTE a[5] = {0xBB, 0x01, 0xE3, 0x00, 0x10};
  UBYTE e = 0;

  CardInformationInfo Cardinformation;

  Copy_command_library(34);

  ToHex(Access_Password, 5, 8);
  ToHex(PSF, 9, 9);

  Check_bit_accumulation();
  Send_the_modified_command();

  Readcallback();

  if (e = DelayScanwarning())
  {
    if (e == 0x16)
    {
      Return_to_convert(1);
      Cardinformation = Access_Password_is_incorrect();
    }
    return Cardinformation;
  }
  else
  {

    if (Verify_the_return(a, 5))
    {
      Return_to_convert(1);

      Cardinformation = Operation_is_successful();
      Cardinformation._Successful = "NXP Change EAS OK";

      return Cardinformation;
    }

  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used for the NXP EAS_ALARM instruction
  The NXP G2X tag supports the EAS_ALARM directive.When the tag receives the EAS_ALARM instruction, the tag returns immediately
  64 bits EAS - Alarm code.Note that the tag responds to EAS_Alarm only if the tag's PSF position is' 1 '.
  Otherwise the label will not respond to the EAS_ALARM instruction.This instruction is suitable for electronic goods anti - theft (theft) system.

  用于NXP EAS_Alarm 指令

  NXP G2X 标签支持 EAS_Alarm 指令。当标签接收到 EAS_Alarm 指令后，标签会立刻返回
  64bits EAS-Alarm code。注意只有当标签的 PSF 位置为’1’的时候，标签才响应 EAS_Alarm 指令，
  否则标签不响应 EAS_Alarm 指令。该指令适合于电子商品防窃（盗）系统。

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
String UHF_RFID::NXP_EAS_Alarm()
{
  UBYTE a[8] = {0xBB, 0x01, 0xE4, 0x00, 0x08};

  Sendcommand(35);
  Delay(20);
  Readcallback();

  if (DelayScanwarning())
  {
    return "";
  }
  else
  {

    if (Verify_the_return(a, 5))
    {
      Return_to_convert(1);

      return "EAS-Alarm code :" + DATA_Str_M5led.substring(10, 26);
    }
    return "";
  }
}


/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used for the NXP ChangeConfig directive
  Certain families of the NXG2X tag (such as G2IM and G2IM +) support the ChangeConfig directive, which can be read or modified
  NXP G2X label 16bits config-word.The config-word of the NXP G2X label is located in the label store BANK 01 (i.e
  EPC area) address 20h (word address), can be Read through the ordinary Read instruction.When the label is in the Secured state
  (security state), you can overwrite the config-word of the label, it should be noted that to rewrite the config-word is to flip the config-word
  , that is, the corresponding bit for writing '1' is flipped (' 1 'becomes' 0', '0' becomes' 1 '), and the corresponding bit for writing '0' remains unchanged.
  This instruction should be preceded by setting the SELECT parameter to Select the specified label to operate on.
  Config-word: 0x0000(all 0 label returns unchanged config-word, equivalent to read)

  用于NXP ChangeConfig 指令

  NXP G2X 标签某些系列（如 G2iM 和 G2iM+）支持 ChangeConfig 指令，可以通过该指令读取或修改
  NXP G2X 标签的 16bits Config-Word。NXP G2X 标签的 Config-Word 位于标签存储区 Bank 01（即
  EPC 区）地址 20h 处（word address），可以通过普通的 Read 指令读取。当标签处于 Secured 状态
  （安全状态）时，可以改写 标签的 Config-Word，需要注意的是改写 Config-Word 是翻转 Config-Word
  的相应数据位，即写入’1’的对应 位翻转（‘1’变成‘0’，‘0’变成‘1’），写入‘0’的对应位保持不变。
  这条指令之前应先设置 Select 参数，以便选择指 定的标签进行操作。

  Config-Word:	0x0000(全 0 时标签返回未更改的 Config-Word，相当于读取)

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
CardInformationInfo UHF_RFID::NXP_Change_Config(UDOUBLE Access_Password, UWORD Config_Word)
{
  UBYTE a[5] = {0xBB, 0x01, 0xE0, 0x00, 0x11};
  UBYTE e = 0;

  CardInformationInfo Cardinformation;

  Copy_command_library(36);

  ToHex(Access_Password, 5, 8);
  ToHex(Config_Word, 9, 10);

  Check_bit_accumulation();
  Send_the_modified_command();

  Readcallback();

  if (e = DelayScanwarning())
  {
    if (e == 0x16)
    {
      Return_to_convert(1);
      Cardinformation = Access_Password_is_incorrect();
    }
    return Cardinformation;
  }
  else
  {

    if (Verify_the_return(a, 5))
    {
      Return_to_convert(1);

      Cardinformation = UI_PC_EPC();
      Cardinformation._Successful = "Config Word :" + DATA_Str_M5led.substring( 40,  44) + "NXP Change Config OK";

      return Cardinformation;
    }

  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used for the IMPINJ Monza QT directive
  Impinj Monza 4 Qt tags support Qt instructions that can modify the tag's Qt Control Word, where the QT_SR bit is set
  You can shorten the actions of the label in the Open and Secured states or when it is about to enter the Open and Secured states
  The QT_MEM bit can be used to switch between a Public Memory Map and a Private Memory Map
  (private storage).
  This directive should be preceded by setting the SELECT parameter to Select the specified label to operate on.
  Read/Write: 0x01(0x00: Read, 0x01: Write)
  Persistence: 0x01(0x00: Write to label volatile storage, 0x01: Write to non-volatile storage)
  Payload: 0x4000(Qt Control, maximum two bits QT_SR and QT_MEM)

  用于Impinj Monza QT 指令

  Impinj Monza 4 QT 标签支持 QT 指令，该指令可以修改标签的 QT Control word，其中设置 QT_SR 位
  可以缩 短标签在 Open（开放）和 Secured（安全）状态或者即将进入到 Open 和 Secured 状态时的操作
  距离，修改 QT_MEM 位可以切换标签使用 Public Memory Map（公共存储区）还是 Private Memory Map
  （私有存储区）。
  这条指令之前应先设置 Select 参数，以便选择指定的标签进行操作。

  Read/Write:	0x01(0x00: Read，0x01: Write)
  Persistence:	0x01(0x00: 写入标签挥发性存储区，0x01: 写入非挥发性存储区)
  Payload:	0x4000(QT Control，最高两个 bits 分别为 QT_SR 和 QT_MEM)

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
CardInformationInfo UHF_RFID::Impinj_Monza_QT(UDOUBLE Access_Password, UBYTE Read_Write, UBYTE Persistence, UWORD Payload)
{
  UBYTE a[5] = {0xBB, 0x01, 0xE5, 0x00, 0x11};
  UBYTE b[5] = {0xBB, 0x01, 0xE6, 0x00, 0x10};
  UBYTE e = 0;

  CardInformationInfo Cardinformation;

  Copy_command_library(37);

  ToHex(Access_Password, 5, 8);
  ToHex(Read_Write, 9, 9);
  ToHex(Persistence, 10, 10);
  ToHex(Payload, 11, 12);

  Check_bit_accumulation();
  Send_the_modified_command();

  Readcallback();

  if (e = DelayScanwarning())
  {
    if (e == 0x16)
    {
      Return_to_convert(1);
      Cardinformation = Access_Password_is_incorrect();
    }
    return Cardinformation;
  }
  else
  {

    if (Verify_the_return(a, 5))
    {
      Return_to_convert(1);

      Cardinformation = UI_PC_EPC();
      Cardinformation._Successful = "QT Control :0" + DATA_Str_M5led.substring( 40,  42) + "1" + DATA_Str_M5led.substring( 42,  44) + "Read Impinj Monza QT OK";

      return Cardinformation;
    }
    else if (Verify_the_return(b, 5))
    {
      Return_to_convert(1);

      Cardinformation = Operation_is_successful();
      Cardinformation._Successful = "Write Impinj Monza QT OK";

      return Cardinformation;
    }

  }
}

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
   Used for the BlockPermalock directive
  The BlockPermalock directive can lock blocks permanently or read the locked state of a Block.
  This directive should be preceded by setting the SELECT parameter to Select the specified label to operate on.
  Read_Lock: 0 x00 (0 x00: Read, 0 x01: Lock)
  Block PTR: 0x0000 Block address of Mask
  Block range: 0x01
  Mask: 0x0700(when Read/Lock data field is 0x00, i.e. Read state, this data field is omitted)

  用于BlockPermalock 指令

  BlockPermalock 指令可以永久锁定用户区的某几个 Block，或者读取 Block 的锁定状态。
  这条指令之前应先 设置 Select 参数，以便选择指定的标签进行操作。

  Read_Lock:	0x00(0x00:Read，0x01:Lock)
  BlockPtr:	0x0000(Mask 的起始 Block 地址，以 16 个 Block 为单位)
  BlockRange:	0x01(16 个 Block 为单位)
  Mask:	0x0700(当 Read/Lock 数据域为 0x00，即读取状态时，该数据域省略)

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
CardInformationInfo UHF_RFID::BlockPermalock(UDOUBLE Access_Password, UBYTE Read_Lock, UBYTE MemBank, UWORD BlockPtr, UBYTE BlockRange, UWORD Mask)
{
  UBYTE a[5] = {0xBB, 0x01, 0xD3, 0x00, 0x12};
  UBYTE b[5] = {0xBB, 0x01, 0xD4, 0x00, 0x10};
  UBYTE e = 0;

  CardInformationInfo Cardinformation;

  Copy_command_library(38);

  ToHex(Access_Password, 5, 8);
  ToHex(Read_Lock, 9, 9);
  ToHex(MemBank, 10, 10);
  ToHex(BlockPtr, 11, 12);
  ToHex(BlockRange, 13, 13);
  ToHex(Mask, 14, 15);

  Check_bit_accumulation();
  Send_the_modified_command();

  Readcallback();

  if (e = DelayScanwarning())
  {
    if (e == 0x16)
    {
      Return_to_convert(1);
      Cardinformation = Access_Password_is_incorrect();
    }
    else if (e / 0x10 == 0xE )
    {
      Return_to_convert(1);
      Cardinformation = EPC_Gen2_error_code();
    }
    return Cardinformation;
  }
  else
  {

    if (Verify_the_return(a, 5))
    {
      Return_to_convert(1);

      Cardinformation = UI_PC_EPC( );
      Cardinformation._Successful = "BlockPermalock :" + DATA_Str_M5led.substring( 40,  44) + "Read Impinj Monza QT OK";

      return Cardinformation;
    }
    else if (Verify_the_return(b, 5))
    {
      Return_to_convert(1);

      Cardinformation = Operation_is_successful();
      Cardinformation._Successful = "Lock Impinj Monza QT OK";


      return Cardinformation;
    }

  }
}
