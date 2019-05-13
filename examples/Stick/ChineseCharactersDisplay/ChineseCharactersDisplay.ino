/*Coding by wjxjmj*/
#include <Arduino.h>
#include <U8g2lib.h>

#define BtnPin 35
#define BuzzerPin 26

U8G2_SH1107_64X128_F_4W_HW_SPI u8g2(U8G2_R0,14, /* dc=*/ 27, /* reset=*/ 33);

int btnState;
int oledState;
unsigned long initTime;
unsigned long duration; /* Turn off the oled screen if no button is pushed in duration time */

void setup() {
  u8g2.begin();
  u8g2.setFont(u8g2_font_wqy12_t_gb2312); /*12*12 Chinese characters using GB2312*/
  u8g2.enableUTF8Print();
   
  pinMode(BtnPin, INPUT_PULLUP);
  attachInterrupt(BtnPin,pressed,FALLING); /*Chech if the button is pushed using interrupt*/
  
  pinMode(BuzzerPin, OUTPUT);
  digitalWrite(BuzzerPin, LOW);/*Keep the buzzer quiet*/
  
  btnState=0;
  oledState=0;
  duration=15000; /*keep screen on in 15 second*/
  delay(500);
  initTime=millis();
}

void pressed()
{
  btnState=1;
}

void draw(void)
{
  u8g2.firstPage();  
  do {
    /*Chinese characters strings*/
    u8g2.setColorIndex(1);
    u8g2.drawUTF8(2,14*1+4,"    道忽思");
    u8g2.drawUTF8(2,14*2+4,"    ，已君");
    u8g2.drawUTF8(2,14*3+4,"古  努晚令");
    u8g2.drawUTF8(2,14*4+4,"诗  力。人");
    u8g2.drawUTF8(2,14*5+4,"十  加弃老");
    u8g2.drawUTF8(2,14*6+4,"九  餐捐，");
    u8g2.drawUTF8(2,14*7+4,"首  饭勿岁");
    u8g2.drawUTF8(2,14*8+4,"    。复月");

    /*Some punctuations are missing, draw them using u8g2 api
      Upper quotation marks for title of books */
    u8g2.drawFrame(0+2,14*2-1,12,5); 
    u8g2.drawFrame(0+2,14*2+1,10,3);
    u8g2.setColorIndex(0);
    u8g2.drawFrame(0+2,14*2+2,9,2);

    /*Lower quotation marks for title of books */
    u8g2.setColorIndex(1);
    u8g2.drawFrame(0+1,14*8-3,12,5); 
    u8g2.drawFrame(0+3,14*8-3,10,3);
    u8g2.setColorIndex(0);
    u8g2.drawFrame(0+4,14*8-3,9,2);
  } while( u8g2.nextPage() );
}
void loop() {
  if(btnState==1)
  {
    oledState=(oledState+1)%2; 
    if(oledState==1)u8g2.noDisplay();/*turn off screen if it is on*/
    else u8g2.display();             /*and vice versa*/
    initTime=millis();/*renew the time if button is pushed*/
    btnState=0;       /*ready for next push of button*/
  }
  if (millis()-initTime>duration)esp_deep_sleep_start(); /*Entering power save mode*/
  draw();
  delay(1);
}