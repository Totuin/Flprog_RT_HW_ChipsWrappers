#include "pcf8575.h"

FLProgPCF8575::FLProgPCF8575(uint8_t address, uint8_t bus, uint32_t speed)
{
  RT_HW_Base.i2cSetParam(_device, address, bus, speed);
  _sensor.flagInvIn = 0;
  _sensor.flagInvOut = 0;
  _sensor.cnfgA = 0xFF;
}

FLProgPCF8575::FLProgPCF8575(uint8_t address, uint8_t bus, uint32_t speed, uint8_t expander, uint8_t channel)
{
  RT_HW_Base.i2cSetParam(_device, address, bus, speed, expander, channel);
  _sensor.flagInvIn = 0;
  _sensor.flagInvOut = 0;
  _sensor.cnfgA = 0xFF;
}

void FLProgPCF8575::workPool()
{
  uint8_t taskRun = _task.run();
  _sensor.direct(taskRun);
  _task.run(taskRun);
  _sensor.fresh = 0;
}

void FLProgPCF8575::pinMode(uint8_t pin, uint8_t mode)
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

void FLProgPCF8575::write(uint8_t pin, bool value)
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

bool FLProgPCF8575::read(uint8_t pin)
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