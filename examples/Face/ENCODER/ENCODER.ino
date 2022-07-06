#include <M5Stack.h>

#define Faces_Encoder_I2C_ADDR 0X5E

int encoder_increment;  // positive: clockwise nagtive: anti-clockwise
int encoder_value = 0;
uint8_t direction;  // 0: clockwise 1: anti-clockwise
uint8_t last_button, cur_button;

void GetValue(void) {
    int temp_encoder_increment;

    Wire.requestFrom(Faces_Encoder_I2C_ADDR, 3);
    if (Wire.available()) {
        temp_encoder_increment = Wire.read();
        cur_button             = Wire.read();
    }
    if (temp_encoder_increment > 127) {  // anti-clockwise
        direction         = 1;
        encoder_increment = 256 - temp_encoder_increment;
    } else {
        direction         = 0;
        encoder_increment = temp_encoder_increment;
    }
}

void Led(int i, int r, int g, int b) {
    Wire.beginTransmission(Faces_Encoder_I2C_ADDR);
    Wire.write(i);
    Wire.write(r);
    Wire.write(g);
    Wire.write(b);
    Wire.endTransmission();
}

void setup() {
    M5.begin();
    M5.Power.begin();
    Wire.begin();
    dacWrite(25, 0);
    M5.Lcd.setTextFont(2);
    M5.Lcd.println("FACES ENCODER I2C Read Example");
    Serial.println("FACES ENCODER I2C Read Example");

    for (int i = 0; i < 12; i++) {
        Led(i, 0, 0xff, 0);
        delay(10);
    }
    for (int i = 0; i < 12; i++) {
        Led(i, 0, 0, 0);
        delay(10);
    }
}

void loop() {
    int i;
    M5.Lcd.setCursor(0, 40);
    M5.Lcd.print("Encoder Value: ");
    M5.Lcd.setCursor(0, 60);
    M5.Lcd.print("  Key State : ");
    M5.Lcd.setCursor(0, 80);
    M5.Lcd.print("  Key Value : ");
    GetValue();

    if (last_button != cur_button) {
        M5.Lcd.fillRect(100, 60, 100, 25, BLACK);
        M5.Lcd.fillRect(100, 80, 100, 25, BLACK);
        last_button = cur_button;
    }
    if (cur_button) {
        M5.Lcd.setCursor(100, 60);
        M5.Lcd.print("released");
        M5.Lcd.setCursor(100, 80);
        M5.Lcd.print("1");
        for (i = 0; i < 12; i++) {
            Led(i, 0, 0, 0);
        }
    } else {
        M5.Lcd.setCursor(100, 60);
        M5.Lcd.print("pressed");
        M5.Lcd.setCursor(100, 80);
        M5.Lcd.print("0");
        for (i = 0; i < 12; i++) {
            Led(i, 255, 255, 255);
        }
    }

    M5.Lcd.fillRect(100, 40, 50, 25, BLACK);
    if (direction) {
        encoder_value -= encoder_increment;
        M5.Lcd.setCursor(100, 40);
        M5.Lcd.print("-");
        M5.Lcd.setCursor(100, 40);
        M5.Lcd.print(encoder_value);
        Serial.print("encoder_value: ");
        Serial.print("-");
        Serial.print(encoder_value);
    } else {
        encoder_value += encoder_increment;
        M5.Lcd.setCursor(100, 40);
        M5.Lcd.print(encoder_value);
        Serial.print("encoder_value: ");
        Serial.print(encoder_value);
    }
    Serial.print("    button_state: ");
    Serial.println(cur_button);
}
