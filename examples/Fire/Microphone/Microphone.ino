/*
 microphone test

   hardwware:  M5StackFire

 please use the serial plotter in the Arduino IDE to show the signal
 change the baudrate of the plotter to 115200

 September 2018, ChrisMicro
*/

#define M5STACKFIRE_MICROPHONE_PIN 34

void setup() {
    Serial.begin(115200);
}

void loop() {
    int micValue = analogRead(M5STACKFIRE_MICROPHONE_PIN);

    Serial.println(micValue);
    delay(1);
}
