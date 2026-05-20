#include "pcf8574.h"

FLProgPCF8574::FLProgPCF8574(uint8_t address, uint8_t bus, uint32_t speed)
{
  RT_HW_Base.i2cSetParam(_device, address, bus, speed);
  _sensor.flagInvIn = 0;
  _sensor.flagInvOut = 0;
  _sensor.cnfgA = 0xFF;
}

FLProgPCF8574::FLProgPCF8574(uint8_t address, uint8_t bus, uint32_t speed, uint8_t expander, uint8_t channel)
{
  RT_HW_Base.i2cSetParam(_device, address, bus, speed, expander, channel);
  _sensor.flagInvIn = 0;
  _sensor.flagInvOut = 0;
  _sensor.cnfgA = 0xFF;
}

void FLProgPCF8574::workPool()
{
  uint8_t taskRun = _task.run();
  _sensor.direct(taskRun);
  _task.run(taskRun);
  _sensor.fresh = 0;
}

void FLProgPCF8574::pinMode(uint8_t pin, uint8_t mode)
{
  if (pin > 7)
  {
    return;
  }
  if ((mode != INPUT) && (mode != OUTPUT))
  {
    return;
  }
  if (mode == INPUT)
  {
    _sensor.cnfgA |= (1 << pin);
    return;
  }
  _sensor.cnfgA &= ~(1 << pin);
}

void FLProgPCF8574::write(uint8_t pin, bool value)
{
  if (pin > 7)
  {
    return;
  }
  if (value)
  {
    _sensor.extA |= (1 << pin);
    return;
  }
  _sensor.extA &= ~(1 << pin);
}

bool FLProgPCF8574::read(uint8_t pin)
{
  if (pin > 7)
  {
    return false;
  }
  return (_sensor.inA & (1 << pin)) ? true : false;
}