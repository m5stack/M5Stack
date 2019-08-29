/*
  This Unit connects to GRVOE B on M5Core.
*/
#include <M5Stack.h>

int last_value_red = 0;
int cur_value_red = 0;
int last_value_blue = 0;
int cur_value_blue = 0;

void setup() {
  // init lcd
  M5.begin();
  M5.Power.begin();
  pinMode(26, INPUT);
  pinMode(36, INPUT);
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(40, 0);
  M5.Lcd.println("Dual Button example");
  M5.Lcd.setTextColor(WHITE);
}

void loop() {
  cur_value_red = digitalRead(26);
  cur_value_blue = digitalRead(36);

  M5.Lcd.setCursor(0,25); M5.Lcd.print("Blue Status: ");
  M5.Lcd.setCursor(0,45); M5.Lcd.print("Blue Value: ");
  M5.Lcd.setCursor(0,65); M5.Lcd.print(" Red Status: ");
  M5.Lcd.setCursor(0,85); M5.Lcd.print(" Red Value: ");

  if(cur_value_blue != last_value_blue){
    M5.Lcd.fillRect(160,25,100,25,BLACK);
    M5.Lcd.fillRect(160,45,100,25,BLACK);
    if(cur_value_blue==0){
     M5.Lcd.setCursor(160,25); M5.Lcd.print("pressed");
     M5.Lcd.setCursor(160,45); M5.Lcd.print("0");
     Serial.println("Button Status: blue pressed");
     Serial.println(" value: 0");
    }
    else{
      M5.Lcd.setCursor(160,25); M5.Lcd.print("released");
      M5.Lcd.setCursor(160,45); M5.Lcd.print("1");
      Serial.println("Button Status: blue released");
      Serial.println(" value: 1");
    }
      last_value_blue = cur_value_blue;
  }

    if(cur_value_red != last_value_red){
      M5.Lcd.fillRect(160,65,100,25,BLACK);
      M5.Lcd.fillRect(160,85,100,25,BLACK);
      if(cur_value_red==0){
        M5.Lcd.setCursor(160,65); M5.Lcd.print("pressed");
        M5.Lcd.setCursor(160,85); M5.Lcd.print("0");
        Serial.println("Button Status: red pressed");
        Serial.println(" value: 0");
      }
      else{
        M5.Lcd.setCursor(160,65); M5.Lcd.print("released");
        M5.Lcd.setCursor(160,85); M5.Lcd.print("1");
        Serial.println("Button Status: red released");
        Serial.println(" value: 1");
      }
      last_value_red = cur_value_red;
  }

  M5.update();
}
