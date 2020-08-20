/*
    Description: Use LoRaWAN Module to send "Hello World" message. Click button A to clear the screen, click button B to switch the 433 band, and click button C to switch the 868 band.
*/

#include <M5Stack.h>
#include <LoRaWan.h>

#define SerialUSB Serial

unsigned char buffer[128] = {
    0,
};
uint8_t flag_test_868 = 0;

void key_scan(void *arg)
{
  while (1)
  {
    onReceive();
    delay(10);
    // lora.loraDebug();
  }
  vTaskDelete(NULL);
}

void setup(void)
{
  M5.begin();
  M5.Power.begin();
  SerialUSB.begin(9600);
  lora.init();
  delay(2000); // must delay for lorawan power on
  lora.initP2PMode(433, SF12, BW500, 8, 8, 20);
  M5.Lcd.setTextFont(2);
  M5.Lcd.println("Device B");
  M5.Lcd.setTextColor(BLUE);
  int core = xPortGetCoreID();
  // M5.Lcd.println(core);
  xTaskCreatePinnedToCore(key_scan, "key_scan", 3096, NULL, 5, NULL, 0);
}

void loop(void)
{
  // lora.loraDebug();
  if (M5.BtnA.wasPressed())
  {
    M5.Lcd.setCursor(0, 18);
    M5.Lcd.fillRect(0, 18, 320, 222, BLACK);
  }
  if (M5.BtnB.wasPressed()) //send data (433MHz)
  {
    flag_test_868 = 0;
    M5.Lcd.setTextColor(BLUE);
    init_433();
    send_data();
    delay(300);
    Serial2.print("AT+TEST=RXLRPKT\r\n");
        M5.Lcd.println("433M Init and Send \"Hello World\"...");
  }
  if (M5.BtnC.wasPressed()) //send data (868MHz)
  {
    flag_test_868 = 1;
    M5.Lcd.setTextColor(YELLOW);
    init_868();
    send_data();
    delay(300);
    Serial2.print("AT+TEST=RXLRPKT\r\n");
    M5.Lcd.println("868M Init and Send \"Hello World\"...");
  }
  M5.update();
}

void init_433()
{
  lora.initP2PMode(433, SF12, BW500, 8, 8, 20);
}

void init_868()
{
  lora.initP2PMode(868, SF12, BW500, 8, 8, 20);
}

void send_data()
{
  lora.transferPacketP2PMode("hello world");
}

void onReceive()
{
  short length = 0;
  short rssi = 0;

  memset(buffer, 0, 128);
  length = lora.receivePacketP2PMode(buffer, 128, &rssi, 1);

  if (length)
  {
    SerialUSB.print("Length is: ");
    SerialUSB.println(length);
    SerialUSB.print("RSSI is: ");
    SerialUSB.println(rssi);
    SerialUSB.print("Data is: ");
    if (flag_test_868)
    {
      M5.Lcd.print("868M Recv:  ");
    }
    else
    {
      M5.Lcd.print("433M Recv:  ");
    }
    for (unsigned char i = 0; i < length; i++)
    {
      SerialUSB.print((char)buffer[i]);
      M5.Lcd.print((char)buffer[i]);
      //   SerialUSB.print("0x");
      //   SerialUSB.print(buffer[i], HEX);
      //   SerialUSB.print(" ");
    }
    SerialUSB.println();
    M5.Lcd.println();
  }
}
