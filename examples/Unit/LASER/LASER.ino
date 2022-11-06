/*
    Description: LASER Unit wireless UART application: burn the program to two
M5Cores     And connect LASER.TX and LASER.RX to PORTC port respectively. Point
LASER.TX to LASER.RX and press the button on the panel to send characters to the
receiver of LASER.RX.
*/

#include <M5Stack.h>

char ch;
// serial 2 write and read
//#define RX
void setup() {
    M5.begin();
    M5.Power.begin();
    Serial.begin(115200);

    // Serial2.begin(unsigned long baud, uint32_t config, int8_t rxPin, int8_t
    // txPin, bool invert)
    Serial2.begin(9600, SERIAL_8N1, 16, 17);
    pinMode(5, OUTPUT);
    digitalWrite(5, 1);

    M5.Lcd.setTextSize(4);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setCursor(60, 50);
#ifdef RX
    M5.Lcd.print("LASER RX");
#elif defined TX
    M5.Lcd.print("LASER TX");
#else
    M5.Lcd.setCursor(30, 50);
    M5.Lcd.print("LASER TX/RX");
    M5.Lcd.setCursor(50, 200);
    M5.Lcd.print('A');
    M5.Lcd.setCursor(150, 200);
    M5.Lcd.print('B');
    M5.Lcd.setCursor(240, 200);
    M5.Lcd.print('C');
#endif
    M5.Lcd.setCursor(0, 100);
}

void loop() {
#ifdef RX
    M5.update();
    if (Serial2.available()) {
        char ch = Serial2.read();
        M5.Lcd.print(ch);
    }

    if (M5.BtnA.wasReleased()) {
        M5.Lcd.clear();
        M5.Lcd.setCursor(0, 0);
    }
#elif defined TX
    Serial2.write('A');
    delay(50);
#else
    if (M5.BtnA.wasReleased()) {
        ch = 'A';
        Serial2.write(ch);
    } else if (M5.BtnB.wasReleased()) {
        ch = 'B';
        Serial2.write(ch);
    } else if (M5.BtnC.wasReleased()) {
        ch = 'C';
        Serial2.write(ch);
    }
    M5.update();
    if (Serial2.available()) {
        char ch = Serial2.read();
        M5.Lcd.print(ch);
    }
#endif
}
