#include "GoPlus2.h"


GoPlus2::GoPlus2(){

}

void GoPlus2::begin(){
  Wire.begin();
}

void GoPlus2::Servo_write_angle(uint8_t number, uint8_t angle) {
    Wire.beginTransmission(GOPLUS_ADDR);
    Wire.write(SERVO_ADDR | number);
    Wire.write(angle);
    Wire.endTransmission();
}

void GoPlus2::Servo_write_plusewidth(uint8_t number,  uint16_t width) {
    Wire.beginTransmission(GOPLUS_ADDR);
    Wire.write(PLUSE_ADDR | number);
    uint8_t buff_out[2];
    buff_out[0] = width >> 8;
    buff_out[1] = width & 0xff;
    Wire.write(buff_out[0]);
    Wire.write(buff_out[1]);
    Wire.endTransmission();
}

void GoPlus2::Motor_write_speed(uint8_t number, int motor_speed) {
    Wire.beginTransmission(GOPLUS_ADDR);
    Wire.write(MOTOR_ADDR | number);
    Wire.write(motor_speed);
    Wire.endTransmission();
}

uint16_t GoPlus2::hub1_d_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB_READ_DIGITAL | reg);
  Wire.endTransmission();
  
  uint8_t RegValue_L;

  Wire.requestFrom(GOPLUS_ADDR,1);
  while(Wire.available()){
    RegValue_L = Wire.read();
  }
  return RegValue_L;   
} 


uint16_t GoPlus2::hub1_a_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB_READ_ANALOG | reg);
  Wire.endTransmission();
  
  uint8_t RegValue_L,RegValue_H;

  Wire.requestFrom(GOPLUS_ADDR,2);
  while(Wire.available()){
    RegValue_H = Wire.read();
    RegValue_L = Wire.read();
  }
  
  return (RegValue_H<<8) | RegValue_L;   
} 

uint16_t GoPlus2::hub2_a_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB_READ_ANALOG | reg);
  Wire.endTransmission();
  
  uint8_t RegValue_L,RegValue_H;

  Wire.requestFrom(GOPLUS_ADDR,2);
  while(Wire.available()){
    RegValue_H = Wire.read();
    RegValue_L = Wire.read();
  }
  
  return (RegValue_H<<8) | RegValue_L;   
} 

uint16_t GoPlus2::hub2_d_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB_READ_DIGITAL | reg);
  Wire.endTransmission();
  
  uint8_t RegValue_L;

  Wire.requestFrom(GOPLUS_ADDR,1);
  while(Wire.available()){
    RegValue_L = Wire.read();
  }
  
  return RegValue_L;   
} 

uint16_t GoPlus2::hub3_a_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB_READ_ANALOG | reg);
  Wire.endTransmission();
  
  uint8_t RegValue_L,RegValue_H;

  Wire.requestFrom(GOPLUS_ADDR,2);
  while(Wire.available()){
    RegValue_H = Wire.read();
    RegValue_L = Wire.read();
  }
  
  return (RegValue_H<<8) | RegValue_L;   
} 



uint16_t GoPlus2::hub3_d_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB_READ_DIGITAL | reg);
  Wire.endTransmission();
  
  uint8_t RegValue_L;

  Wire.requestFrom(GOPLUS_ADDR,1);
  while(Wire.available()){ 
    RegValue_L = Wire.read();
  }
  
  return RegValue_L;   
} 


void GoPlus2::hub1_wire_value(uint8_t reg,uint8_t value){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB_WIRE | reg);
  Wire.write(value & 0xff);
  Wire.endTransmission(); 
} 

void GoPlus2::hub2_wire_value(uint8_t reg,uint8_t value){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB_WIRE | reg);
  Wire.write(value & 0xff);
  Wire.endTransmission(); 
} 

void GoPlus2::hub3_wire_value(uint8_t reg,uint8_t value){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB_WIRE | reg);
  Wire.write(value & 0xff);
  Wire.endTransmission(); 
} 


void GoPlus2::hub1_set_io(uint8_t reg, uint8_t value){
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB_IO_SET | reg);
  Wire.write(value & 0xff);
  Wire.endTransmission(); 
}

void GoPlus2::hub2_set_io(uint8_t reg, uint8_t value){
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB_IO_SET | reg);
  Wire.write(value & 0xff);
  Wire.endTransmission(); 
}

void GoPlus2::hub3_set_io(uint8_t reg, uint8_t value){
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB_IO_SET | reg);
  Wire.write(value & 0xff);
  Wire.endTransmission(); 
}



uint8_t GoPlus2::hub1_d_o_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB_READ_O_DIGITAL | reg);
  Wire.endTransmission();
  
  uint8_t RegValue;

  Wire.requestFrom(GOPLUS_ADDR,1);
  while(Wire.available()){ 
    RegValue = Wire.read();
  }
  
  return RegValue;   
} 


uint8_t GoPlus2::hub2_d_o_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB_READ_O_DIGITAL | reg);
  Wire.endTransmission();
  
  uint8_t RegValue;

  Wire.requestFrom(GOPLUS_ADDR,1);
  while(Wire.available()){ 
    RegValue = Wire.read();
  }
  
  return RegValue;   
} 

uint8_t GoPlus2::hub3_d_o_read_value(uint8_t reg){
  
  Wire.beginTransmission(GOPLUS_ADDR);
  Wire.write(HUB_READ_O_DIGITAL | reg);
  Wire.endTransmission();
  
  uint8_t RegValue;

  Wire.requestFrom(GOPLUS_ADDR,1);
  while(Wire.available()){ 
    RegValue = Wire.read();
  }
  
  return RegValue;   
} 
