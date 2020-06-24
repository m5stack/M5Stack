/*
    Description: Read the status of the four line-tracking sensors of TRACE Unit and output them through UART.
*/

#include <M5Stack.h>

// #define VALUE_SPLIT

uint8_t value;
int SensorArray[4] = {0};

void setup() {
    m5.begin();
    M5.Power.begin();

    Serial.begin(115200);
    Wire.begin();
    M5.Lcd.clear(BLACK);
    M5.Lcd.setTextColor(YELLOW);
    M5.Lcd.setTextSize(2);
    M5.Lcd.setCursor(80, 0);
    M5.Lcd.println("TRACE example");
    M5.Lcd.setTextColor(WHITE);
}

void loop(){
    SensorStatus();
    delay(100);
}

void SensorStatus(void){
    Wire.beginTransmission(0x5a);
    Wire.write(0x00);
    Wire.endTransmission();
    Wire.requestFrom(0x5a,1);
    while(Wire.available()){
        value = Wire.read();
    }
    Serial.print(" value = ");Serial.println(value, HEX);
#ifdef VALUE_SPLIT
    SensorArray[3] = (value&0x08)>>3;
    SensorArray[2] = (value&0x04)>>2;
    SensorArray[1] = (value&0x02)>>1;
    SensorArray[0] = (value&0x01)>>0;
    Serial.println("After splitting... ");
    Serial.print("SensorArray[0] = ");Serial.print(SensorArray[0]);
    Serial.print("  SensorArray[1] = ");Serial.print(SensorArray[1]);
    Serial.print("  SensorArray[2] = ");Serial.print(SensorArray[2]);
    Serial.print("  SensorArray[3] = ");Serial.print(SensorArray[3]);
    Serial.println();
#endif
}
