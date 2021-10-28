/*
*******************************************************************************
* Copyright (c) 2021 by M5Stack
*                  Equipped with M5Core sample source code
*                          配套  M5Core 示例源代码
* Visit the website for more information：https://docs.m5stack.com/en/core/gray
* 获取更多资料请访问：https://docs.m5stack.com/zh_CN/core/gray
*
* describe：Finger Unit example
* date：2021/10/28
*******************************************************************************
Description: FINGER UNIT use case: Press the left button to enter the fingerprint entry mode. Press the middle button to enter the fingerprint identification mode，Right click to delete all saved users
FINGER UNIT 使用案例按左键进入指纹录入模式，短按中间键进入指纹识别模式，按下右键删除所有保存的用户
*/

#include <M5Stack.h>
#include "M5_FPC1020A.h"

FingerPrint FP_M;

void setup() {
  M5.begin();
  M5.Power.begin();
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setTextSize(2);
  FP_M.begin();
  M5.Lcd.drawString("Add User", 10, 210);
  M5.Lcd.drawString("Verify", 125, 210);
  M5.Lcd.drawString("Del User", 220, 210);

  M5.Lcd.setCursor(0, 20);
  M5.Lcd.println("Finger Unit TEST");
  M5.Lcd.println("1. delete all user");
  M5.Lcd.println("2. add a user fingerprint");
  M5.Lcd.println("3. verify user permission");
}

void loop(){
  uint8_t res1;
  //ButtonA: Add user.  添加用户
  if(M5.BtnA.wasPressed()){
    M5.Lcd.fillRect(0, 0, 320, 200, BLACK);
    Serial.println("Start Fingerprint Typing");
    Serial.println("Put Your Finger on the sensor");
    Serial.println("wating....");

    M5.Lcd.println("Start Fingerprint Typing");
    M5.Lcd.println("Put Your Finger on the sensor");
    M5.Lcd.println("wating....");

    res1 = FP_M.fpm_addUser(22,1);  //(user_num, userPermission)
    if(res1 == ACK_SUCCESS){
      M5.Lcd.println("Success");
      Serial.println("Success");
    }else{
      M5.Lcd.println("Fail");
      Serial.println("Fail");
    }
  }
//ButtonB: Matching.  匹配指纹
  if(M5.BtnB.wasPressed()){

    M5.Lcd.fillRect(0, 0, 320, 100, BLACK);
    Serial.println("Start Verify Fingerprint");
    res1 = FP_M.fpm_compareFinger();
    if(res1 == ACK_SUCCESS){

      Serial.println("Success");
      Serial.print("User ID: ");
      Serial.println(FP_M.fpm_getUserId());

      M5.Lcd.println("Success");
      M5.Lcd.print("User ID: ");
      M5.Lcd.println(FP_M.fpm_getUserId());

    }else{
      Serial.println("No Such User");

      M5.Lcd.println("No Such User");
    }
  }
//ButtonC: Delete All User.  删除所有用户
  if(M5.BtnC.wasPressed()){
    M5.Lcd.fillRect(0, 0, 320, 100, BLACK);
    Serial.println("Start Delete Fingerprint");
    M5.Lcd.println("Start Delete Fingerprint");
    res1 = FP_M.fpm_deleteAllUser();
    if(res1 == ACK_SUCCESS){
      Serial.println("Delete All User Successful");
      M5.Lcd.println("Delete All User Successful");
    }
    else{
      Serial.println("Delete All User Failed");
      M5.Lcd.println("Delete All User Failed");
    }
  }
  M5.Lcd.setCursor(0, 20);
  M5.update();
}
