/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit for more information: https://docs.m5stack.com/en/module/lora868
* 获取更多资料请访问: https://docs.m5stack.com/zh_CN/module/lora868
*
* Describe: Module LoRa868.
* Date: 2021/12/26
*******************************************************************************
  LoRa868 Duplex communication.Send messages regularly "HeLoRa World!"
  LoRa868 双工通讯。定期发送消息“HeLoRa World！”

*/
#include <M5LoRa.h>
#include <M5Stack.h>

String outgoing;  // outgoing message.  传出讯息

byte msgCount     = 0;     // count of outgoing messages.  传出消息的计数
byte localAddress = 0xFF;  // address of this device.  此设备的地址
byte destination  = 0xBB;  // destination to send to.  发送目的地

long lastSendTime = 0;     // last send time.  上次发送时间
int interval      = 1000;  // interval between sends.  发送间隔

void setup() {
    M5.begin();
    M5.Power.begin();

    while (!Serial)
        ;

    Serial.println("LoRa Duplex B");

    // override the default CS, reset, and IRQ pins (optional).  覆盖默认的
    // CS、复位和 IRQ 引脚（可选）
    LoRa.setPins();  // set CS, reset, IRQ pin.  设置 CS、复位、IRQ 引脚

    if (!LoRa.begin(
            868E6)) {  // initialize ratio at 868 MHz.  868 MHz 时的初始化比率
        Serial.println("LoRa init failed. Check your connections.");
        while (true)
            ;  // if failed, do nothing.  如果失败，什么都不做
    }

    Serial.println("LoRa init succeeded.");
}

void loop() {
    if (millis() - lastSendTime > interval) {
        String message = "HeLoRa World!";  // send a message.  发送消息
        sendMessage(message);
        Serial.println("Sending " + message);
        M5.Lcd.setTextColor(BLUE);
        M5.Lcd.println("Sending " + message);
        lastSendTime = millis();  // timestamp the message.  给消息加时间戳
        interval     = random(1000) + 500;
    }

    // parse for a packet, and call onReceive with the result:.
    // 解析数据包，并使用结果调用 onReceive:
    onReceive(LoRa.parsePacket());

    if (M5.BtnA.wasPressed()) {
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.clear(BLACK);
    }

    if (M5.BtnB.wasPressed()) {
        reinit();
    }

    M5.update();
}

void reinit() {
    Serial.println("LoRa Duplex Reinitialization");

    // override the default CS, reset, and IRQ pins (optional).  覆盖默认的
    // CS、复位和 IRQ 引脚（可选）
    LoRa.setPins();  // set CS, reset, IRQ pin.  设置 CS、复位、IRQ 引脚

    if (!LoRa.begin(
            868E6)) {  // initialize ratio at 868 MHz.  868 MHz 时的初始化比率
        Serial.println("LoRa init failed. Check your connections.");
        M5.Lcd.setCursor(0, 0);
        M5.Lcd.setTextColor(RED);
        M5.Lcd.println("Init failed!!!");
        while (true)
            ;  // if failed, do nothing.  如果失败，什么都不做
    }

    Serial.println("LoRa init succeeded.");
}

void sendMessage(String outgoing) {
    LoRa.beginPacket();        // start packet.  开始包
    LoRa.write(destination);   // add destination address.  添加目标地址
    LoRa.write(localAddress);  // add sender address.  添加发件人地址
    LoRa.write(msgCount);      // add message ID.  添加消息标识
    LoRa.write(outgoing.length());  // add payload length.  添加有效载荷长度
    LoRa.print(outgoing);           // add payload.  添加有效载荷
    LoRa.endPacket();  // finish packet and send it.  完成数据包并发送
    msgCount++;        // increment message ID.  增加消息 ID
}

void onReceive(int packetSize) {
    if (packetSize == 0)
        return;  // if there's no packet, return.  如果没有包，返回。

    // read packet header bytes:
    int recipient      = LoRa.read();  // recipient address.  收件人地址。
    byte sender        = LoRa.read();  // sender address.  发件人地址。
    byte incomingMsgId = LoRa.read();  // incoming msg ID.  传入的消息 ID。
    byte incomingLength = LoRa.read();  // incoming msg length.  传入消息长度。

    String incoming = "";

    while (LoRa.available()) {
        incoming += (char)LoRa.read();
    }

    if (incomingLength !=
        incoming.length()) {  // check length for error.  检查错误长度
        Serial.println("error: message length does not match length");
        return;  // skip rest of function.  跳过其余功能
    }

    // if the recipient isn't this device or broadcast,.
    // 如果收件人不是此设备或广播，
    if (recipient != localAddress && recipient != 0xFF) {
        Serial.println("This message is not for me.");
        return;  // skip rest of function.  跳过其余功能
    }

    // if message is for this device, or broadcast, print details:.
    // 如果消息是针对此设备或广播的，则打印详细信息:
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
