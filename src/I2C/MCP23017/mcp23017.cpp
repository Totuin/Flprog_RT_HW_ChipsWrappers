#include "mcp23017.h"

FLProgMCP23017::FLProgMCP23017(uint8_t address, uint8_t bus, uint32_t speed)
{
  RT_HW_Base.i2cSetParam(_device, address, bus, speed);
  _sensor.invIn = 0;
  _sensor.invOut = 0;
  _sensor.cnfgA = 0xFF;
  _sensor.cnfgB = 0xFF;
}

FLProgMCP23017::FLProgMCP23017(uint8_t address, uint8_t bus, uint32_t speed, uint8_t expander, uint8_t channel)
{
  RT_HW_Base.i2cSetParam(_device, address, bus, speed, expander, channel);
  _sensor.invIn = 0;
  _sensor.invOut = 0;
  _sensor.cnfgA = 0xFF;
  _sensor.cnfgB = 0xFF;
}

void FLProgMCP23017::workPool()
{
  uint8_t runIn = 1;
  uint8_t runOut = 1;
  if ((_sensor.cnfgA == 0xFF) && (_sensor.cnfgB == 0xFF))
  {
    runOut = 0;
  }
  if ((_sensor.cnfgA == 0x00) && (_sensor.cnfgB == 0x00))
  {
    runIn = 0;
  }
  _sensor.direct(runIn, runOut);
  _sensor.fresh = 0;
}

void FLProgMCP23017::pinMode(uint8_t pin, uint8_t mode)
{
  if (pin > 15)
  {
    return;
  }
  if ((mode != INPUT) && (mode != OUTPUT))
  {
    return;
  }
  if (pin < 8)
  {
    if (mode == INPUT)
    {
      _sensor.cnfgA |= (1 << pin);
      return;
    }
    _sensor.cnfgA &= ~(1 << pin);
    return;
  }
  uint8_t pinB = pin - 8;
  if (mode == INPUT)
  {
    _sensor.cnfgB |= (1 << pinB);
    return;
  }
  _sensor.cnfgB &= ~(1 << pinB);
}

void FLProgMCP23017::write(uint8_t pin, bool value)
{
  if (pin > 15)
  {
    return;
  }
  if (pin < 8)
  {
    if (((_sensor.inA & (1 << pin)) ? true : false) != value)
    {
      _task.reset();
    }
    if (value)
    {
      _sensor.extA |= (1 << pin);
      return;
    }
    _sensor.extA &= ~(1 << pin);
    return;
  }
  uint8_t pinB = pin - 8;
  if (((_sensor.inB & (1 << pinB)) ? true : false) != value)
  {
    _task.reset();
  }
  if (value)
  {
    _sensor.extB |= (1 << pinB);
    return;
  }
  _sensor.extB &= ~(1 << pinB);
}

bool FLProgMCP23017::read(uint8_t pin)
{
  if (pin > 15)
  {
    return false;
  }
  if (pin < 8)
  {
    return (_sensor.inA & (1 << pin)) ? true : false;
  }
  uint8_t pinB = pin - 8;
  return (_sensor.inB & (1 << pinB)) ? true : false;
}