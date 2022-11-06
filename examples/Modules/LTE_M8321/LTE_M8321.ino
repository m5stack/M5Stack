/*
    Description: Make calls using LTE Module
*/
#include <M5Stack.h>
#include <stdint.h>

#include <vector>

TFT_eSprite Disbuff            = TFT_eSprite(&M5.Lcd);
TaskHandle_t xhandle_lte_event = NULL;
SemaphoreHandle_t command_list_samap;

typedef enum {
    kQUERY_MO = 0,
    KTEST_MO,
    kASSIGN_MO,
    kACTION_MO,
    kQUERY_MT,
    kTEST_MT,
    kASSIGN_MT,
    kACTION_MT,
    kINFORM
} LTEMsg_t;

typedef enum {
    kErrorSendTimeOUT = 0xe1,
    kErrorReError     = 0xe2,
    kErroeSendError   = 0xe3,
    kSendReady        = 0,
    kSending          = 1,
    kWaitforMsg       = 2,
    kWaitforRead      = 3,
    kReOK
} LTEState_t;

struct ATCommand {
    uint8_t command_type;
    String str_command;
    uint16_t send_max_number;
    uint16_t max_time;
    uint8_t state;
    String read_str;
    uint16_t _send_count;
    uint16_t _send_time_count;

} user;

using namespace std;
vector<ATCommand> serial_at;
String zmmi_str;
void LTEModuleTask(void *arg) {
    int Number = 0;
    String restr;
    while (1) {
        xSemaphoreTake(command_list_samap, portMAX_DELAY);

        if (Serial2.available() != 0) {
            String str = Serial2.readString();
            restr += str;

            if (restr.indexOf("\r\n") != -1) {
            }

            if (restr.indexOf("+ZMMI:") != -1) {
                zmmi_str = restr;
            } else if ((restr.indexOf("OK") != -1) ||
                       (restr.indexOf("ERROR") != -1)) {
                Serial.print(restr);
                if (restr.indexOf("OK") != -1) {
                    if ((serial_at[0].command_type == kACTION_MO) ||
                        (serial_at[0].command_type == kASSIGN_MO)) {
                        serial_at.erase(serial_at.begin());
                        Serial.printf("erase now %d\n", serial_at.size());
                    } else {
                        serial_at[0].read_str = restr;
                        serial_at[0].state    = kWaitforRead;
                    }
                } else if (restr.indexOf("ERROR") != -1) {
                    serial_at[0].state = kErrorReError;
                } else {
                }
                restr.clear();
            }
        }

        if (serial_at.empty() != true) {
            Number = 0;
            switch (serial_at[0].state) {
                case kSendReady:
                    Serial.printf(serial_at[0].str_command.c_str());
                    Serial2.write(serial_at[0].str_command.c_str());
                    serial_at[0].state = kSending;
                    break;
                case kSending:

                    if (serial_at[0]._send_time_count > 0) {
                        serial_at[0]._send_time_count--;
                    } else {
                        serial_at[0].state = kWaitforMsg;
                    }
                    /* code */
                    break;
                case kWaitforMsg:
                    if (serial_at[0]._send_count > 0) {
                        serial_at[0]._send_count--;
                        serial_at[0]._send_time_count = serial_at[0].max_time;
                        Serial.printf(serial_at[0].str_command.c_str());
                        Serial2.write(serial_at[0].str_command.c_str());
                        restr.clear();
                        serial_at[0].state = 1;
                    } else {
                        serial_at[0].state = kErrorSendTimeOUT;
                    }
                    /* code */
                    break;
                case kWaitforRead:
                    /* code */
                    break;
                case 4:
                    /* code */
                    break;
                case kErrorSendTimeOUT:
                    /* code */
                    break;
                case 0xe2:
                    /* code */
                    break;
                default:
                    break;
            }
        }
        xSemaphoreGive(command_list_samap);
        delay(10);
    }
}

void AddMsg(String str, uint8_t type, uint16_t sendcount, uint16_t sendtime) {
    struct ATCommand newcommand;
    newcommand.str_command      = str;
    newcommand.command_type     = type;
    newcommand.max_time         = sendtime;
    newcommand.send_max_number  = sendcount;
    newcommand.state            = 0;
    newcommand._send_count      = sendcount;
    newcommand._send_time_count = sendtime;
    xSemaphoreTake(command_list_samap, portMAX_DELAY);
    serial_at.push_back(newcommand);
    xSemaphoreGive(command_list_samap);
}

uint8_t readSendState(uint32_t number) {
    xSemaphoreTake(command_list_samap, portMAX_DELAY);
    uint8_t restate = serial_at[number].state;
    xSemaphoreGive(command_list_samap);
    return restate;
}

uint32_t getATMsgSize() {
    xSemaphoreTake(command_list_samap, portMAX_DELAY);
    uint32_t restate = serial_at.size();
    xSemaphoreGive(command_list_samap);
    return restate;
}
String ReadMsgstr(uint32_t number) {
    xSemaphoreTake(command_list_samap, portMAX_DELAY);
    String restate = serial_at[number].read_str;
    xSemaphoreGive(command_list_samap);
    return restate;
}

bool EraseFirstMsg() {
    xSemaphoreTake(command_list_samap, portMAX_DELAY);
    serial_at.erase(serial_at.begin());
    xSemaphoreGive(command_list_samap);
    return true;
}

uint16_t GetstrNumber(String Str, uint32_t *ptrbuff) {
    uint16_t count = 0;
    String Numberstr;
    int indexpos = 0;
    while (Str.length() > 0) {
        indexpos = Str.indexOf(",");
        if (indexpos != -1) {
            Numberstr      = Str.substring(0, Str.indexOf(","));
            Str            = Str.substring(Str.indexOf(",") + 1, Str.length());
            ptrbuff[count] = Numberstr.toInt();
            count++;
        } else {
            ptrbuff[count] = Str.toInt();
            count++;
            break;
        }
    }
    return count;
}
vector<String> restr_v;
uint16_t GetstrNumber(String StartStr, String EndStr, String Str) {
    uint16_t count = 0;
    String Numberstr;
    int indexpos = 0;

    Str = Str.substring(Str.indexOf(StartStr) + StartStr.length(),
                        Str.indexOf(EndStr));
    Str.trim();
    restr_v.clear();

    while (Str.length() > 0) {
        indexpos = Str.indexOf(",");
        if (indexpos != -1) {
            Numberstr = Str.substring(0, Str.indexOf(","));
            Str       = Str.substring(Str.indexOf(",") + 1, Str.length());
            restr_v.push_back(Numberstr);
            count++;
        } else {
            restr_v.push_back(Numberstr);
            ;
            count++;
            break;
        }
    }
    return count;
}

String getReString(uint16_t Number) {
    if (restr_v.empty()) {
        return String("");
    }
    return restr_v.at(Number);
}

uint16_t GetstrNumber(String StartStr, String EndStr, String Str,
                      uint32_t *ptrbuff) {
    uint16_t count = 0;
    String Numberstr;
    int indexpos = 0;

    Str = Str.substring(Str.indexOf(StartStr) + StartStr.length(),
                        Str.indexOf(EndStr));
    Str.trim();

    while (Str.length() > 0) {
        indexpos = Str.indexOf(",");
        if (indexpos != -1) {
            Numberstr      = Str.substring(0, Str.indexOf(","));
            Str            = Str.substring(Str.indexOf(",") + 1, Str.length());
            ptrbuff[count] = Numberstr.toInt();
            count++;
        } else {
            ptrbuff[count] = Str.toInt();
            count++;
            break;
        }
    }
    return count;
}
uint32_t numberbuff[128];
String readstr;

void setup() {
    // put your setup code here, to run once:
    M5.begin();
    M5.Power.begin();
    Serial2.begin(115200, SERIAL_8N1, 16, 17);

    Disbuff.createSprite(320, 100);
    Disbuff.fillRect(0, 0, 320, 100, BLACK);
    Disbuff.drawRect(0, 0, 320, 20, Disbuff.color565(36, 36, 36));
    Disbuff.pushSprite(0, 0);

    pinMode(2, OUTPUT);
    digitalWrite(2, 0);

    Disbuff.setTextColor(WHITE);
    Disbuff.setTextSize(1);
    for (int i = 0; i < 100; i++) {
        Disbuff.fillRect(0, 0, 320, 20, Disbuff.color565(36, 36, 36));
        Disbuff.pushSprite(0, 0);
        Disbuff.setCursor(7, 7);
        Disbuff.printf("Reset Module %02d", i);
        Disbuff.pushSprite(0, 0);
        delay(10);
    }
    digitalWrite(2, 1);

    xTaskCreate(LTEModuleTask, "LTEModuleTask", 1024 * 2, (void *)0, 4,
                &xhandle_lte_event);
    command_list_samap = xSemaphoreCreateMutex();
    xSemaphoreGive(command_list_samap);

    int count_t = 0;
    AddMsg("AT\r\n", kASSIGN_MO, 1000, 1000);
    while (getATMsgSize() > 0) {
        Disbuff.fillRect(0, 0, 320, 20, Disbuff.color565(36, 36, 36));
        Disbuff.pushSprite(0, 0);
        Disbuff.setCursor(7, 7);
        Disbuff.printf("Wait Modlue Srart %02d", count_t);
        Disbuff.pushSprite(0, 0);
        count_t++;
        delay(300);
    }
    /*
    AddMsg("AT+GMR?\r\n", kQUERY_MT, 1000, 1000);
    while ((readSendState(0) == kSendReady) || (readSendState(0) == kSending) ||
    (readSendState(0) == kWaitforMsg))delay(50);
    */

    AddMsg("AT^CARDMODE\r\n", kQUERY_MT, 1000, 1000);
    while ((readSendState(0) == kSendReady) || (readSendState(0) == kSending) ||
           (readSendState(0) == kWaitforMsg))
        delay(50);
    Serial.printf("Read state = %d \n", readSendState(0));
    readstr   = ReadMsgstr(0).c_str();
    int count = GetstrNumber("CARDMODE:", "OK", readstr, numberbuff);
    if (count != 0) {
        Serial.printf("CardMode = %d", numberbuff[0]);
        Disbuff.fillRect(0, 0, 320, 20, Disbuff.color565(36, 36, 36));
        Disbuff.pushSprite(0, 0);
        Disbuff.setCursor(7, 7);
        switch (numberbuff[0]) {
            case 0:
                Disbuff.printf("Unknown Card");
                break;
            case 1:
                Disbuff.printf("SIM Card");
                break;
            case 2:
                Disbuff.printf("USIM Card");
                break;
            default:
                Disbuff.printf("Unknown Card:E");
                break;
        }
        Disbuff.pushSprite(0, 0);
    }
    EraseFirstMsg();

    AddMsg("AT+CFUN=1\r\n", kASSIGN_MO, 1000, 1000);

    AddMsg("AT+CGDCONT=1,\"IP\"\r\n", kASSIGN_MO, 1000, 1000);
    AddMsg("AT+CLIP=1\r\n", kASSIGN_MO, 1000, 1000);
    AddMsg("AT+CREG=1\r\n", kASSIGN_MO, 1000, 1000);
    AddMsg("AT+CGREG=1\r\n", kASSIGN_MO, 1000, 1000);
    AddMsg("AT+COPS=1,2,\"46008\",0\r\n", kASSIGN_MO, 1000, 1000);
    AddMsg("AT+S32K=0\r\n", kASSIGN_MO, 1000, 1000);
}

uint8_t restate;
void loop() {
    AddMsg("AT+CSQ\r\n", kQUERY_MT, 1000, 1000);
    while ((readSendState(0) == kSendReady) || (readSendState(0) == kSending) ||
           (readSendState(0) == kWaitforMsg))
        delay(50);
    restate = readSendState(0);
    readstr = ReadMsgstr(0).c_str();
    Serial.printf("Read state = %d \n", restate);
    if ((readstr.indexOf("+CSQ:") != -1) && (restate == kWaitforRead)) {
        int count = GetstrNumber("+CSQ:", "OK", readstr, numberbuff);
        if (count != 0) {
            Disbuff.fillRect(220, 0, 100, 20, Disbuff.color565(36, 36, 36));
            Disbuff.setCursor(220, 7);
            Disbuff.setTextColor(WHITE);
            Disbuff.setTextSize(1);
            switch (numberbuff[2]) {
                case 0:
                    Disbuff.printf("no service");
                    break;
                case 3:
                    Disbuff.printf("GSM/GPRS  ");
                    if (numberbuff[0] == 99)
                        Disbuff.printf("UnKnown");
                    else if (numberbuff[0] == 31)
                        Disbuff.printf(">-51dBm");
                    else if (numberbuff[0] == 1)
                        Disbuff.printf("UnKnown");
                    else if (numberbuff[0] == 0)
                        Disbuff.printf("<-110dBm");
                    else
                        Disbuff.printf("-%ddBm", 109 - (numberbuff[0] - 2) * 2);
                    break;
                case 5:
                    Disbuff.printf("WCDMA  ");
                    numberbuff[0] -= 100;
                    if (numberbuff[0] == 99)
                        Disbuff.printf("UnKnown");
                    else if (numberbuff[0] == 91)
                        Disbuff.printf(">-25dBm");
                    else if (numberbuff[0] == 0)
                        Disbuff.printf("<-115dBm");
                    else
                        Disbuff.printf("-%ddBm", 115 - (numberbuff[0] - 1));
                    break;
                case 15:
                    Disbuff.printf("TD-SCDMA  ");
                    numberbuff[0] -= 100;
                    if (numberbuff[0] == 99)
                        Disbuff.printf("UnKnown");
                    else if (numberbuff[0] == 91)
                        Disbuff.printf(">-25dBm");
                    else if (numberbuff[0] == 0)
                        Disbuff.printf("<-115dBm");
                    else
                        Disbuff.printf("-%ddBm", 115 - (numberbuff[0] - 1));
                    break;
                case 17:
                    Disbuff.printf("LTE  ");
                    numberbuff[0] -= 100;
                    if (numberbuff[0] == 99)
                        Disbuff.printf("UnKnown");
                    else if (numberbuff[0] == 97)
                        Disbuff.printf(">-44dBm");
                    else if (numberbuff[0] == 0)
                        Disbuff.printf("<-140dBm");
                    else
                        Disbuff.printf("-%ddBm", 140 - (numberbuff[0] - 1));
                    break;
                default:
                    Disbuff.printf("Unknown Card  ");
                    break;
            }
            // Disbuff.setCursor(160,7);
        } else {
            /* code */
        }
    }
    Disbuff.pushSprite(0, 0);
    EraseFirstMsg();
    AddMsg("AT+COPS=3,0\r\n", kASSIGN_MO, 1000, 1000);
    AddMsg("AT+COPS?\r\n", kQUERY_MT, 1000, 1000);
    while ((readSendState(0) == kSendReady) || (readSendState(0) == kSending) ||
           (readSendState(0) == kWaitforMsg))
        delay(50);
    restate = readSendState(0);
    readstr = ReadMsgstr(0).c_str();

    Disbuff.fillRect(0, 20, 320, 10, BLACK);
    Disbuff.setCursor(20, 25);
    Disbuff.setTextColor(WHITE);
    Disbuff.setTextSize(1);
    readstr.trim();
    Disbuff.print(readstr);

    if ((readstr.indexOf("+COPS:") != -1) && (restate == kWaitforRead)) {
        int count = GetstrNumber("+COPS:", "OK", readstr);

        Disbuff.fillRect(100, 0, 120, 20, Disbuff.color565(36, 36, 36));
        Disbuff.setCursor(100, 7);
        Disbuff.setTextColor(WHITE);
        Disbuff.setTextSize(1);

        if (count == 1) {
            Disbuff.printf("NO Operator");
        } else if (count > 1) {
            Disbuff.print(getReString(2).c_str());
        }

        Disbuff.pushSprite(0, 0);
    }
    Serial.print(readstr);
    EraseFirstMsg();

    //*******************************************************
    //  	call +18200000000
    /*
    delay(500);
    M5.update();
    if( M5.BtnA.wasPressed())
    {
        Disbuff.fillRect(0,70,320,10,BLACK);
        Disbuff.setCursor(20,70);
        Disbuff.setTextColor(WHITE);
        Disbuff.setTextSize(1);
        Disbuff.printf("Calling +18200000000");
        Disbuff.pushSprite(0,0);
        AddMsg("ATD18200000000;\r\n", kQUERY_MT, 1000, 1000);
        ATD

        while ((readSendState(0) == kSendReady) || (readSendState(0) ==
    kSending) || (readSendState(0) == kWaitforMsg))delay(50);
        Serial.printf("Read state = %d \n", readSendState(0));
        readstr = ReadMsgstr(0).c_str();
        Serial.print(readstr);
        while(1)
        {
            M5.update();
            if( M5.BtnA.wasPressed()) break;
            delay(100);
            Disbuff.pushSprite(0,0);
        }
        Disbuff.printf("End");
        EraseFirstMsg();
        AddMsg("AT+CHUP\r\n", kASSIGN_MO, 1000, 1000);
    }
    */

    // put your main code here, to run repeatedly:
}
