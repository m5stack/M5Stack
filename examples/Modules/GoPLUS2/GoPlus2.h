#ifndef __GOPLUS2_H__
#define __GOPLUS2_H__

#include <Wire.h>

#define GOPLUS_ADDR 0x38  // I2C ADDR

#define SERVO_ADDR 0x00  // SERVO angle register
#define PLUSE_ADDR 0x10  // SERVO pluse width register

#define SERVO_NUM0 0x00
#define SERVO_NUM1 0x01
#define SERVO_NUM2 0x02
#define SERVO_NUM3 0x03

#define SERVO_NUM0_PW 0x00
#define SERVO_NUM1_PW 0x02
#define SERVO_NUM2_PW 0x04
#define SERVO_NUM3_PW 0x06

#define MOTOR_ADDR 0x30  // MOTOR register
#define MOTOR_NUM0 0x00
#define MOTOR_NUM1 0x01

#define HUB1_R_ADDR 0x00
#define HUB2_R_ADDR 0x02
#define HUB3_R_ADDR 0x04

#define HUB1_W_ADDR 0x00
#define HUB2_W_ADDR 0x01
#define HUB3_W_ADDR 0x02

#define HUB_READ_ANALOG 0x40  // HUB ANALOG_INPUT register

#define HUB_READ_DIGITAL 0x50  // HUB DIGITAL_INPUT register

#define HUB_WIRE 0x60  // HUB DIGITAL_OUTPUT register

#define HUB_READ_O_DIGITAL \
    0x70  // HUB DIGITAL_INPUT2(the 'O' pin) register,pull_up

#define HUB1_R_O_ADDR 0x00
#define HUB2_R_O_ADDR 0x01
#define HUB3_R_O_ADDR 0x02

#define HUB_IO_SET \
    0x80  // HUB SET INPUT(1) OR OUTPUT(0) mode (OUTPUT by default)

class GoPlus2 {
   public:
    GoPlus2();
    void begin();

    void Servo_write_angle(uint8_t number, uint8_t angle);
    void Servo_write_plusewidth(uint8_t number, uint16_t width);
    void Motor_write_speed(uint8_t number, int motor_speed);
    uint16_t hub1_a_read_value(uint8_t reg);
    uint16_t hub1_d_read_value(uint8_t reg);
    uint16_t hub2_d_read_value(uint8_t reg);
    uint16_t hub3_d_read_value(uint8_t reg);
    uint16_t hub2_a_read_value(uint8_t reg);
    uint16_t hub3_a_read_value(uint8_t reg);
    void hub1_wire_value(uint8_t reg, uint8_t value);
    void hub2_wire_value(uint8_t reg, uint8_t value);
    void hub3_wire_value(uint8_t reg, uint8_t value);

    void hub1_set_io(uint8_t reg, uint8_t value);
    void hub2_set_io(uint8_t reg, uint8_t value);
    void hub3_set_io(uint8_t reg, uint8_t value);

    uint8_t hub1_d_o_read_value(uint8_t reg);
    uint8_t hub2_d_o_read_value(uint8_t reg);
    uint8_t hub3_d_o_read_value(uint8_t reg);
};

#endif
