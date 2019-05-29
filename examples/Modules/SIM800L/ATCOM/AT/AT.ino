#include <M5Stack.h>
#define RX_PIN      16
#define TX_PIN      17
#define RESET_PIN   5   

void setup() {
  M5.begin();
  
  // Host serial communication
  Serial.begin(115200);   

  // SIM800L serial communication
  Serial2.begin(115200, SERIAL_8N1, RX_PIN, TX_PIN);   
  pinMode(RESET_PIN, OUTPUT);
}

void loop() {

    //AT instruction write
    if(Serial.available()){
    Serial2.write(Serial.read());
  }

  //AT instruction result
  if(Serial2.available()){    
    Serial.write(Serial2.read());
  }

}
