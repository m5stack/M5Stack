#include <M5Stack.h>

void setup() {
  M5.begin();
  M5.Power.begin();
  M5.Lcd.setBrightness(200);

  M5.Power.setWakeupButton(BUTTON_A_PIN);
  if(!M5.Power.canControl()) {
    M5.Lcd.printf("IP5306 is not i2c version\n");
  }
}

void loop() {

  M5.update();
  bool c=M5.Power.isResetbyPowerSW();
  bool d=M5.Power.isResetbyDeepsleep();
 
  M5.Lcd.printf("<<Sleep test>>\n");
  M5.Lcd.printf("power-on triggered at:%s%s\n\n",c?"POWER-SW":"",d?"DeepSleep-end":"");
  
  M5.Lcd.printf("Go lightSleep (5s or press buttonA)\n");
  delay(2500);
  // call this fun, power btn will Invalid
  // Restore power btn please call M5.Power.setPowerBoostKeepOn(false)
  M5.Power.lightSleep(0);
  
  M5.Lcd.printf("resume.\n\nGo deepSleep (5s or press buttonA) ");
  delay(2500);
  // call this fun, power btn will Invalid
  M5.Power.deepSleep(0);
  
}
