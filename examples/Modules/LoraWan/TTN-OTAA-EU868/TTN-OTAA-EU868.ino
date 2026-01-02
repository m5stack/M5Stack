

#include <M5Stack.h>
#include <LoRaWan.h>

//LoRa
char buffer[256];
long sentMillis = 0;
long currentMillis = 0;
long interval = 15000;
//byte payload[2];

void setup() {
  /* Prepare M5STACK */
  M5.begin();

  initlora();

  Serial.println("Started");
}

void loop() {

  currentMillis = millis();
  if (currentMillis - sentMillis > interval) {
    sendobject();
  }

  M5.update();
}

void initlora() {

  lora.init();

  delay(1000);

  memset(buffer, 0, 256);
  lora.getVersion(buffer, 256, 1);
  Serial.print(buffer);

  memset(buffer, 0, 256);
  lora.getId(buffer, 256, 1);
  Serial.print(buffer);

  // void setId(char *DevAddr, char *DevEUI, char *AppEUI);
  lora.setId(NULL, "1234567890", "1234567890");
  // setKey(char *NwkSKey, char *AppSKey, char *AppKey);
  lora.setKey(NULL, NULL, "12345678901234567890");

  lora.setDeviceMode(LWOTAA);
  lora.setDataRate(DR5, EU868);

  lora.setChannel(0, 868.1);
  lora.setChannel(1, 868.3);
  lora.setChannel(2, 868.5);
  lora.setChannel(3, 867.1);
  lora.setChannel(4, 867.3);
  lora.setChannel(5, 867.5);
  lora.setChannel(6, 867.7);
  lora.setChannel(7, 867.9);

  lora.setReceiveWindowFirst(0, 868.1);
  lora.setReceiveWindowSecond(869.525, DR3);

  lora.setPower(14);
  lora.setPort(1);
  lora.setAdaptiveDataRate(true);
  //lora.setDutyCycle(false);
  //lora.setJoinDutyCycle(false);
  while (!lora.setOTAAJoin(JOIN, 10));
}

void sendobject() {
  bool result = false;

  sentMillis = millis();

  Serial.println("Sending");

  result = lora.transferPacket("Hello World!", 5);
  //result = lora.transferPacket(payload, sizeof(payload), 5);

  if (result == true) {
    Serial.println("Sent");


  } else {
    Serial.println("Error");
  }
}
