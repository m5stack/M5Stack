#include <M5Stack.h>
#define RX_PIN      16
#define TX_PIN      17  


#define X_LOCAL 40
#define Y_LOCAL 40

#define X_OFF 160
#define Y_OFF 30

int i=0,s=0;

void header(const char *string, uint16_t color){
    M5.Lcd.fillScreen(color);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_MAGENTA, TFT_BLUE);
    M5.Lcd.fillRect(0, 0, 320, 30, TFT_BLUE);
    M5.Lcd.setTextDatum(TC_DATUM);
    M5.Lcd.drawString(string, 160, 3, 4);
}

void setup() {
  M5.begin();
  M5.Power.begin();

  header("RS485 A", TFT_BLACK);
  M5.Lcd.setTextFont(2);
  M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  Serial.begin(115200);   
  Serial2.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);   
}


void loop() {
  
  Serial2.write('a');
  s++;

//  if(Serial2.available()){
//    char c = Serial2.read();
//    if(c=='b'){
//      c = 0;
//      i++;
//    }
//  }
  
  M5.Lcd.setCursor(X_LOCAL, Y_LOCAL,2);
  M5.Lcd.printf("S :%d\n",s);
//  M5.Lcd.setCursor(X_LOCAL+X_OFF, Y_LOCAL,2);
//  M5.Lcd.printf("R :%d\n",i);
//  delay(10);
}
