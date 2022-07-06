#ifndef __PORTHUB_H__
#define __PORTHUB_H__

#include <Wire.h>

#define IIC_ADDR1 0x61
#define IIC_ADDR2 0x62
#define IIC_ADDR3 0x63
#define IIC_ADDR4 0x64
#define IIC_ADDR5 0x65
#define IIC_ADDR6 0x66
#define IIC_ADDR7 0x67
#define IIC_ADDR8 0x68
#define HUB1_ADDR 0x40
#define HUB2_ADDR 0x50
#define HUB3_ADDR 0x60
#define HUB4_ADDR 0x70
#define HUB5_ADDR 0x80
#define HUB6_ADDR 0xA0

class PortHub {
   public:
    PortHub();
    PortHub(uint8_t iic_addr);
    void begin();

    uint16_t hub_a_read_value(uint8_t reg);

    uint8_t hub_d_read_value_A(uint8_t reg);
    uint8_t hub_d_read_value_B(uint8_t reg);

    void hub_d_wire_value_A(uint8_t reg, uint16_t level);
    void hub_d_wire_value_B(uint8_t reg, uint16_t level);

    void hub_a_wire_value_A(uint8_t reg, uint16_t duty);
    void hub_a_wire_value_B(uint8_t reg, uint16_t duty);

    void hub_wire_length(uint8_t reg, uint16_t length);

    void hub_wire_index_color(uint8_t reg, uint16_t num, uint8_t r, int8_t g,
                              uint8_t b);

    void hub_wire_fill_color(uint8_t reg, uint16_t first, uint16_t count,
                             uint8_t r, int8_t g, uint8_t b);

    void hub_wire_setBrightness(uint8_t reg, uint8_t brightness);

   public:
   private:
    uint8_t _iic_addr = IIC_ADDR1;

   private:
};

#endif
