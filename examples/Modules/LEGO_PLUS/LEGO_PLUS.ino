/*
    Description: Use LEGO PLUS Module to drive the LEGO motor to rotate and monitor the encoder value.
*/
#include <M5Stack.h>
#include <Wire.h>
#include "Free_Fonts.h" 
#include "utility/CommUtil.h"

#define SLAVE_ADDR      0x56
#define MOTOR_ADDR_BASE      0x00
#define ENCODER_ADDR_BASE      0x08
#define STEP_V      51
#define FRONT 4
#define X_LOCAL  60
#define Y_LOCAL  80
#define XF  30
#define YF  30

int16_t Speed = 0;
CommUtil Util;


/*************************************************
Function:MotorRun
Description: Motor forward and reverse API
Input:
      n:Motor 0 to motor 3
  Speed:Speed value from -255 to +255,when speed=0,The motor stopped. 
  
Return: Successful return 1
Others: 
*************************************************/
int32_t MotorRun(uint8_t n,int16_t Speed){
    if(n>3)  
         return 0;
  
    if(Speed <= -255)
        Speed = -255;
  
    if(Speed >=255)
        Speed = 255;
       
    Util.writeBytes(SLAVE_ADDR,MOTOR_ADDR_BASE+n*2,(uint8_t *)&Speed,2);
    
    return 1;
}

/*************************************************
Function:ReadEncoder
Description: Motor rotation code value
Input:
      n:Motor 0 to motor 3 
  
Return: Successful return 1
Others: 
*************************************************/
int32_t ReadEncoder(uint8_t n){
    uint8_t  dest[4]={0};

    if(n>3)  
        return 0;
    
    Util.readBytes(SLAVE_ADDR,ENCODER_ADDR_BASE+n*4,4,dest);
    
    return *((int32_t*)dest);
}


/*************************************************
Function:header
Description:The UI title
Input:
Return: 
Others: 
*************************************************/
void header(const char *string, uint16_t color){
    M5.Lcd.fillScreen(color);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_MAGENTA, TFT_BLUE);
    M5.Lcd.fillRect(0, 0, 320, 30, TFT_BLUE);
    M5.Lcd.setTextDatum(TC_DATUM);
    M5.Lcd.drawString(string, 160, 3, 4);
   
}

/*************************************************
Function:motor_demo
Description:Press the button to control motor rotation,
            and coded values are displayed in real time.
Input:
Return: 
Others:A -> Speed--   B -> Speed=0   C -> Speed++  
*************************************************/
void motor_demo(void){
  uint8_t BtnFlag = 0;
  M5.update();
  
  if (M5.BtnA.wasReleased()) {
    Speed -= STEP_V;

    if(Speed <= -255)
    Speed = -255;

    BtnFlag = 1;
  }else if (M5.BtnB.wasReleased()) {
    Speed = 0;
    BtnFlag = 1;
  }
  else if (M5.BtnC.wasReleased()) {
    Speed += STEP_V;

    if(Speed >=255)
    Speed = 255;

    BtnFlag = 1;
  }  

  if(BtnFlag == 1){
      BtnFlag = 0;
      for(int i =0;i<4;i++){
          MotorRun(i,Speed);    
          M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + YF*i , FRONT);
          M5.Lcd.printf("S%d:%d       \n",i,Speed);
      }
  }

  for(int i=0;i<4;i++){
    M5.Lcd.setCursor(X_LOCAL + XF*4, Y_LOCAL + YF*i , FRONT);
    M5.Lcd.printf("E%d:%d           \n",i,ReadEncoder(i));
  }
}

void setup() {
    M5.begin();
    M5.Power.begin();
    Wire.begin();
    Serial.begin(115200);
    M5.Lcd.fillScreen(TFT_BLACK);               
    header("L E G O +", TFT_BLACK);
    M5.Lcd.setTextColor(TFT_WHITE,TFT_BLACK);
    for(int i =0;i<4;i++){
      M5.Lcd.setCursor(X_LOCAL, Y_LOCAL + YF*i , FRONT);
      M5.Lcd.printf("S%d:%d     \n",i,Speed);
   }   
}

void loop() {
    motor_demo();   
}
