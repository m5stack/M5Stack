#include <Arduino.h>
#include <U8g2lib.h>

#define BtnPin 35
#define BuzzerPin 26

U8G2_SH1107_64X128_F_4W_HW_SPI u8g(U8G2_R0,14, /* dc=*/ 27, /* reset=*/ 33);

int btnClicked;
int oledState;
unsigned long initTime;

void setup() {
  u8g.begin();
  u8g.setFont(u8g2_font_wqy12_t_gb2312);
  u8g.enableUTF8Print();
   
  pinMode(BtnPin, INPUT_PULLUP);
  attachInterrupt(BtnPin,pressed,FALLING);
  
  pinMode(BuzzerPin, OUTPUT);
  digitalWrite(BuzzerPin, LOW);
  
  btnClicked=0;
  oledState=0;
  
  delay(500);
  initTime=millis();
}

void pressed()
{
  btnClicked=1;
}

void draw(void)
{
  u8g.firstPage();  
  do {
    u8g.drawUTF8(2,14*1+4,"    道忽思");
    u8g.drawUTF8(2,14*2+4,"    ，已君");
    u8g.drawUTF8(2,14*3+4,"古  努晚令");
    u8g.drawUTF8(2,14*4+4,"诗  力。人");
    u8g.drawUTF8(2,14*5+4,"十  加弃老");
    u8g.drawUTF8(2,14*6+4,"九  餐捐，");
    u8g.drawUTF8(2,14*7+4,"首  饭勿岁");
    u8g.drawUTF8(2,14*8+4,"    。复月");
    
    u8g.drawLine(10+2,14*2+4,12+2,14*2+4);
    u8g.drawLine(12+2,14*2-1,12+2,14*2+4);
    u8g.drawLine(12+2,14*2-1,0+2,14*2-1);
    u8g.drawLine(0+2,14*2+1,0+2,14*2-1);
    u8g.drawLine(0+2,14*2+1,10+2,14*2+1);
    u8g.drawLine(10+2,14*2+1,10+2,14*2+3);
    
    u8g.drawLine(1+2,14*8-4,-1+2,14*8-4);
    u8g.drawLine(-1+2,14*8+1,-1+2,14*8-4);
    u8g.drawLine(-1+2,14*8+1,11+2,14*8+1);
    u8g.drawLine(11+2,14*8-1,11+2,14*8+1);
    u8g.drawLine(11+2,14*8-1,1+2,14*8-1);
    u8g.drawLine(1+2,14*8-1,1+2,14*8-3);
  } while( u8g.nextPage() );
}
void loop() {
  if(btnClicked==1)
  {
    oledState=(oledState+1)%2;
    if(oledState==1)u8g.noDisplay();
    else u8g.display();
    initTime=millis();
    btnClicked=0;
  }
  if (millis()-initTime>60000)esp_deep_sleep_start();
  draw();
  delay(1);
}
