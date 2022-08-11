/*
    Description: Press btnB init to test SIMcard,signal,GPRS. Press btnC get
   host ip and test ping website.
*/
#include <M5Stack.h>
#define RX_PIN 16
#define TX_PIN 17
#define RESET_PIN \
    5  // Module reset resistance is not soldered. if necessary, weld it
       // yourself.

void header(const char *string) {
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(WHITE, BLUE);
    M5.Lcd.fillRect(0, 0, 320, 30, BLUE);
    M5.Lcd.setTextDatum(TC_DATUM);
    M5.Lcd.drawString(string, 160, 3, 4);
    M5.Lcd.drawString("SIGNAL-REFRESH", 160, 220, 2);
    M5.Lcd.drawString("GET-IP&PING", 270, 220, 2);
    M5.Lcd.setCursor(180, 65, 2);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.print("Support USB Serial");
    M5.Lcd.setCursor(150, 85, 2);
    M5.Lcd.print("AT Command Pass-through");
}

String _readSerial(uint32_t timeout) {
    uint64_t timeOld = millis();
    while (!Serial2.available() && !(millis() > timeOld + timeout)) {
        delay(13);
    }
    String str;
    while (Serial2.available()) {
        if (Serial2.available() > 0) {
            str += (char)Serial2.read();
        }
    }
    Serial.print(str);
    return str;
}

void simcard_test() {
    Serial2.print(F("AT+CPIN?\r"));
    String simcard_status = _readSerial(3000);
    Serial.print(simcard_status);
    M5.Lcd.setCursor(0, 40, 2);
    if (simcard_status == "") {
        M5.Lcd.setTextColor(WHITE, RED);
        M5.Lcd.print("No SIM Card");
    } else if (simcard_status.indexOf("READY") != -1) {
        M5.Lcd.setTextColor(WHITE, 0x03E0);
        M5.Lcd.print("SIM Card OK");
    }
}

void signal_test() {
    String csq_data;
    M5.Lcd.setCursor(0, 65, 2);
    M5.Lcd.setTextColor(WHITE, RED);
    M5.Lcd.print("Not Signal");
    M5.Lcd.setTextColor(WHITE, 0x03E0);
    do {
        M5.Lcd.print(".");
        delay(500);
        Serial2.print(F("AT+CSQ\r"));
        csq_data = _readSerial(3000);
    } while (csq_data.indexOf("+CSQ: 0,0") != -1);
    M5.Lcd.setCursor(0, 65, 2);
    M5.Lcd.print(
        csq_data.substring(csq_data.indexOf("+CSQ:"), csq_data.indexOf("OK")));
}

void GPRS_init() {
    Serial2.print(F("AT+CIPSHUT\r"));
    String init_data = _readSerial(4000);
    Serial2.print(F("AT+CSTT=\"CMNET\"\r"));
    init_data = _readSerial(4000);
    Serial2.print(F("AT+CIICR\r"));
    init_data = _readSerial(4000);
    Serial.print(init_data);
    M5.Lcd.setCursor(0, 90, 2);
    if ((init_data.indexOf("ERROR") != -1) ||
        (init_data.indexOf("DEACT") != -1) || (init_data == "")) {
        M5.Lcd.setTextColor(WHITE, RED);
        M5.Lcd.print("NOT");
    } else {
        M5.Lcd.setTextColor(WHITE, 0x03E0);
        M5.Lcd.print("GPRS OK");
    }
}

void ping_test() {
    String ping_data;
    Serial2.print(F("AT+CIFSR\r"));
    M5.Lcd.setCursor(0, 115, 2);
    M5.Lcd.setTextColor(WHITE, YELLOW);
    M5.Lcd.print("PINGING...");
    ping_data = _readSerial(4000);
    M5.Lcd.setCursor(0, 115, 2);
    if ((ping_data.indexOf("ERROR") != -1) || (ping_data == "")) {
        M5.Lcd.setTextColor(WHITE, RED);
        M5.Lcd.print("PING NOT");
    } else {
        M5.Lcd.setTextColor(WHITE, 0x03E0);
        M5.Lcd.print(ping_data);
        Serial2.print(F("AT+CIPPING=\"www.baidu.com\"\r"));
        delay(1000);
        ping_data = _readSerial(4000);
        M5.Lcd.setCursor(0, 160, 1);
        M5.Lcd.print(ping_data);
    }
}

void setup() {
    M5.begin();
    M5.Power.begin();
    header("SIM800L Factory Test");
    Serial2.begin(115200, SERIAL_8N1, 16, 17);
    delay(1000);
    simcard_test();
    signal_test();
    GPRS_init();
}

void loop() {
    // AT instruction write
    if (Serial.available()) {
        Serial2.write(Serial.read());
    }
    // AT instruction result
    if (Serial2.available()) {
        Serial.write(Serial2.read());
    }
    // test SIMcard,signal,GPRS
    if (M5.BtnB.wasPressed()) {
        M5.Lcd.fillRect(0, 30, 150, 190, BLACK);
        simcard_test();
        signal_test();
        GPRS_init();
    }
    // get host ip and test ping website.
    if (M5.BtnC.wasPressed()) {
        ping_test();
    }
    M5.update();
}
