#include "chip74HC165.h"

FLProgSpi74HC165::FLProgSpi74HC165(uint8_t bus, uint8_t csPin, uint32_t speed)
{
  _csPin = csPin;
  _bus = bus;
  _speed = speed;
}

FLProgSpn74HC165::FLProgSpn74HC165(uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin, uint8_t csPin, uint8_t speed)
{
  _sckPin = sckPin;
  _misoPin = misoPin;
  _mosiPin = mosiPin;
  _csPin = csPin;
  _speed = speed;
}

void FLProgAbstract74HC165::setChipCount(uint8_t count)
{
  if ((count > 4) || (count == 0))
  {
    return;
  }
  _qnt = count;
}


uint8_t FLProgAbstract74HC165::readChipByte(uint8_t chip)
{
  if ((chip > _qnt) || (chip == 0))
  {
    return 0;
  }
  uint32_t temp = extVar();
  if (chip == 1)
  {
    return temp & 0xFF;
  }
  if (chip == 2)
  {
    return (temp >> 8) & 0xFF;
  }
  if (chip == 3)
  {
    return (temp >> 16) & 0xFF;
  }
  return (temp >> 24) & 0xFF;
}

bool FLProgAbstract74HC165::readBitOnChip(uint8_t bit, uint8_t chip)
{
  if ((chip > _qnt) || (chip == 0))
  {
    return false;
  }
  if (bit > 7)
  {
    return false;
  }
  uint8_t binNumber = ((chip - 1) * 8) + bit;
  return bitRead(extVar(), binNumber);
}

void FLProgSpi74HC165::workPool()
{

  if (!_sensor.dvS.custom)
  {
    _sensor.qnt = _qnt;
    _sensor.dvS.bitOrder = _bitOrder;
    _sensor.inv = _invertInput;
    RT_HW_Base.spiSetParam(_sensor.dvS, _csPin, _bus, _speed);
  }
  uint8_t taskRun = _task.run();
  _sensor.direct(taskRun);
  _task.run(taskRun);
}

void FLProgSpn74HC165::workPool()
{
  if (!_sensor.dvS.custom)
  {
    _sensor.qnt = _qnt;
    _sensor.dvS.bitOrder = _bitOrder;
    _sensor.inv = _invertInput;
    RT_HW_Base.spnSetParam(_sensor.dvS, _sckPin, _misoPin, _mosiPin, _csPin, (uint8_t)_speed);
  }
  uint8_t taskRun = _task.run();
  _sensor.direct(taskRun);
  _task.run(taskRun);
}