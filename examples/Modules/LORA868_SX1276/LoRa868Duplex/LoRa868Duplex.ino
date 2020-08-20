/*
    Description: LoRa868 Duplex communication.Send messages regularly "HeLoRa World!"
*/
#include <M5Stack.h>
#include <M5LoRa.h>

String outgoing;              // outgoing message

byte msgCount = 0;            // count of outgoing messages
byte localAddress = 0xFF;     // address of this device
byte destination = 0xBB;      // destination to send to

long lastSendTime = 0;        // last send time
int interval = 1000;          // interval between sends

void setup() {
  M5.begin();
  M5.Power.begin();
  while (!Serial);

  Serial.println("LoRa Duplex B");

  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins();// set CS, reset, IRQ pin

  if (!LoRa.begin(868E6)) {             // initialize ratio at 868 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true);                       // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");
}

void loop() {
  if (millis() - lastSendTime > interval) {
    String message = "HeLoRa World!";   // send a message
    sendMessage(message);
    Serial.println("Sending " + message);
    M5.Lcd.setTextColor(BLUE);
    M5.Lcd.println("Sending " + message);
    lastSendTime = millis();            // timestamp the message
    interval = random(1000) + 500;
  }

  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());

  if(M5.BtnA.wasPressed()){
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.clear(BLACK);
  }

  if(M5.BtnB.wasPressed()){
    reinit();
  }

  M5.update();
}

void reinit(){
  Serial.println("LoRa Duplex Reinitialization");

  // override the default CS, reset, and IRQ pins (optional)
  LoRa.setPins();// set CS, reset, IRQ pin

  if (!LoRa.begin(868E6)) {             // initialize ratio at 868 MHz
    Serial.println("LoRa init failed. Check your connections.");
    M5.Lcd.setCursor(0, 0);
    M5.Lcd.setTextColor(RED);
    M5.Lcd.println("Init failed!!!");
    while (true);                       // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");
}

void sendMessage(String outgoing) {
  LoRa.beginPacket();                   // start packet
  LoRa.write(destination);              // add destination address
  LoRa.write(localAddress);             // add sender address
  LoRa.write(msgCount);                 // add message ID
  LoRa.write(outgoing.length());        // add payload length
  LoRa.print(outgoing);                 // add payload
  LoRa.endPacket();                     // finish packet and send it
  msgCount++;                           // increment message ID
}

void onReceive(int packetSize) {
  if (packetSize == 0) return;          // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();          // recipient address
  byte sender = LoRa.read();            // sender address
  byte incomingMsgId = LoRa.read();     // incoming msg ID
  byte incomingLength = LoRa.read();    // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length()) {   // check length for error
    Serial.println("error: message length does not match length");
    return;                             // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");
    return;                             // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println("RSSI: " + String(LoRa.packetRssi()));
  Serial.println("Snr: " + String(LoRa.packetSnr()));
  Serial.println();

  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.println("Message: " + incoming);
}
