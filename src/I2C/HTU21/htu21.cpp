#include "htu21.h"

FLProgHTU21::FLProgHTU21(uint8_t address, uint8_t bus, uint32_t speed)
{
  RT_HW_Base.i2cSetParam(_sensor.dvi, address, bus, speed);
}

FLProgHTU21::FLProgHTU21(uint8_t address, uint8_t bus, uint32_t speed, uint8_t expander, uint8_t channel)
{
  RT_HW_Base.i2cSetParam(_sensor.dvi, address, bus, speed, expander, channel);
}

void FLProgHTU21::workPool()
{
  uint8_t taskRun = _task.run();
  _sensor.direct(taskRun);
  _task.run(taskRun);
  _sensor.fresh = 0;
}