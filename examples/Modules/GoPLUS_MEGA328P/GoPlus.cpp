#include "GoPlus.h"


GoPlus::GoPlus(){

}

void GoPlus::begin(){
  Wire.begin();
}

void GoPlus::Servo_write_angle(uint8_t number, uint8_t angle) {
    Wire.beginTransmission(GOPLUS_ADDR);
    Wire.write(SERVO_ADDR | number);
    Wire.write(angle);
    Wire.endTransmission();
}

void GoPlus::Motor_write_speed(uint8_t number, uint8_t trun,uint8_t motor_speed) {
    Wire.beginTransmission(GOPLUS_ADDR);
    Wire.write(MOTOR_ADDR | number);
    Wire.write(trun);
    Wire.write(motor_speed);
    Wire.endTransmission();
}

uint16_t GoPlus::hub1_a_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB1_ADDR | reg);
  Wire.endTransmission();
  
  uint8_t RegValue_L,RegValue_H;

  Wire.requestFrom(GOPLUS_ADDR,1);
  while(Wire.available()){
    RegValue_L = Wire.read();
  }
  
  return RegValue_L;   
} 


uint16_t GoPlus::hub1_d_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB1_ADDR | reg);
  Wire.endTransmission();
  
  uint8_t RegValue_L,RegValue_H;

  Wire.requestFrom(GOPLUS_ADDR,2);
  while(Wire.available()){
    RegValue_L = Wire.read();
    RegValue_H = Wire.read();
  }
  
  return (RegValue_H<<8) | RegValue_L;   
} 

uint16_t GoPlus::hub2_d_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB2_ADDR | reg);
  Wire.endTransmission();
  
  uint8_t RegValue_L,RegValue_H;

  Wire.requestFrom(GOPLUS_ADDR,2);
  while(Wire.available()){
    RegValue_L = Wire.read();
    RegValue_H = Wire.read();
  }
  
  return (RegValue_H<<8) | RegValue_L;   
} 

uint16_t GoPlus::hub2_a_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB2_ADDR | reg);
  Wire.endTransmission();
  
  uint8_t RegValue_L,RegValue_H;

  Wire.requestFrom(GOPLUS_ADDR,1);
  while(Wire.available()){
    RegValue_L = Wire.read();
  }
  
  return RegValue_L;   
} 

uint16_t GoPlus::hub3_d_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB3_ADDR | reg);
  Wire.endTransmission();
  
  uint8_t RegValue_L,RegValue_H;

  Wire.requestFrom(GOPLUS_ADDR,2);
  while(Wire.available()){
    RegValue_L = Wire.read();
    RegValue_H = Wire.read();
  }
  
  return (RegValue_H<<8) | RegValue_L;   
} 



uint16_t GoPlus::hub3_a_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB3_ADDR | reg);
  Wire.endTransmission();
  
  uint8_t RegValue_L,RegValue_H;

  Wire.requestFrom(GOPLUS_ADDR,1);
  while(Wire.available()){
    RegValue_L = Wire.read();
  }
  
  return RegValue_L;   
} 


void GoPlus::hub1_wire_value(uint8_t reg,uint16_t value){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB1_ADDR | reg);
  Wire.write(value & 0xff);
  Wire.endTransmission(); 
} 

void GoPlus::hub2_wire_value(uint8_t reg,uint16_t value){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB2_ADDR | reg);
  Wire.write(value & 0xff);
  Wire.endTransmission(); 
} 

void GoPlus::hub3_wire_value(uint8_t reg,uint16_t value){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB3_ADDR | reg);
  Wire.write(value & 0xff);
  Wire.endTransmission(); 
} 
