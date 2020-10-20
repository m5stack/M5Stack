/*
    Description: Display the distance measured by ultrasonic
*/

#include <M5Stack.h>

TFT_eSprite Terminalbuff = TFT_eSprite(&M5.Lcd);

void setup() {
  // put your setup code here, to run once:
    M5.begin();
    
    Wire.begin(21,22);
    
    Terminalbuff.createSprite(160, 80);
    Terminalbuff.fillRect(80,20,160,80,BLACK);
    
    Terminalbuff.pushSprite(80,20);
    Terminalbuff.setTextFont(4);
    M5.Lcd.setCursor(105, 0, 4);
    
    M5.Lcd.print("Ultrasonic");
}

float readEUS()
{
    uint32_t data;
    Wire.beginTransmission(0x57);
    Wire.write(0x01);
    Wire.endTransmission();
    delay(120);
    Wire.requestFrom(0x57,3);
    data  = Wire.read();data <<= 8;
    data |= Wire.read();data <<= 8;
    data |= Wire.read();
    return float(data) / 1000;
    
}

void loop() {
                                                                            
    float newvalue = 0;

    while(1)
    {
        newvalue = readEUS();

        Terminalbuff.fillRect(80,20,160,80,BLACK);
        Terminalbuff.setCursor(30,50);

        if(( newvalue < 1500 )&&( newvalue > 20 ))
        {
            Terminalbuff.printf("%.2fmm",newvalue);
            Terminalbuff.pushSprite(80,20);
        }
        delay(100);
    }

}
