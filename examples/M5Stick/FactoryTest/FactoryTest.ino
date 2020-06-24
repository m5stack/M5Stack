/*
Stick test
    hardware: M5Stack Stick
  
  please install the U8g2 library first ...
*/

#include <Arduino.h>
#include <U8x8lib.h>
#include <SPI.h>
#include <Wire.h>

#define LedPin 19
#define IrPin 17
#define BuzzerPin 26
#define BtnPin 35

U8X8_SH1107_64X128_4W_HW_SPI u8x8(14, /* dc=*/ 27, /* reset=*/ 33);
bool mpu9250_exis = false;
void mpu9250_test() {
    uint8_t data = 0;
    Wire.beginTransmission(0x68);         
    Wire.write(0x75);                     
    Wire.endTransmission(true);
    Wire.requestFrom(0x68, 1);  
    data = Wire.read();                   

    Serial.print("mpu9250 addr: ");
    Serial.println(data, HEX);
    if(data == 0x71) {
        mpu9250_exis = true;
    }
}

void setup() {
    // put your setup code here, to run once:
    Wire.begin(21, 22, 100000);
    u8x8.begin();
    Serial.begin(115200);

    pinMode(LedPin, OUTPUT);
    pinMode(IrPin, OUTPUT);
    pinMode(BuzzerPin, OUTPUT);
    pinMode(BtnPin, INPUT_PULLUP);
    ledcSetup(1, 38000, 10);
    ledcAttachPin(IrPin, 1);
    digitalWrite(BuzzerPin, LOW);
    u8x8.fillDisplay();
    u8x8.setFont(u8x8_font_chroma48medium8_r);
    delay(1500);
    u8x8.clearDisplay();
    mpu9250_test();
}

void loop()
{
    digitalWrite(LedPin, 1 - digitalRead(LedPin));
    ledcWrite(1, ledcRead(1) ? 0 : 512);
    delay(200);
    if(digitalRead(BtnPin) == 0){
        u8x8.drawString(2,0,"Hello");
        u8x8.drawString(2,1,"World!");
        if(mpu9250_exis) {
            u8x8.drawString(2, 3, "Exis");
        } else{
            u8x8.drawString(3, 3, "No");
        }
        for(int i=0;i<100;i++){
            digitalWrite(BuzzerPin,HIGH);
            delay(1);
            digitalWrite(BuzzerPin,LOW);
            delay(1);
        }
    } else {
        u8x8.clearDisplay();
    }
    // delay(200);
}