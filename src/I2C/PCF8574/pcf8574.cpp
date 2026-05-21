#include "pcf8574.h"

FLProgPCF8574::FLProgPCF8574(uint8_t address, uint8_t bus, uint32_t speed)
{
  RT_HW_Base.i2cSetParam(_sensor.dvi, address, bus, speed);
  _sensor.flagInvIn = 0;
  _sensor.flagInvOut = 0;
  _sensor.cnfgA = 0xFF;
  _sensor.custom = 1;
}

FLProgPCF8574::FLProgPCF8574(uint8_t address, uint8_t bus, uint32_t speed, uint8_t expander, uint8_t channel)
{
  RT_HW_Base.i2cSetParam(_sensor.dvi, address, bus, speed, expander, channel);
  _sensor.flagInvIn = 0;
  _sensor.flagInvOut = 0;
  _sensor.cnfgA = 0xFF;
  _sensor.custom = 1;
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
  bool value = mode == INPUT;
  bitWrite(_sensor.cnfgA, pin, value);
}

void FLProgPCF8574::write(uint8_t pin, bool value)
{
  if (pin > 7)
  {
    return;
  }
  bitWrite(_sensor.extA, pin, value);
}

bool FLProgPCF8574::read(uint8_t pin)
{
  if (pin > 7)
  {
    return false;
  }
  return bitRead(_sensor.inA, pin);
}