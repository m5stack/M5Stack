/*

Simple reaction game for M5 Stick

Requires the library below

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification, 
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list 
    of conditions and the following disclaimer.
    
  * Redistributions in binary form must reproduce the above copyright notice, this 
    list of conditions and the following disclaimer in the documentation and/or other 
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND 
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF 
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, 
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.  

*/

#include <Arduino.h>
#include <U8x8lib.h>

#define BtnPin 35
#define BuzzerPin 26

int pos = 0; //balls position
int score = 0; //score counter
int pace = 100; //the speed of the game

U8X8_SH1107_64X128_4W_HW_SPI u8x8(14, /* dc=*/ 27, /* reset=*/ 33);

void setup() {
    u8x8.begin();
    pinMode(BtnPin, INPUT_PULLUP);
    pinMode(BuzzerPin, OUTPUT);
    u8x8.setFont(u8x8_font_chroma48medium8_r);

    //set buzzer low at start
    digitalWrite(BuzzerPin, LOW);
    u8x8.drawString(0,0,"Catch");
    u8x8.drawString(0,1,"the ball");
    u8x8.drawString(0,2,"in the");
    u8x8.drawString(0,3,"middle");
    u8x8.drawString(0,4,"to score");
    delay(2000);
}
 
void lines(){
    //draw the central columns
    uint8_t Lline[16] = { 0xf0, 0x0f, 1, 0xf0, 1, 0x0f, 1, 0xf0,1,0x0f,1,1,1,1,0xf0,1};
    uint8_t Rline[16] = { 0xf0, 0x0f, 1, 0xf0, 1, 0x0f, 1, 0xf0,1,0x0f,1,1,1,1,0xf0,1};
    u8x8.drawTile(0, 7, 1, Lline);
    u8x8.drawTile(7, 7, 1, Rline); 
    u8x8.setCursor(0, 0);
    u8x8.print(score);
}

void scoreCheck(){
      if(digitalRead(BtnPin) == 0 && pos == 7){
      score = score + 1;
     }
     
  //bounce sound
    if (pos == 0 || pos == 15) {
      for(int f=0;f<100;f++){
            digitalWrite(BuzzerPin,HIGH);
            delay(1);
            digitalWrite(BuzzerPin,LOW);
            delay(1);
      }
    } 
}

void loop()
{
  lines();
  scoreCheck();
  
    for(pos=0;pos<=15;pos++){
            uint8_t square[16] = { 255, 255, 255,255, 255, 255, 255, 255};
            u8x8.drawTile(4, pos, 2, square);
            lines();
            delay(pace);
            scoreCheck();
            u8x8.clearDisplay();
            
            
        }
     for(pos=15;pos>=0;pos--){
            uint8_t square[16] = { 255, 255, 255,255, 255, 255, 255, 255};
            u8x8.drawTile(4, pos, 2, square);
            lines();
            delay(pace); 
            scoreCheck();
            u8x8.clearDisplay();
            
        }   
      //increase speed on each pass  
      pace = pace - 5;
}    

