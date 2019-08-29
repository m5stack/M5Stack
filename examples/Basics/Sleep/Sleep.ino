#include <M5Stack.h>

void setup() {
  M5.begin();

  /*
    Power chip connected to gpio21, gpio22, I2C device
    Set battery charging voltage and current
    If used battery, please call this function in your project
  */
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
  
  M5.Lcd.printf("Go lightSleep (5s or press buttonA wake up)\n");
  delay(2500);
  // Calling this function will disable the power button
  // To restore power btn please call M5.Power.setPowerBoostKeepOn(false)
  M5.Power.lightSleep(SLEEP_SEC(5));
  
  M5.Lcd.printf("Go lightSleep (press buttonA wake up)\n");
  delay(2500);
  // Calling this function will disable the power button
  // To restore power btn please call M5.Power.setPowerBoostKeepOn(false)
  M5.Power.lightSleep(0);
  
  M5.Lcd.printf("resume.\n\nGo deepSleep (press buttonA wake up) ");
  delay(2500);
  // Calling this function will disable the power button
  // To restore power btn please call M5.Power.setPowerBoostKeepOn(false)
  M5.Power.deepSleep(0);
  
}
