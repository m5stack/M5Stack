/*
  please add MCP_CAN_LIB to your library first........
  MCP_CAN_LIB file in M5stack lib examples -> modules -> COMMU ->
  MCP_CAN_lib.rar
*/

#include <M5Stack.h>
#include <mcp_can.h>
#include "m5_logo.h"

/*
    test RS485 first by default
 */

/**
 * variable for RS485
 */

// FH  len  fun   data   CRC
// 01 aa 00 09 00 01 00 01 b6

char uart_buffer[10] = {0x01, 0xaa, 0x00, 0x09, 0x00, 0x01, 0x00, 0x01, 0xb6};
char comchar;
char Num       = 0;
int Send_Count = 0;
int Send_OK    = 0;
int RECcheck   = 0;
char stringnum = 0;

typedef union {
    struct {
        char buff[10];
    };
    struct {
        short frame;
        short datalength;
        short function;
        short data;
        char check;
    };
    struct {
        char framelow;
        char framehigh;
        char datalengthlow;
        char datalengthhigh;
        char functionhigh;
        char functionlow;
        char datahigh;
        char datalow;
    };
} uart_data;

uart_data UART_DATA    = {0};
uart_data UART_RECDATA = {0};

void init_rs485();
void test_rs485();

void updatedata() {
    Send_Count = (Send_Count + 1) & 0xffff;
    // Update values
    UART_DATA.datahigh = (Send_Count & 0xff00) >> 8;
    UART_DATA.datalow  = Send_Count & 0xff;
    // Calculates and verifies values
    UART_DATA.check = UART_DATA.framelow + UART_DATA.framehigh +
                      UART_DATA.datalengthlow + UART_DATA.datalengthhigh +
                      UART_DATA.functionlow + UART_DATA.functionhigh +
                      UART_DATA.datalow + UART_DATA.datahigh;

    for (stringnum = 0; stringnum < 9; stringnum++)
        Serial2.print(UART_DATA.buff[stringnum]);

    M5.Lcd.fillRect(0, 30, 300, 20, WHITE);
    M5.Lcd.setCursor(0, 30);
    M5.Lcd.printf("Send count: %d", Send_Count);

    M5.Lcd.fillRect(0, 50, 300, 20, WHITE);
    M5.Lcd.setCursor(0, 50);
    M5.Lcd.printf("Send count OK: %d\r\n", Send_OK);
}

void setup() {
    M5.begin();
    M5.Power.begin();
    Serial.begin(9600);
    Serial2.begin(9600, SERIAL_8N1, 16, 17);

    M5.Lcd.pushImage(0, 0, 320, 240, (uint16_t *)gImage_logoM5);
    delay(500);
    M5.Lcd.setTextColor(BLACK);
    // M5.Lcd.setTextSize(1);

    init_rs485();
    Serial.println("Test RS485...");
}

void loop() {
    if (M5.BtnA.wasPressed()) {
        M5.Lcd.clear();
        M5.Lcd.printf("RS485 Test B!\n");
        M5.Lcd.pushImage(0, 0, 320, 240, (uint16_t *)gImage_logoM5);
        init_rs485();
        Serial.println("Test RS485...");
    }
    test_rs485();
    M5.update();
}

void init_rs485() {
    M5.Lcd.pushImage(0, 0, 320, 240, (uint16_t *)gImage_logoM5);
    delay(500);

    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(0, 10);
    M5.Lcd.printf("RS485 Test B!\n");
    Serial.println("RS485 Test B!\n");
    /* nit data
    FH  len  fun   data   CRC
    01 aa 00 09 00 01 00 01 b6 */
    memcpy(UART_DATA.buff, uart_buffer, 9);

    // updatedata();
}

void test_rs485() {
    while (Serial2.available() > 0) {
        Num = Serial2.readBytes(uart_buffer, 9);  // Read the serial string
        if (Num == 9) {
            memcpy(UART_RECDATA.buff, uart_buffer, 9);
            // Calculates and verifies values
            RECcheck = UART_RECDATA.framelow + UART_RECDATA.framehigh +
                       UART_RECDATA.datalengthlow +
                       UART_RECDATA.datalengthhigh + UART_RECDATA.functionlow +
                       UART_RECDATA.functionhigh + UART_RECDATA.datalow +
                       UART_RECDATA.datahigh;
            if (UART_RECDATA.check == (RECcheck & 0xff)) {
                Send_OK++;
            }
        }
        updatedata();
    }
}
