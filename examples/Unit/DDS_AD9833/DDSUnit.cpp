#include "DDSUnit.h"

void DDSUnit::writeDDSReg(uint8_t addr, uint8_t data)
{
    //Serial.printf("ADDR:%02X,DATA:%02X\r\n",addr,data);
    _pwire->beginTransmission(DDS_UNIT_I2CADDR);
    _pwire->write(addr);
    _pwire->write(data);
    _pwire->endTransmission();
}

void DDSUnit::writeDDSReg(uint8_t addr, uint8_t* data, size_t size )
{
    _pwire->beginTransmission(DDS_UNIT_I2CADDR);
    _pwire->write(addr);
    for (size_t i = 0; i < size; i++)
    {
        _pwire->write(data[i]);
    }
    _pwire->endTransmission();
}

uint8_t DDSUnit::readDDSReg(uint8_t addr)
{
    uint8_t data;
    _pwire->beginTransmission(DDS_UNIT_I2CADDR);
    _pwire->write(addr);
    _pwire->endTransmission();
    _pwire->requestFrom(DDS_UNIT_I2CADDR, 1);
    data = _pwire->read();
    _pwire->endTransmission();

    return data;
}

void DDSUnit::readDDSRegs(uint8_t addr,uint8_t* dataptr,uint8_t size)
{
    _pwire->beginTransmission(DDS_UNIT_I2CADDR);
    _pwire->write(addr);
    _pwire->endTransmission();
    _pwire->requestFrom(DDS_UNIT_I2CADDR, (int)size);
    for (int i = 0; i < 6; i++)
    {
        dataptr[i] = _pwire->read();
    }
    _pwire->endTransmission();
}

int DDSUnit::begin( TwoWire *p )
{
    _pwire = p;

    char snStr[7];
    memset(snStr,0,sizeof(char)*7);
    readDDSRegs(DDS_DESC_ADDR,(uint8_t*)snStr,6);

    String desc(snStr);

    Serial.printf("sn:%s\r\n",snStr);

    return ( desc == String("ad9833")) ? 0 : -1;
}

void DDSUnit::setFreq( uint8_t reg ,uint64_t freq )
{
    freq = freq * 268435456 / DDS_FMCLK;

    uint8_t sendbuff[4] = {0,0,0,0};
    sendbuff[0] |= ((( freq >> 24 ) & 0xff ) | ( reg == 1 ) ? 0xC0 : 0x80 );
    sendbuff[1] |= (( freq >> 16 ) & 0xff );
    sendbuff[2] |= (( freq >> 8  ) & 0xff );
    sendbuff[3] |= ( freq & 0xff );
    writeDDSReg(DDS_FREQ_ADDR,sendbuff,4);
}

void DDSUnit::setPhase( uint8_t reg ,uint32_t phase )
{
    uint8_t sendbuff[2] = {0,0};
    phase = phase * 2048 / 360;
    sendbuff[0] |= ((( phase >> 8 ) & 0xff ) | (( reg == 1 ) ? 0xC0 : 0x80 ));
    sendbuff[1] |= ( phase & 0xff );

    //Serial.printf("%02X %02X",sendbuff[0],sendbuff[1]);
    writeDDSReg(DDS_PHASE_ADDR,sendbuff,2);
}

void DDSUnit::setFreqAndPhase( uint8_t freg, uint64_t freq, uint8_t preg, uint32_t phase )
{
    uint8_t sendbuff[6] = {0,0,0,0,0,0};

    freq = freq * 268435456 / DDS_FMCLK;
    sendbuff[0] |= ((( freq >> 24 ) & 0xff ) | ( freg == 1 ) ? 0xC0 : 0x80 );
    sendbuff[1] |= (( freq >> 16 ) & 0xff );
    sendbuff[2] |= (( freq >> 8  ) & 0xff );
    sendbuff[3] |= ( freq & 0xff );

    phase = phase * 2048 / 360;
    sendbuff[4] |= ((( phase >> 8 ) & 0xff ) | (( preg == 1 ) ? 0xC0 : 0x80 ));
    sendbuff[5] |= ( phase & 0xff );

    writeDDSReg(DDS_FREQ_ADDR,sendbuff,6);
}

void DDSUnit::setMode( DDSmode mode )
{
    writeDDSReg(DDS_MODE_ADDE,0x80 | mode );
}

void DDSUnit::setCTRL(uint8_t ctrlbyte)
{
    writeDDSReg(DDS_CTRL_ADDR,0x80 | ctrlbyte );
}

void DDSUnit::selectFreqReg( uint8_t num )
{
    uint8_t reg = readDDSReg(DDS_CTRL_ADDR);
    reg &= (~0x40);
    writeDDSReg(DDS_CTRL_ADDR,reg | 0x80 | ( num == 1 ) ? 0x40 : 0 );
}

void DDSUnit::selectPhaseReg( uint8_t num )
{
    uint8_t reg = readDDSReg(DDS_CTRL_ADDR);
    reg &= (~0x20);
    writeDDSReg(DDS_CTRL_ADDR, reg | 0x80 | ( num == 1 ) ? 0x20 : 0 );
}

void DDSUnit::quickOUT(DDSmode mode, uint64_t freq, uint32_t phase)
{
    if( mode <= kSQUAREMode )
    {
        setFreqAndPhase(0,freq,0,phase);
    }
    writeDDSReg( 0x20,0x80 | mode );
    writeDDSReg( 0x21,0x80 );
}

void DDSUnit::OUT( uint8_t freqnum,uint8_t phasenum )
{
    uint8_t reg = readDDSReg(DDS_CTRL_ADDR);
    reg &= (~0x60);
    writeDDSReg(DDS_CTRL_ADDR, reg | 0x80 | (( freqnum == 1 ) ? 0x40 : 0 ) | (( phasenum == 1 ) ? 0x20 : 0 ));
}

void DDSUnit::setSleep(uint8_t level)
{
    uint8_t reg = readDDSReg(DDS_CTRL_ADDR);
    reg &= (~0x18);
    reg |= ( level == 1 ) ? 0x10 : 0;
    reg |= ( level == 2 ) ? 0x08 : 0;
    writeDDSReg( DDS_CTRL_ADDR, 0x80 | reg );
}

void DDSUnit::reset()
{
    writeDDSReg(DDS_CTRL_ADDR,0x80 | 0x04 );
}