#include "PCA9554.h"

PCA9554::PCA9554(byte SlaveAddress)
{
  _SlaveAddress = SlaveAddress;
}


bool PCA9554::twiRead(byte &registerAddress)
{
  Wire.beginTransmission(_SlaveAddress);
  Wire.write(registerAddress);
  if (Wire.endTransmission() == 0)
  {
    delay(15);
    Wire.requestFrom((uint8_t)_SlaveAddress, (uint8_t)1);
    while (Wire.available() < 1);
    registerAddress = Wire.read();
    return true;
  }
  return false;
}

bool PCA9554::twiWrite(byte registerAddress, byte dataWrite)
{
  Wire.beginTransmission(_SlaveAddress);
  Wire.write(registerAddress);
  Wire.write(dataWrite);
  if (Wire.endTransmission() == 0)
    return true;
  return false;
}

bool PCA9554::pinMode0(byte pinNumber, bool state)
{
  byte oldValue = CONFIGPORT0;
  if (this->twiRead(oldValue) && (pinNumber <= 7))
  {
    if (!state)
    {
      oldValue |= (1 << pinNumber);
      if (this->portMode0(oldValue))
        return true;
      return false;
    }
    else if (state)
    {
      oldValue &= ~(1 << pinNumber);
      if (this->portMode0(oldValue))
        return true;
      return false;
    }
  }
  return false;
}

/* bool PCA9554::pinMode1(byte pinNumber, bool state)
{
  byte oldValue = CONFIGPORT1;
  if(this->twiRead(oldValue) && (pinNumber <= 7))
  {
    if(!state)
    {
      oldValue |= (1 << pinNumber);
      if(this->portMode1(oldValue))
        return true;
      return false;
    }
    else if(state)
    {
      oldValue &= ~(1 << pinNumber);
      if(this->portMode1(oldValue))
        return true;
      return false;
    }
  }
  return false;
} */

bool PCA9554::portMode0(byte value)
{
  if (this->twiWrite(CONFIGPORT0, value))
    return true;
  return false;
}

/* bool PCA9554::portMode1(byte value)
{
  if(this->twiWrite(CONFIGPORT1, value))
    return true;
  return false;
} */

bool PCA9554::digitalWrite0(byte pinNumber, bool state)
{
  byte oldValue = OUTPUTPORT0;
  if (this->twiRead(oldValue) && pinNumber <= 7)
  {
    if (state)
    {
      oldValue |= (1 << pinNumber);
      if (this->digitalWritePort0(oldValue))
        return true;
      return false;
    }
    else if (!state)
    {
      oldValue &= ~(1 << pinNumber);
      if (this->digitalWritePort0(oldValue))
        return true;
      return false;
    }
  }
  return false;
}

/* bool PCA9554::digitalWrite1(byte pinNumber, bool state)
{
  byte oldValue = OUTPUTPORT1;
  if(this->twiRead(oldValue) && pinNumber <= 7)
  {
    if(state)
    {
      oldValue |= (1 << pinNumber);
      if(this->digitalWritePort1(oldValue))
        return true;
      return false;
    }
    else if(!state)
    {
      oldValue &= ~(1 << pinNumber);
      if(this->digitalWritePort1(oldValue))
        return true;
      return false;
    }
  }
  return false;
} */

bool PCA9554::digitalWritePort0(byte value)
{
  if (this->twiWrite(OUTPUTPORT0, value))
    return true;
  return false;
}

/* bool PCA9554::digitalWritePort1(byte value)
{
  if(this->twiWrite(OUTPUTPORT1, value))
    return true;
  return false;
} */

bool PCA9554::digitalRead0(byte &pinNumber)
{
  byte oldValue = INPUTPORT0;
  if (this->twiRead(oldValue) && (pinNumber <= 7))
  {
    oldValue &= (1 << pinNumber);
    if (oldValue > 0) pinNumber = 1;
    else pinNumber = 0;
    return true;
  }
  return false;
}

/* bool PCA9554::digitalRead1(byte &pinNumber)
{
  byte oldValue = INPUTPORT1;
  if(this->twiRead(oldValue) && (pinNumber <= 7))
  {
    oldValue &= (1 << pinNumber);
    if(oldValue > 0) pinNumber = 1;
    else pinNumber = 0;
    return true;
  }
  return false;
}
 */
bool PCA9554::digitalReadPort0(byte &value)
{
  value = INPUTPORT0;
  if (this->twiRead(value))
    return true;
  return false;
}

/* bool PCA9554::digitalReadPort1(byte &value)
{
  value = INPUTPORT1;
  if(this->twiRead(value))
    return true;
  return false;
}
 */