#include <M5Stack.h>

void setup() {
  M5.begin();
  M5.Lcd.setBrightness(200);
  M5.Lcd.begin();
  M5.Lcd.clear();
  Serial.begin(115200);

  M5.Power.setWakeupButton(BUTTON_A_PIN);

}

void loop() {

  M5.update();
  bool c=M5.Power.isResetbyPowerSW();
  bool d=M5.Power.isResetbyDeepsleep();
 
  M5.Lcd.printf("<<Sleep test>>\n");
  M5.Lcd.printf("power-on triggered at:%s%s\n\n",c?"POWER-SW":"",d?"DeepSleep-end":"");
  
  M5.Lcd.printf("Go lightSleep (5s or press buttonA)\n");
  delay(2500);
  M5.Power.lightSleep(5000000);
  
  M5.Lcd.printf("resume.\n\nGo deepSleep (5s or press buttonA) ");
  delay(2500);
  M5.Power.deepSleep(5000000);
  
}
