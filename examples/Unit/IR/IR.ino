/*
    Description: Use IR Unit to receive and test infrared receiving and transmitting
*/
#include <M5Stack.h>
// select the input pin for the potentiometer
int ir_recv_pin = 36;
int ir_send_pin = 26;
int last_recv_value = 0;
int cur_recv_value = 0;

void setup() {
  M5.begin();
  M5.Power.begin();
  Serial.begin(115200);
  pinMode(ir_recv_pin, INPUT);
  pinMode(ir_send_pin, OUTPUT);
  //send infrared light
  //now, you can see the infrared light through mobile phone camera
  digitalWrite(ir_send_pin, 1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(0, 0);
  M5.Lcd.print("Test for IR receiver: ");
}

void loop() {
  //now, once you press the button on a remote controller to send infrared light
  //the screen will display "detected!"
  cur_recv_value = digitalRead(ir_recv_pin);
  if(last_recv_value != cur_recv_value){
    M5.Lcd.setCursor(0, 25);
    M5.Lcd.fillRect(0, 25, 150, 25, BLACK);
    if(cur_recv_value == 0){//0: detected 1: not detected
      M5.Lcd.print("detected!");
    }
    last_recv_value = cur_recv_value;
  }
  Serial.println(cur_recv_value);
//  delay(100);
}
