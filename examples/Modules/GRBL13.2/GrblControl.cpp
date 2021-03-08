#include "GrblControl.h"
#include <Wire.h>

GRBL::GRBL(int addr){
    this->addr = addr;
}

void GRBL::Init()		
{
	Wire.begin();
}

void GRBL::Init(uint32_t x_step,uint32_t y_step,uint32_t z_step,uint32_t acc)		
{
	Wire.begin();
    if(x_step){
        char code[256];
        sprintf(code,"$0=%d",x_step);
        Gcode(code);
    }
    if(y_step){
        char code[256];
        sprintf(code,"$1=%d",y_step);
        Gcode(code);
    }
    if(z_step){
        char code[256];
        sprintf(code,"$2=%d",z_step);
        Gcode(code);
    }
    if(acc){
        char code[256];
        sprintf(code,"$8=%d",acc);
        Gcode(code);
    }
}

void GRBL::Gcode(char *c)
{
  Wire.beginTransmission(addr);
  while ((*c) != 0) {
    Wire.write(*c);
    c++;
  }
  Wire.write(0x0d);
  Wire.write(0x0a);
  Wire.endTransmission();
}

void GRBL::SendByte(byte b) {
  Wire.beginTransmission(addr);
  Wire.write(b);
  Wire.endTransmission();
}

void GRBL::SendBytes(uint8_t *data, size_t size) {
  Wire.beginTransmission(addr);
  Wire.write(data,size);
  Wire.endTransmission();
}

void GRBL::ReadClean(){
    while(1){
        uint8_t i = 0;
        char data[10];
        Wire.requestFrom(addr, 10);
        while (Wire.available() > 0) {
            data[i++] = Wire.read();
        }
        if(data[9] == 0xff) break;
    }
}

void GRBL::UnLock() {
  this->SendByte(0x18);
  delay(5);
  char bytes[] = "$X\r\n";
  this->SendBytes((uint8_t *)bytes, 4);
}

void GRBL::SetMotor(int x, int y, int z, int speed) {

    char code[256];
    memset(code,0,sizeof(char)*256);
    sprintf(code,"G1 X%dY%dZ%d F%d",x,y,z,speed);
    return this->Gcode(code);
}

void GRBL::SetMode(String mode){
    if(mode == "distance"){
        char bytes[] = "G91\n";
        this->SendBytes((uint8_t *)bytes, 4);
        this->mode = mode;
    }else if(mode == "absolute"){
        char bytes[] = "G90\n";
        this->SendBytes((uint8_t *)bytes, 4);
        this->mode = mode;
    }
}

void GRBL::WaitIdle(){
    this->ReadClean();
    while(1){
        this->SendByte('@');
        char state;
        Wire.requestFrom(addr, 1);
        if (Wire.available() > 0) {
            state = Wire.read();
        }
        if(state == 'I'){
            break;
        }
        delay(5);
    }
}

//read grbl return message

String GRBL::ReadLine() {
    String Data = ""; 
    while(1){
        uint8_t i = 0;
        char data[10];
        Wire.requestFrom(addr, 10);
        while (Wire.available() > 0) {
            data[i] = Wire.read();
            i++;
        }
        Data += data;
        if (data[9] == 0xff) {
            break;
        }
    }
    return Data;
}

String GRBL::ReadStatus() {
    this->ReadClean();
    this->SendByte('@');
    return this->ReadLine();
}

    // read grbl state
bool GRBL::ReadIdle() {
    return this->ReadStatus()[0] == 'I';
}

bool GRBL::InLock() {
    return this->ReadStatus()[0] == 'A';
}