#include <M5Stack.h>

int last_value = 0;
int cur_value = 0;

void setup() {
  // init lcd
  M5.begin();
  M5.Power.begin();
  Serial.begin(115200);
  pinMode(36, INPUT);
  M5.Lcd.clear(BLACK);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(80, 0); M5.Lcd.println("Button example");
  Serial.println("Button example: ");
  M5.Lcd.setTextColor(WHITE);
}

void loop() {
  cur_value = digitalRead(36);// read the value of BUTTON

  M5.Lcd.setCursor(0,25); M5.Lcd.print("Status: ");
  M5.Lcd.setCursor(0,45); M5.Lcd.print("Value: ");
  if(cur_value != last_value){
    M5.Lcd.fillRect(95,25,100,25,BLACK);
    M5.Lcd.fillRect(95,45,100,25,BLACK);
    if(cur_value==0){
      M5.Lcd.setCursor(95,25); M5.Lcd.print("pressed");// display the status
      M5.Lcd.setCursor(95,45); M5.Lcd.print("0");
      Serial.println("Button Status: pressed");
      Serial.println("       value:  0");
    }
    else{
      M5.Lcd.setCursor(95,25); M5.Lcd.print("released");// display the status
      M5.Lcd.setCursor(95,45); M5.Lcd.print("1");
      Serial.println("Button Status: released");
      Serial.println("       value:  1");
    }
    last_value = cur_value;
  }
  M5.update();
}
