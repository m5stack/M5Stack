#ifndef __PORTHUB_H__
#define __PORTHUB_H__

#include <Wire.h>


#define IIC_ADDR  0x61
#define HUB1_ADDR 0x40
#define HUB2_ADDR 0x50
#define HUB3_ADDR 0x60
#define HUB4_ADDR 0x70
#define HUB5_ADDR 0x80
#define HUB6_ADDR 0xA0



class PortHub {

  public:
    PortHub();
    void begin();

    uint16_t hub_a_read_value(uint8_t reg);
	
    uint8_t  hub_d_read_value_A(uint8_t reg);
    uint8_t  hub_d_read_value_B(uint8_t reg);
	
    void hub_d_wire_value_A(uint8_t reg,uint16_t level);
    void hub_d_wire_value_B(uint8_t reg,uint16_t level);

    void hub_a_wire_value_A(uint8_t reg,uint16_t duty);
    void hub_a_wire_value_B(uint8_t reg,uint16_t duty);

    void hub_wire_length(uint8_t reg,uint16_t length);
	
    void hub_wire_index_color(uint8_t reg,uint16_t num,uint8_t r,int8_t g,uint8_t b);
	
    void hub_wire_fill_color(uint8_t reg,uint16_t first, uint16_t count, uint8_t r,int8_t g,uint8_t b);
	
    void hub_wire_setBrightness(uint8_t reg,uint8_t brightness);
  public:

  private:
    
  private:

};


#endif
