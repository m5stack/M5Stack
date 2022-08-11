/*
  LoRa Duplex communication with Spreading Factor

  Sends a message every half second, and polls continually
  for new incoming messages. Sets the LoRa radio's spreading factor.

  Spreading factor affects how far apart the radio's transmissions
  are, across the available bandwidth. Radios with different spreading
  factors will not receive each other's transmissions. This is one way you
  can filter out radios you want to ignore, without making an addressing scheme.

  Spreading factor affects reliability of transmission at high rates, however,
  so avoid a hugh spreading factor when you're sending continually.

  See the Semtech datasheet, http://www.semtech.com/images/datasheet/sx1276.pdf
  for more on Spreading Factor.

  created 28 April 2017
  by Tom Igoe
*/
#include <M5Stack.h>
#include <M5LoRa.h>

byte msgCount     = 0;     // count of outgoing messages
int interval      = 2000;  // interval between sends
long lastSendTime = 0;     // time of last packet send

void header(const char *string, uint16_t color) {
    M5.Lcd.fillScreen(color);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_MAGENTA, TFT_BLUE);
    M5.Lcd.fillRect(0, 0, 320, 30, TFT_BLUE);
    M5.Lcd.setTextDatum(TC_DATUM);
    M5.Lcd.drawString(string, 160, 3, 4);
}

void setup() {
    M5.begin();  // initialize serial
    M5.Power.begin();
    while (!Serial)
        ;

    header("LoRa Set spreading factor", TFT_BLACK);
    M5.Lcd.setTextFont(2);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
    M5.Lcd.drawString("Please use serial port to view data.", 0, 35, 2);

    Serial.println("LoRa Duplex - Set spreading factor");

    // override the default CS, reset, and IRQ pins (optional)
    LoRa.setPins();  // set CS, reset, IRQ pin

    if (!LoRa.begin(433E6)) {  // initialize ratio at 915 MHz
        Serial.println("LoRa init failed. Check your connections.");
        while (true)
            ;  // if failed, do nothing
    }

    LoRa.setSpreadingFactor(8);  // ranges from 6-12,default 7 see API docs
    Serial.println("LoRa init succeeded.");
}

void loop() {
    if (millis() - lastSendTime > interval) {
        String message = "HeLoRa World! ";  // send a message
        message += msgCount;
        sendMessage(message);
        Serial.println("Sending " + message);
        lastSendTime = millis();             // timestamp the message
        interval     = random(2000) + 1000;  // 2-3 seconds
        msgCount++;
    }

    // parse for a packet, and call onReceive with the result:
    onReceive(LoRa.parsePacket());
}

void sendMessage(String outgoing) {
    LoRa.beginPacket();    // start packet
    LoRa.print(outgoing);  // add payload
    LoRa.endPacket();      // finish packet and send it
    msgCount++;            // increment message ID
}

void onReceive(int packetSize) {
    if (packetSize == 0) return;  // if there's no packet, return

    // read packet header bytes:
    String incoming = "";

    while (LoRa.available()) {
        incoming += (char)LoRa.read();
    }

    Serial.println("Message: " + incoming);
    Serial.println("RSSI: " + String(LoRa.packetRssi()));
    Serial.println("Snr: " + String(LoRa.packetSnr()));
    Serial.println();
}
