/*
    Description: 
    By enabling or deleting the macro definition `COORDINNATOR`, burn the program to the two devices respectively
    After booting, `Coordinator` will continuously broadcast data, `End Device` will display the current signal strength and data packet loss
    Note: 16 and 17 of the DIP switch are set to ON.
*/


#include "M5Stack.h"
#include "byteArray.h"
#include "DRFZigbee.h"

#include <stdarg.h>
#include <initializer_list>

#include "resource.h"

DRFZigbee zigbee;

#define ZIGBEE_PANID    0x1620
//#define ZIGBEE_PANID    0x162A
//#define COORDINNATOR

#define ZIGBEE_RSSI_TEST

#ifdef ZIGBEE_RSSI_TEST
    TFT_eSprite rssiPrintSprite(&M5.Lcd);
#endif

uint16_t reviceCount = 0,timeoutCount = 0,errorCount = 0;
unsigned long reviceTime = 0;

const uint8_t *iconptrbuff[3] = {
    coordinator_jpeg_120x140,
    endDevice_jpeg_120x140,
    router_jpeg_120x140,
};
size_t iconSizeBuff[3] = {
    27537,
    24609,
    26433,
};

uint16_t posbuff[10][2] = {
    {180,60},
    {200,60},
    {220,60},
    {240,60},
    {260,60},

    {180,80},
    {200,80},
    {220,80},
    {240,80},
    {260,80},
};

char asciiHexList[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

bool flushFlag = true;

void configZigbee()
{
#ifdef COORDINNATOR
    M5.Lcd.drawJpg(iconptrbuff[0], iconSizeBuff[0], 100, 40, 120, 140, 0, 0);
#else
    M5.Lcd.drawJpg(iconptrbuff[1], iconSizeBuff[1], 20, 40, 120, 140, 0, 0);
#endif
    DRFZigbee::zigbee_arg_t *arg = new DRFZigbee::zigbee_arg_t;
    zigbee.linkMoudle();
    zigbee.readModuleparm(arg);

#ifdef COORDINNATOR
    arg->main_pointType = DRFZigbee::kCoordinator;
#else
    arg->main_pointType = DRFZigbee::kEndDevice;
#endif
    arg->main_PANID = DRFZigbee::swap<uint16_t>(ZIGBEE_PANID);
    arg->main_channel = 20;
    arg->main_transmissionMode = DRFZigbee::kN2Ntransmission;
    arg->main_ATN = DRFZigbee::kANTEXP;

    Serial.printf("PAIN ID:%04X\r\n",arg->main_PANID);

    zigbee.setModuleparm(*arg);
    zigbee.rebootModule();
    delay(500);
}

void setup()
{
    M5.begin();
    Serial2.begin(38400, SERIAL_8N1, 16, 17);
    zigbee.begin(Serial2);

    M5.Lcd.fillRect(0, 0, 320, 240, M5.Lcd.color565(56, 56, 56));

    #ifdef ZIGBEE_RSSI_TEST
    rssiPrintSprite.createSprite(140,40);
    rssiPrintSprite.fillRect(0, 0, 140, 40, M5.Lcd.color565(56, 56, 56));
    #endif

    configZigbee();

    reviceTime = millis();
}

void loop()
{
#ifdef COORDINNATOR
    zigbee.sendDataP2P(DRFZigbee::kP2PShortAddrMode,0xffff,{0xaa,0x55,0x01,0x12});
    delay(50);
#else
    DRFZigbee::reviceData_t revice;
    if (zigbee.reviceData(&revice,DRFZigbee::kP2PShortAddrMode,1000) == DRFZigbee::kReviceOK)
    {
        revice.array->printfArray<HardwareSerial>(&Serial);
        if(( revice.array->at(0) == 0xaa )&&( revice.array->at(1) == 0x55 )&&( revice.array->at(3) == 0x12))
        {
            M5.Lcd.fillRect(posbuff[reviceCount + errorCount][0] + 2,posbuff[reviceCount + errorCount][1] + 2,15,15,M5.Lcd.color565(0x10,0x10,0x10));
            M5.Lcd.fillRect(posbuff[reviceCount + errorCount][0],posbuff[reviceCount + errorCount][1],15,15,M5.Lcd.color565(0xab,0xff,0x58));
            reviceCount++;
        }
        else 
        {
            M5.Lcd.fillRect(posbuff[reviceCount + errorCount][0] + 2,posbuff[reviceCount + errorCount][1] + 2,15,15,M5.Lcd.color565(0x10,0x10,0x10));
            M5.Lcd.fillRect(posbuff[reviceCount + errorCount][0],posbuff[reviceCount + errorCount][1],15,15,TFT_RED);
            errorCount++;
            Serial.printf("Error\r\n");
        }
    }
    if( reviceCount + errorCount == 10 )
    {
        char strbuff[256];
        sprintf(strbuff,"%d %d %d %ld\r\n",reviceCount + errorCount,reviceCount,errorCount,(millis() - reviceTime));
        M5.Lcd.setTextDatum(TL_DATUM);

        M5.Lcd.fillRect(180 + 2,120 + 2,20,20,M5.Lcd.color565(0x10,0x10,0x10));
        if(((millis() - reviceTime) < 2500 )&&( errorCount < 2 ))
        {
            M5.Lcd.setTextColor(M5.Lcd.color565(0xab,0xff,0x58));
            M5.Lcd.fillRect(180,120,20,20,M5.Lcd.color565(0xab,0xff,0x58));
            M5.Lcd.drawString("OK",210,120,4);
        }
        else
        {
            M5.Lcd.setTextColor(TFT_RED);
            M5.Lcd.fillRect(180,120,20,20,TFT_RED);
            M5.Lcd.drawString("FAILED",210,120,4);
        }
        //M5.Lcd.drawString(strbuff,180,153,2);

        int8_t rssi = -1;
        do
        {
            rssi = zigbee.getModuleRSSI();
            delay(100);
        } while ( rssi == -1 );

        sprintf(strbuff,"RSSI:%d\r\n",rssi);
        M5.Lcd.drawString(strbuff,180,160,4);

        while(1)
        {
            if( M5.BtnB.wasPressed())
            {
                M5.Lcd.fillRect(180, 0, 140, 240, M5.Lcd.color565(56, 56, 56));
                configZigbee();
                break;
            }
            M5.update();
            delay(10);
        }

        reviceTime = millis();
        errorCount = 0;
        reviceCount = 0;
        delay(10);
    }
    /*
    int8_t rssi = zigbee.getModuleRSSI();
    Serial.printf("rssi:%d",rssi);
    if( rssi != -1 )
    {
        char strbuff[256];
        sprintf(strbuff,"RSSI:%d\r\n",rssi);

        rssiPrintSprite.setTextColor(M5.Lcd.color565(0xab,0xff,0x58));
        rssiPrintSprite.fillRect(0, 0, 140, 40, M5.Lcd.color565(56, 56, 56));
        rssiPrintSprite.drawString(strbuff,0,0,4);
        rssiPrintSprite.pushSprite(180,60);
    }
    delay(200);
    */

#endif
    M5.update();
    delay(10);
}