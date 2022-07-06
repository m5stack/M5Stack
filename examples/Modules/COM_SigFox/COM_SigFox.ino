/*
    Description: Get device ID,PAC and Send "00FFEE" to Dongle.
*/

#include <M5Stack.h>

#define rxPin 16
#define txPin 17

// Set to 0 if you don't need to see the messages in the console
#define DEBUG 1

// Message buffer
uint8_t msg[12];

// the setup function runs once when you press reset or power the board
void setup() {
    M5.begin();
    Serial2.begin(9600, SERIAL_8N1, rxPin, txPin);

    delay(2000);
    getID();
    delay(1000);
    getPAC();
    delay(1000);
}

// the loop function runs over and over again forever
void loop() {
    Serial2.write("AT$SF=00FFEE\r\n");
    delay(60000);
}

// Get Sigfox ID
String getID() {
    String id = "";
    char output;

    Serial2.write("AT$I=10\r\n");
    while (!Serial2.available()) {
    }
    while (Serial2.available()) {
        output = Serial2.read();
        id += output;
        delay(10);
    }

    if (DEBUG) {
        Serial.println("Sigfox Device ID: ");
        Serial.println(id);
        M5.Lcd.println(id);
    }

    return id;
}

// Get PAC number
String getPAC() {
    String pac = "";
    char output;

    Serial2.write("AT$I=11\r\n");
    while (!Serial2.available()) {
    }

    while (Serial2.available()) {
        output = Serial2.read();
        pac += output;
        delay(10);
    }

    if (DEBUG) {
        Serial.println("PAC number: ");
        Serial.println(pac);
        M5.Lcd.println(pac);
    }

    return pac;
}
