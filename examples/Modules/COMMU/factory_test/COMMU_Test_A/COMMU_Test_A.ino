/*
    Description: Press button A for CAN communication test, press button B for RS485 communication test.
    Please install library before compiling:  
    MCP_CAN_LIB: file in M5stack lib examples -> modules -> COMMU -> MCP_CAN_lib.rar（unzip the lib zip file to the Arduino Lib path）
*/

#include <M5Stack.h>
#include <mcp_can.h>
#include "m5_logo.h"

/*
    test RS485 first by default
 */

/**
 * variable for loop
 */
uint8_t flag_test_rs485 = 1;//test RS485 first by default
uint8_t flag_test_can = 0;

byte data[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};


/**
 * variable for CAN
 */
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[8];
char msgString[128];                        // Array to store serial string

#define CAN0_INT 15                              // Set INT to pin 2
MCP_CAN CAN0(12);                               // Set CS to pin 10

/**
 * variable for RS485
 */
// FH  len  fun   data   CRC
// 01 aa 00 09 00 01 00 01 b6
char uart_buffer[10] = {0x01, 0xaa, 0x00, 0x09, 0x00, 0x01, 0x00, 0x01, 0xb6};
char comchar;
char Num = 0;
int Send_Count = 0;
int Send_OK = 0;
int RECcheck = 0;
char stringnum = 0;

typedef union
{
  struct
  {
    char buff[10];
  };
  struct
  {
    short frame;
    short datalength;
    short function;
    short data;
    char check;
  };
  struct
  {
    char framelow;
    char framehigh;
    char datalengthlow;
    char datalengthhigh;
    char functionhigh;
    char functionlow;
    char datahigh;
    char datalow;
  };
}uart_data;

uart_data UART_DATA = {0};
uart_data UART_RECDATA = {0};

void init_rs485();
void init_can();
void test_rs485();
void test_can();

void updatedata()
{
  Send_Count = ( Send_Count + 1 ) & 0xffff;
   // Update values
  UART_DATA.datahigh =  (Send_Count & 0xff00)>>8;
  UART_DATA.datalow =  Send_Count & 0xff;
  // Calculates and verifies values
  UART_DATA.check = UART_DATA.framelow + UART_DATA.framehigh + UART_DATA.datalengthlow + UART_DATA.datalengthhigh + UART_DATA.functionlow + UART_DATA.functionhigh + UART_DATA.datalow + UART_DATA.datahigh;

  for(stringnum = 0;stringnum < 9; stringnum ++)
  {
    Serial2.print(UART_DATA.buff[stringnum]);
    Serial.print(UART_DATA.buff[stringnum]);
    Serial.print(" ");
  }

  M5.Lcd.fillRect(0, 30, 300, 20, WHITE);
  M5.Lcd.setCursor(0, 30);
  M5.Lcd.printf("Send count: %d",Send_Count);

  M5.Lcd.fillRect(0, 50, 300, 20, WHITE);
  M5.Lcd.setCursor(0, 50);
  M5.Lcd.printf("Send count OK: %d\r\n",Send_OK);
}

void setup() {
  M5.begin();
  Serial.begin(9600);
  Serial2.begin(9600, SERIAL_8N1, 16, 17);

  M5.Lcd.drawBitmap(0, 0, 320, 240, (uint16_t *)gImage_m5_logo);
  delay(500);
  M5.Lcd.setTextColor(BLACK);
  // M5.Lcd.setTextSize(1);

  init_rs485();
}

void loop() {
  if(M5.BtnA.wasPressed())
  {
    flag_test_can = 1;
    flag_test_rs485 = 0;
    init_can();
    Serial.println("Test CAN...");
  }
  if(M5.BtnB.wasPressed())
  {
    flag_test_can = 0;
    flag_test_rs485 = 1;
    init_rs485();
    Serial.println("Test RS485...");
  }
  M5.update();
  if(flag_test_can)
  {
    test_can();
  }
  if(flag_test_rs485)
  {
    test_rs485();
  }
  M5.update();
}

void init_rs485(){
  M5.Lcd.drawBitmap(0, 0, 320, 240, (uint16_t *)gImage_m5_logo);
  delay(500);

  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.printf("RS485 Test A!\n");
  Serial.println("RS485 Test A!\n");
  /* nit data
  FH  len  fun   data   CRC
  01 aa 00 09 00 01 00 01 b6 */
  memcpy(UART_DATA.buff, uart_buffer, 9);

  updatedata();
}

void init_can(){
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 10);
  M5.Lcd.drawBitmap(0, 0, 320, 240, (uint8_t *)gImage_m5_logo);
  delay(500);

  M5.Lcd.printf("CAN Test!\n");
  M5.Lcd.printf("Receive first, then testing for sending function!\n");

  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if(CAN0.begin(MCP_ANY, CAN_1000KBPS, MCP_8MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.

  pinMode(CAN0_INT, INPUT);                            // Configuring pin for /INT input

  Serial.println("MCP2515 Library Receive Example...");
}

void test_rs485(){
  while(Serial2.available() > 0)
  {
    Num = Serial2.readBytes(uart_buffer,9);//Read the serial string
    if(Num == 9)
    {
      memcpy(UART_RECDATA.buff, uart_buffer, 9);
        // Calculates and verifies values
      RECcheck = UART_RECDATA.framelow + UART_RECDATA.framehigh + UART_RECDATA.datalengthlow + UART_RECDATA.datalengthhigh + UART_RECDATA.functionlow + UART_RECDATA.functionhigh + UART_RECDATA.datalow + UART_RECDATA.datahigh;
      {
        Send_OK++;
      }
    }
    updatedata();
  }
}

void test_can(){
  if(!digitalRead(CAN0_INT))                         // If CAN0_INT pin is low, read receive buffer
  {
    CAN0.readMsgBuf(&rxId, &len, rxBuf);      // Read data: len = data length, buf = data byte(s)

    if((rxId & 0x80000000) == 0x80000000)     // Determine if ID is standard (11 bits) or extended (29 bits)
      sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", (rxId & 0x1FFFFFFF), len);
    else
      sprintf(msgString, "Standard ID: 0x%.3lX       DLC: %1d  Data:", rxId, len);

    Serial.print(msgString);
    M5.Lcd.printf(msgString);
    if((rxId & 0x40000000) == 0x40000000){    // Determine if message is a remote request frame.
      sprintf(msgString, " REMOTE REQUEST FRAME");
      Serial.print(msgString);
    } else {
      for(byte i = 0; i<len; i++){
        sprintf(msgString, " 0x%.2X", rxBuf[i]);
        Serial.print(msgString);
        M5.Lcd.printf(msgString);
      }
    }
    M5.Lcd.printf("\n");
    Serial.println();
  }
}