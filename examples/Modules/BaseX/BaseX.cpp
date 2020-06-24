/*
 * @Author: Sorzn
 * @Date: 2019-12-12 14:33:45
 * @LastEditTime: 2019-12-13 15:58:32
 * @Description: M5Stack project
 * @FilePath: /M5Stack/examples/Modules/BaseX/BaseX.cpp
 */

#include "M5Stack.h"
#include "BaseX.h"

#define MAX(in, max) (((in) > (max)) ? (in) : (max))
#define MIN(in, min) (((in) < (min)) ? (in) : (min))
#define CONSTRAIN(in, min, max) MAX(min, MIN(in, max))

uint8_t BASE_X::CheckPos(uint8_t pos)
{
    pos = pos - 1;
    pos = CONSTRAIN(pos, 0, 3);
    return pos;
}

/**
 * @description: 
 * @param pos: 1 ~ 4
 * @param mode: NORMAL, POSITION LOCK, SPEED LOCK
 * @return: None
 */
void BASE_X::SetMode(uint8_t pos, uint8_t mode)
{    
    pos = CheckPos(pos);
    M5.I2C.writeByte(BASE_X_ADDR, BASE_X_CONFIG_ADDR + (0x10 * pos), mode);
}   

/**
 * @description: 
 * @param pos: 1 ~ 4
 * @return: encoder value
 */
int32_t BASE_X::GetEncoderValue(uint8_t pos)
{
    uint8_t addr;
    uint8_t read_buf[4] = {0, 0, 0, 0};
    
    pos = CheckPos(pos);
    addr = BASE_X_ENCODER_ADDR + 4*pos;
    M5.I2C.readBytes(BASE_X_ADDR, addr, 4, read_buf);
    return (read_buf[0] << 24) | (read_buf[1] << 16) | (read_buf[2] << 8) | read_buf[3];
}

/**
 * @description: 
 * @param pos: 1 ~ 4 
 * @param encoder: INT32_MIN ~ INT32_MAX 
 * @return: None 
 */
void BASE_X::SetEncoderValue(uint8_t pos, int32_t encoder)
{
    uint8_t addr;
    uint8_t write_buf[4] = {0, 0, 0, 0};

    pos = CheckPos(pos);
    addr = BASE_X_ENCODER_ADDR + 4*pos;
    write_buf[0] = encoder >> 24;
    write_buf[1] = encoder >> 16;
    write_buf[2] = encoder >> 8;
    write_buf[3] = encoder & 0xff;

    M5.I2C.writeBytes(BASE_X_ADDR, addr, write_buf, 4);
}

/**
 * @description: 
 * @param pos: 1 ~ 4  
 * @param duty: Set motor speed, -127 ~ 127
 * @return: 
 */
void BASE_X::SetMotorSpeed(uint8_t pos, int8_t duty)
{
    uint8_t addr;
    pos = CheckPos(pos);
    addr = BASE_X_PWM_DUTY_ADDR + pos;

    M5.I2C.writeByte(BASE_X_ADDR, addr, duty);
}

/**
 * @description: 
 * @param pos: 1 ~ 4
 * @return Motor run speed, -127 ~ 127: 
 */
int8_t BASE_X::GetMotorSpeed(uint8_t pos)
{
    uint8_t read_data;
    uint8_t addr;
    pos = CheckPos(pos);
    addr = BASE_X_PWM_DUTY_ADDR + pos;

    M5.I2C.readByte(BASE_X_ADDR, addr, &read_data);
    return read_data;
}


/**
 * @description: 
 * @param pos: 1 ~ 4
 * @return: Motor encoder increments every 20 ms
 */
int8_t BASE_X::GetMotorSpeed20MS(uint8_t pos)
{
    uint8_t read_data;
    uint8_t addr;
    pos = CheckPos(pos);
    addr = BASE_X_SPEED_ADDR + pos;

    M5.I2C.readByte(BASE_X_ADDR, addr, &read_data);
    return read_data;
}

void BASE_X::SetPositionPID(uint8_t pos, uint8_t kp, uint8_t ki, uint8_t kd)
{
    uint8_t write_buf[3] = {0, 0, 0};
    uint8_t addr;
    pos = CheckPos(pos);

    addr = BASE_X_CONFIG_ADDR + pos * 0x10 + 0x01;
    write_buf[0] = kp;
    write_buf[1] = ki;
    write_buf[2] = kd;

    M5.I2C.writeBytes(BASE_X_ADDR, addr, write_buf, 3);
}   

/**
 * @description: 
 * @param pos: 1 ~ 4 
 * @param position_point: in POSITION mode, motor will lock in this value, INT32_MIN ~ INT32_MAX
 * @return: None
 */
void BASE_X::SetPositionPoint(uint8_t pos, int32_t position_point)
{
    uint8_t addr;
    uint8_t write_buf[4] = {0, 0, 0, 0};

    pos = CheckPos(pos);
    addr = BASE_X_CONFIG_ADDR + pos * 0x10 + 0x04;
    write_buf[0] = position_point & 0xff;
    write_buf[1] = position_point >> 8;
    write_buf[2] = position_point >> 16;
    write_buf[3] = position_point >> 24;

    // Serial.printf(" %x %x %x %x \r\n", write_buf[0], write_buf[1], write_buf[2], write_buf[3]);
    M5.I2C.writeBytes(BASE_X_ADDR, addr, write_buf, 4);
}

/**
 * @description: 
 * @param  pos: 1 ~ 4  
 * @param  max_pwm: 0 ~ 127, POSITION mode, max speed  
 * @return: 
 */
void BASE_X::SetPostionPIDMaxSpeed(uint8_t pos, uint8_t max_pwm)
{
    uint8_t addr;
    pos = CheckPos(pos);
    addr = BASE_X_CONFIG_ADDR + pos * 0x10 + 0x08;

    M5.I2C.writeByte(BASE_X_ADDR, addr, max_pwm);
}

void BASE_X::SetSpeedPID(uint8_t pos, uint8_t kp, uint8_t ki, uint8_t kd)
{
    uint8_t write_buf[3] = {0, 0, 0};
    uint8_t addr;
    pos = CheckPos(pos);

    addr = BASE_X_CONFIG_ADDR + pos * 0x10 + 0x09;
    write_buf[0] = kp;
    write_buf[1] = ki;
    write_buf[2] = kd;

    M5.I2C.writeBytes(BASE_X_ADDR, addr, write_buf, 3);
}

/**
 * @description: 
 * @param pos: 1 ~ 4 
 * @param speed_point: speed_point is lock GetMotorSpeed20MS(), not GetMotorSpeed(), maybe -20 ~ 20
 * @return: None
 */
void BASE_X::SetSpeedPoint(uint8_t pos, int8_t speed_point)
{
    uint8_t addr;
    pos = CheckPos(pos);
    addr = BASE_X_CONFIG_ADDR + pos * 0x10 + 0x0c;

    M5.I2C.writeByte(BASE_X_ADDR, addr, (uint8_t)speed_point);
}

/**
 * @description: 
 * @param pos: 1 ~ 2
 * @param angle: 0 ~ 180
 * @return: None
 */
void BASE_X::SetServoAngle(uint8_t pos, uint8_t angle)
{
    uint8_t addr;
    pos = CheckPos(pos);
    addr = BASE_X_SERVO_ANGLE_ADDR + pos;

    M5.I2C.writeByte(BASE_X_ADDR, addr, angle);
}

/**
 * @description: 
 * @param pos: 1 ~ 2
 * @param width: 500 ~ 2500
 * @return: None
 */
void BASE_X::SetServoPulseWidth(uint8_t pos, uint16_t width)
{
    uint8_t addr;
    uint8_t write_buf[2] = {0, 0};

    pos = CheckPos(pos);
    addr = BASE_X_SERVO_PULSE_ADDR + pos * 0x02;
    write_buf[0] = width >> 8;
    write_buf[1] = width & 0xff;
    M5.I2C.writeBytes(BASE_X_ADDR, addr, write_buf, 2);
}