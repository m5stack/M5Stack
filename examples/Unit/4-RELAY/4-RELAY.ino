/*
    Description: Control 4 relays and demonstrate the asynchronous control relay LED
    lib: https://github.com/m5stack/UNIT_4RELAY
*/

#include <M5Stack.h>
#include "UNIT_4RELAY.h"

/*-----------------------------------------------------------------------------*/
// |RELAY control reg          | 0x10
// |-----------------------------------------------------------------------------
// |Relay_ctrl_mode_reg[0]     | R/W | System control
//                                   | 7 | 6 | 5 | 4 | 3 | 2 | 1 |     0     |
//                                   | R | R | R | R | R | R | R | Sync Mode |
//                                   | -Sync Mode:0 LED&Relay Async
//                                   | -Sync Mode:1 LED&Relay Sync
//---------------------------------------------------------------------------------
// |Relay_ctrl_mode_reg[1]     | R/W | Relay & LED control
//                             |  7  |  6  |  5  |  4  |  3  |  2  |  1  |  0  |
//                             | LED1| LED2| LED3| LED4| RLY1| RLY2| RLY3| RLY4|
//
/*-------------------------------------------------------------------------------*/

UNIT_4RELAY unit_4relay;

void setup() {
  // put your setup code here, to run once:
  M5.begin(true, true, true, true);
  M5.Lcd.setCursor(90, 0, 4);
  M5.Lcd.setTextColor(TFT_GREEN, TFT_BLACK);
  M5.Lcd.print("4-RELAY UNIT");
  M5.Lcd.setCursor(0, 220, 2);
  M5.Lcd.print("Independent Switch");
  M5.Lcd.setCursor(130, 220, 2);
  M5.Lcd.print("LED Sync/Async");
  M5.Lcd.setCursor(250, 220, 2);
  M5.Lcd.print("ALL Relay");
  M5.Lcd.setCursor(20, 50, 4);
  M5.Lcd.print("Relay State: ");
  M5.Lcd.setCursor(20, 80, 4);
  M5.Lcd.print("Sync Mode: ");

  /*
   * MODE:
   * Async == 0;
   * Sync  == 1;
  */
  unit_4relay.Init(0);
  
}

uint8_t count_i = 0;
bool state = 0;
bool flag_mode = 0, flag_all= false;

void loop() {
    
    if(M5.BtnA.wasPressed()){
      M5.Lcd.fillRect(160, 50, 100, 20, TFT_BLACK);
      M5.Lcd.setCursor(160, 50, 4);
      if((count_i<4)&&(flag_mode == 1))
      {
        M5.Lcd.printf("%d ON", count_i);
        unit_4relay.relayWrite(count_i,1);
      }
      else if((count_i>=4)&&(flag_mode == 1))
      {
        M5.Lcd.printf("%d OFF", (count_i-4));
        unit_4relay.relayWrite((count_i-4),0);
      }
      else if((count_i<4)&&(flag_mode == 0))
      {
        M5.Lcd.printf("%d ON", count_i);
        unit_4relay.LEDWrite(count_i,1);
      }
      else if((count_i>=4)&&(flag_mode == 0))
      {
        M5.Lcd.printf("%d OFF", (count_i-4));
        unit_4relay.LEDWrite((count_i-4),0);
      }
      count_i++;    
      if( count_i >= 8 ) count_i = 0; 
    }
    if(M5.BtnB.wasPressed()){
      flag_mode = !flag_mode;
      M5.Lcd.fillRect(160, 80, 100, 20, TFT_BLACK);
      if(!flag_mode){
        M5.Lcd.setCursor(160, 80, 4);
        M5.Lcd.print("Async");
      }else {
        M5.Lcd.setCursor(160, 80, 4);
        M5.Lcd.print("Sync");
      }
      unit_4relay.switchMode(flag_mode);     
    }
    if(M5.BtnC.wasPressed()){
      M5.Lcd.fillRect(160, 50, 100, 20, TFT_BLACK);
      M5.Lcd.setCursor(160, 50, 4);
      if(flag_mode == 1){
        if(flag_all){
          M5.Lcd.printf("ALL.ON ");
          unit_4relay.relayALL(1);
          }
        else{
          M5.Lcd.printf("ALL.OFF");
          unit_4relay.relayALL(0);
          }
      }
      else{
        if(flag_all){
          M5.Lcd.printf("ALL.ON ");
          unit_4relay.LED_ALL(1);
          }
        else{
          M5.Lcd.printf("ALL.OFF");
          unit_4relay.LED_ALL(0);
          }
      }
        flag_all = !flag_all;
    }
    M5.update();
}