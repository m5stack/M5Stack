#include "bootmenu.h"

void drawTitle(char *str, int color)
{
  m5.lcd.setFont(&FreeSansOblique9pt7b);
  m5.lcd.fillRect(0, 0, 230, 22, color);
  m5.lcd.setCursor(4, 16);
  m5.lcd.setTextColor(WHITE);
  m5.lcd.setTextSize(1);
  m5.lcd.printf(str);
}

int selectMenu()
{
  static bool frist_setup=1;
  static int select_pt=0;
  uint8_t *ico_pt[20];
  #define NUMBER_OF_ICO   3

  ico_pt[0] = (uint8_t*)gImage_wifi_scaner;
  ico_pt[1] = (uint8_t*)gImage_temperature;
  ico_pt[2] = (uint8_t*)gImage_clock;

  const char* app_str[] = {"WiFi Scan", "Environment", "Clock", "Controller"};

  #define X_POS_MIDSET  75
  #define X_POS_START   3
  #define X_POS_OFFSET  72
  int x_pos;

  while(1) {
    if(m5.pressed(BUTTON_A)) {
      if(select_pt > 0) {
        --select_pt;
        Serial.printf("%d\r\n", select_pt);
        x_pos = 5;
        m5.lcd.fillRect(0, 30, 220, 30, WHITE);
        for(int i=0; i<8; i++) {
          // APP 0
          if(select_pt-1 >= 0) {
            m5.lcd.drawPicture  (x_pos-X_POS_OFFSET, 65, 64, 64, ico_pt[select_pt-1]);
          }
          //APP 1
          if(select_pt >= 0) {
            m5.lcd.fillRect(x_pos-10, 65, 11, 64, WHITE);
            m5.lcd.drawPicture  (x_pos, 65, 64, 64, ico_pt[select_pt]);
            m5.lcd.fillRect(x_pos+X_POS_OFFSET-10, 65, 11, 64, WHITE);
          }
          //APP 2
          m5.lcd.drawPicture  (x_pos+X_POS_OFFSET,    65, 64, 64, ico_pt[select_pt+1]);
          m5.lcd.fillRect(x_pos+X_POS_OFFSET*2-10, 65, 11, 64, WHITE);
          //APP 3
          if(select_pt+2 < 3) {
            m5.lcd.drawPicture  (x_pos+X_POS_OFFSET*2, 65, 64, 64, ico_pt[select_pt+2]);
          }

          if(i==7)
          {
            m5.lcd.drawRoundRect(X_POS_MIDSET-3, 65, 64, 64, 11, GRAY);
            m5.lcd.drawRoundRect(X_POS_MIDSET-2, 66, 62, 62, 10, GRAY);

            m5.lcd.setFont(&FreeSansBoldOblique9pt7b);
            m5.lcd.setTextColor(GRAY);
            m5.lcd.setTextSize(1);
            m5.lcd.setCursor(92 - (strlen(app_str[select_pt]) * 3 * 1), 52);
            // m5.lcd.setCursor(70, 50);
            m5.lcd.printf(app_str[select_pt]);
          }
          x_pos = i*12;
        }
      }
    }

    if(m5.pressed(BUTTON_C) || frist_setup) {
      frist_setup = 0;
      if(select_pt < 2) {
        ++select_pt;
        Serial.printf("%d\r\n", select_pt);
        x_pos = 5;
        m5.lcd.fillRect(0, 30, 220, 30, WHITE);
        for(int i=0; i<8; i++) {
          // m5.lcd.fillRect(62, 65, 10, 64, BLACK);
          m5.lcd.fillRect(62+X_POS_OFFSET, 65, 10, 64, WHITE);
          if(select_pt-2 >= 0) {
            m5.lcd.drawPicture  (x_pos, 65, 64, 64, ico_pt[select_pt-2]);
          }
          m5.lcd.fillRect(x_pos+X_POS_OFFSET-12, 65, 11, 64, WHITE);
          if(select_pt-1 >= 0) {
            m5.lcd.drawPicture  (x_pos+X_POS_OFFSET,    65, 64, 64, ico_pt[select_pt-1]);
          }
          m5.lcd.fillRect(x_pos+X_POS_OFFSET*2-10, 65, 11, 64, WHITE);
          // if(select_pt+2 < 9)
          m5.lcd.drawPicture  (x_pos+X_POS_OFFSET*2, 65, 64, 64, ico_pt[select_pt]);
          m5.lcd.fillRect(x_pos+X_POS_OFFSET*2+60, 65, 20, 64, WHITE);
          if(select_pt+1 < 3) {
            m5.lcd.fillRect(x_pos+X_POS_OFFSET*3+60, 65, 20, 64, WHITE);
            m5.lcd.drawPicture  (x_pos+X_POS_OFFSET*3, 65, 64, 64, ico_pt[select_pt+1]);
          } 

          if(i==7)
          {
            m5.lcd.drawRoundRect(X_POS_MIDSET-3, 65, 64, 64, 11, GRAY);
            m5.lcd.drawRoundRect(X_POS_MIDSET-2, 66, 62, 62, 10, GRAY);

            m5.lcd.setFont(&FreeSansBoldOblique9pt7b);
            m5.lcd.setTextColor(GRAY);
            m5.lcd.setTextSize(1);
            m5.lcd.setCursor(92 - (strlen(app_str[select_pt]) * 3 * 1), 52);
            // m5.lcd.setCursor(70, 50);
            m5.lcd.printf(app_str[select_pt]);
          }
          x_pos = -i*12;
        }
        // if(++select_pt>18) select_pt = 18;
      }
    }

    if(m5.pressed(BUTTON_B)) {
      return (select_pt);
    }

    m5.loop();
  }
}