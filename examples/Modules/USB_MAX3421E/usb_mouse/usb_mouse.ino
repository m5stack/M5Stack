/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/module/usb
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/module/usb
*
* describe: USB.
* date：2021/9/1
*******************************************************************************
  This sample code demonstrates the normal use of a USB_Host_SHield_Library_2.0 object.
  此示例代码演示了 USB_Host_SHield_Library_2.0 对象的正常使用。
*/

#include <M5Stack.h>
// #include <SPI.h>
#include <Usb.h>
#include <hiduniversal.h>
#include <hidboot.h>
#include <usbhub.h>
#include "M5Mouse.h"

USB Usb;
USBHub Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE> HidMouse(&Usb);
MouseRptParser Prs;

int StaPotX = 160, StaPotY = 120;

void Mouse_Pointer(int PotDataX, int PotDataY)
{
  static int OldDataX, OldDataY;

  if ((StaPotX + PotDataX) <= 320 && (StaPotX + PotDataX) > 0)
    StaPotX = (StaPotX + PotDataX);
  else if ((StaPotX + PotDataX) <= 0)
    StaPotX = 0;
  else
    StaPotX = 319;

  if ((StaPotY + PotDataY) <= 240 && (StaPotY + PotDataY) > 0)
    StaPotY = (StaPotY + PotDataY);
  else if ((StaPotY + PotDataY) <= 0)
    StaPotY = 0;
  else
    StaPotY = 239;

  // clear draw
  if (OldDataX != StaPotX || OldDataY != StaPotY)
  {
    M5.Lcd.drawLine(OldDataX + 0, OldDataY + 0, OldDataX + 0, OldDataY + 10, BLACK);
    M5.Lcd.drawLine(OldDataX + 0, OldDataY + 0, OldDataX + 7, OldDataY + 7, BLACK);
    M5.Lcd.drawLine(OldDataX + 4, OldDataY + 7, OldDataX + 7, OldDataY + 7, BLACK);
    M5.Lcd.drawLine(OldDataX + 4, OldDataY + 7, OldDataX + 0, OldDataY + 10, BLACK);
    M5.Lcd.drawLine(OldDataX + 3, OldDataY + 7, OldDataX + 6, OldDataY + 12, BLACK);
  }

  // draw
  M5.Lcd.drawLine(StaPotX + 0, StaPotY + 0, StaPotX + 0, StaPotY + 10, WHITE);
  M5.Lcd.drawLine(StaPotX + 0, StaPotY + 0, StaPotX + 7, StaPotY + 7, WHITE);
  M5.Lcd.drawLine(StaPotX + 4, StaPotY + 7, StaPotX + 7, StaPotY + 7, WHITE);
  M5.Lcd.drawLine(StaPotX + 4, StaPotY + 7, StaPotX + 0, StaPotY + 10, WHITE);
  M5.Lcd.drawLine(StaPotX + 3, StaPotY + 7, StaPotX + 6, StaPotY + 12, WHITE);

  OldDataX = StaPotX;
  OldDataY = StaPotY;
}

void setup()
{
  M5.begin();
  M5.Power.begin();
  Serial.println("M5USB_Demo Start...");
  if (Usb.Init() == -1)
    Serial.println("USB Host Init Error");
  HidMouse.SetReportParser(0, (HIDReportParser *)&Prs);
  delay(200);
}

void loop()
{
  Usb.Task();
  if(Usb.getUsbTaskState() == USB_STATE_RUNNING)
  {
    Mouse_Pointer(mou_px, mou_py);
    mou_px = 0;
    mou_py = 0;
    if (mou_button == 1)// left button pressed
      M5.Lcd.drawCircle(StaPotX, StaPotY, 1, WHITE);
    if (mou_button == 2)// right button pressed
      M5.Lcd.drawCircle(StaPotX, StaPotY, 1, GREEN);
    if (mou_button == 4)// middle button pressed
      M5.Lcd.fillScreen(BLACK);
  }
}
