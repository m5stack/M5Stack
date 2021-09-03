/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/unit/finger
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/unit/finger
*
* describe: Finger.
* date：2021/8/26
*******************************************************************************
*/
#include <M5Stack.h>
#include "M5_FPC1020A.h"

uint8_t userNum;           //User number
FingerPrint FP_M;

void CleanScreen()
{
  M5.Lcd.fillRect(0,50,400,300,BLACK);
  M5.Lcd.setCursor(0, 50);
  M5.Lcd.setTextSize(2);
  userNum = FP_M.fpm_getUserNum();  //Get users Numbers.  获取用户数量
  M5.Lcd.print("userNum:");
  M5.Lcd.println(userNum);
}

void setup() {
  M5.begin();
  M5.Power.begin();
  Serial.begin(115200);
  Serial2.begin(19200, SERIAL_8N1, 16, 17);
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setCursor(20, 0);
  M5.Lcd.println("Finger example");
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.fillRect(0,50,400,300,BLACK);
  M5.Lcd.setCursor(0, 50);
  M5.Lcd.setTextSize(2);
  userNum = FP_M.fpm_getUserNum();
  M5.Lcd.print("userNum:");
  M5.Lcd.println(userNum);
}

void loop(){
  M5.update();
  uint8_t res1;
  //ButtonA: Add user.  添加用户
  if(M5.BtnA.wasPressed()){
    CleanScreen();
    M5.Lcd.println("Fingerprint Typing");

    res1 = FP_M.fpm_addUser(userNum,1);
    if(res1 == ACK_SUCCESS){
      M5.Lcd.println("Success");
    }
    else if(res1 == ACK_FAIL){
      M5.Lcd.println("Fail");
    }
    else if(res1 == ACK_FULL){
      M5.Lcd.println("Full");
    }
    else{
      M5.Lcd.println("Timeout");
    }
    userNum++;
  }
//ButtonB: Matching.  匹配指纹
  if(M5.BtnB.wasPressed()){
    CleanScreen();
    M5.Lcd.println("Matching");

    res1 = FP_M.fpm_compareFinger();
    if(res1 == ACK_SUCCESS){
      M5.Lcd.println("Success");
    }
    if(res1 == ACK_NOUSER){
      M5.Lcd.println("No Such User");
    }
    if(res1 == ACK_TIMEOUT){
      M5.Lcd.println("Timeout");
    }
  }
//ButtonC: Delete All User.  删除所有用户
  if(M5.BtnC.wasPressed()){
    res1 = FP_M.fpm_deleteAllUser();
    CleanScreen();

    if(res1 == ACK_SUCCESS){
      M5.Lcd.println("Delete All User Successful");
    }
    else{
      M5.Lcd.println("Delete All User Failed");
    }
  }
}
