/*
  WiFi Web Server LED Blink

 A simple web server that lets you blink an LED via the web.
 This sketch will print the IP address of your WiFi Shield (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to turn on and off the LED on pin 5.

 If the IP address of your shield is yourAddress:
 http://yourAddress/H turns the LED on
 http://yourAddress/L turns it off

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * WiFi shield attached
 * LED attached to pin 5

 created for arduino 25 Nov 2012
 by Tom Igoe

ported for sparkfun esp32
31.01.2017 by Jan Hendrik Berlin

 */
#include "clock.h"

WiFiUDP ntpUDP;
// NTPClient timeClient(ntpUDP);
NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

TimerObject *timer1 = new TimerObject(1000);
TimerObject *timer_setting_select = new TimerObject(500);
TimerObject *timer_button_draw = new TimerObject(10);

typedef void (*CallBackType)();
#define X_POS_OFFSET    15
#define Y_POS_OFFSET    45

// M5Stack_Button button[3];
void upCursorBlink();

class AlarmClock {

enum week {SUN, MON, TUE, WED, THU, FRI, SAT, EVERYDAY};

public:
  AlarmClock() {
    state = 0;
  }

  AlarmClock(uint8_t h, uint8_t m) {
    hours = h;
    minutes = m;
    // seconds = s;
    repeat_week_mask = EVERYDAY;
    state = 1;
  }

  AlarmClock(uint8_t h, uint8_t m, uint8_t repeat_mask) {
    AlarmClock(h, m);
    repeat_week_mask = repeat_mask;
    state = 1;
  }

  void setAlarmTime(uint8_t h, uint8_t m) {
    hours = h;
    minutes = m;
    // seconds = s;
  }

  void setEnable() {
    state = 1;
  }

  void setDisable() {
    state = 0;
  }

  void setOnClock(CallBackType callback) {
    on_alarm_clock = callback;
  }

  void update(uint64_t epoch_time) {
    if(state == 1) {
      unsigned long rawTime = epoch_time;
      unsigned long local_hours = (rawTime % 86400L) / 3600;
      unsigned long local_minutes = (rawTime % 3600) / 60;
      // unsigned long local_seconds = rawTime % 60;

      if((local_minutes==minutes)&&(local_hours==hours)) {
        if(on_alarm_clock) {
          on_alarm_clock();
        }
      }
    }
  }

// private:
  bool state;
  int8_t hours;
  int8_t minutes;
  int8_t seconds;
  int8_t repeat_week_mask;
  CallBackType on_alarm_clock;
};

AlarmClock alarm_clock(6, 30);

// AlarmClock alarm_clock; //= new AlarmClock(7, 30, 00);
uint8_t alarm_state = 0;
uint8_t setting_state = 0;
#define ACLOCK_RUNING   0
#define ACLOCK_SETTING  1

void upTimeDisplay()
{
  m5.lcd.setTextSize(4);
  m5.lcd.setTextColor(BLACK, WHITE);
  m5.lcd.setCursor(X_POS_OFFSET, 45);
  m5.lcd.print(timeClient.getFormattedTime().c_str());
}

void upCursorBlink()
{
  static bool tg = 1;

  m5.lcd.setTextSize(4);
  m5.lcd.setCursor(X_POS_OFFSET+35, 95);

  if((setting_state == 1) && (tg)) {
    m5.lcd.setTextColor(WHITE, BLACK);
  } else {
    m5.lcd.setTextColor(BLACK, WHITE);
  }
  m5.lcd.printf("%2.2d", alarm_clock.hours);

  m5.lcd.setTextColor(BLACK, WHITE);
  m5.lcd.printf(":");

  if((setting_state == 2) && (tg)) {
    m5.lcd.setTextColor(WHITE, BLACK);
  } else {
    m5.lcd.setTextColor(BLACK, WHITE);
  }
  m5.lcd.printf("%2.2d", alarm_clock.minutes);
}

void clock_setup()
{

//---------------------------------------
  m5.lcd.setFont();
  m5.lcd.fillScreen(WHITE);
  m5.lcd.fillRect(0, 0, 220, 22, 0x09F1);
  m5.lcd.setCursor(4, 4);
  m5.lcd.setTextColor(WHITE);
  m5.lcd.setTextSize(2);
  m5.lcd.printf("Alarm Clock");

  #define X_OFFSET  40
  #define Y_POS			165

  m5.lcd.buttonSet(BTN_A, "+");
  m5.lcd.buttonSet(BTN_B, "-");
  m5.lcd.buttonSet(BTN_C, "+SET");

//---------------------------------------------

  m5.lcd.setTextSize(4);
  m5.lcd.setCursor(X_POS_OFFSET, 45);
  // m5.lcd.setTextColor(BLACK);
  m5.lcd.setTextColor(BLACK, WHITE);

  // m5.lcd.setTextWrap(boolean w);
  // m5.lcd.print("hello world!");
  // m5.lcd.print(timeClient.getFormattedTime().c_str());

  // alarm_state = ACLOCK_SETTING;
  // setting_state = 1;

  //--------timeClient------
  // wifi_connect();
  timeClient.begin();
  timeClient.setTimeOffset(28800);

  timer1->setOnTimer(&upTimeDisplay);
  timer1->Start();
  timer_setting_select->setOnTimer(upCursorBlink);
  upTimeDisplay();
  upCursorBlink();
}


void clock_loop(){
  switch (alarm_state) {
    case ACLOCK_RUNING:
      if(m5.pressed(BTN_C)) {
        setting_state = 1;
        alarm_state = ACLOCK_SETTING;
        timer_setting_select->Start();
      }
      break;

    case ACLOCK_SETTING:
      if(m5.pressed(BTN_C)) {
        if(++setting_state == 3) {
          setting_state = 0;
          alarm_state = ACLOCK_RUNING;
          // timer_setting_select->Stop();
        }
      }
      switch (setting_state) {
        case 1:
          if(m5.pressed(BTN_A)) {if(++alarm_clock.hours == 24) alarm_clock.hours = 0;}
          if(m5.pressed(BTN_B)) {if(--alarm_clock.hours == -1) alarm_clock.hours = 23;}
          break;

        case 2:
          if(m5.pressed(BTN_A)) {if(++alarm_clock.minutes == 60) alarm_clock.minutes = 0;}
          if(m5.pressed(BTN_B)) {if(--alarm_clock.minutes == -1) alarm_clock.minutes = 59;}
          break;
      }
      break;
  }

  timer1->Update();
  timer_setting_select->Update();
  // timer_button_draw->Update();
  timeClient.update();
}
