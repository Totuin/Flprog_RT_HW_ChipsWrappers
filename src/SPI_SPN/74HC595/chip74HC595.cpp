#include "chip74HC595.h"

FLProgSpi74HC595::FLProgSpi74HC595(uint8_t bus, uint8_t csPin, uint8_t pinOE, uint32_t speed)
{
  _csPin = csPin;
  _pinOE = pinOE;
  _bus = bus;
  _speed = speed;
}

FLProgSpn74HC595::FLProgSpn74HC595(uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin, uint8_t csPin, uint8_t pinOE, uint8_t speed)
{
  _sckPin = sckPin;
  _misoPin = misoPin;
  _mosiPin = mosiPin;
  _csPin = csPin;
  _pinOE = pinOE;
  _speed = speed;
}

void FLProgAbstract74HC595::setChipCount(uint8_t count)
{
  if ((count > 4) || (count == 0))
  {
    return;
  }
  _qnt = count;
}

void FLProgAbstract74HC595::writeByte(uint8_t chip, uint8_t data)
{

  if ((chip > _qnt) || (chip == 0))
  {
    return;
  }
  uint32_t temp1 = extVar();
  uint8_t temp[4];
  if (chip == 1)
  {
    temp[0] = data;
  }
  else
  {
    temp[0] = temp1 & 0xFF;
  }
  if (chip == 2)
  {
    temp[1] = data;
  }
  else
  {
    temp[1] = (temp1 >> 8) & 0xFF;
  }
  if (chip == 3)
  {
    temp[2] = data;
  }
  else
  {
    temp[2] = (temp1 >> 16) & 0xFF;
  }
  if (chip == 4)
  {
    temp[3] = data;
  }
  else
  {
    temp[3] = (temp1 >> 24) & 0xFF;
  }
  temp1 = (((uint32_t)(temp[3])) << 24) | (((uint32_t)(temp[2])) << 16) | (((uint16_t)(temp[1])) << 8) | ((uint8_t)(temp[0]));
  extVar(temp1);
}

void FLProgAbstract74HC595::writeBtiOnChip(uint8_t bit, uint8_t chip, bool value)
{
  if ((chip > _qnt) || (chip == 0))
  {
    return;
  }
  if (bit > 7)
  {
    return;
  }
  uint8_t binNumber = ((chip - 1) * 8) + bit;
  privateBitWrite(binNumber, value);
}

void FLProgSpi74HC595::workPool()
{
  if (!_sensor.dvS.custom)
  {
    _sensor.qnt = _qnt;
    _sensor.dvS.bitOrder = _bitOrder;
    _sensor.oe = _pinOE;
    RT_HW_Base.spiSetParam(_sensor.dvS, _csPin, _bus, _speed);
  }
  uint8_t taskRun = _task.run();
  _sensor.direct(taskRun);
  _task.run(taskRun);
}

void FLProgSpn74HC595::workPool()
{
  if (!_sensor.dvS.custom)
  {
    _sensor.qnt = _qnt;
    _sensor.dvS.bitOrder = _bitOrder;
    _sensor.oe = _pinOE;
    RT_HW_Base.spnSetParam(_sensor.dvS, _sckPin, _misoPin, _mosiPin, _csPin, (uint8_t)_speed);
  }
  uint8_t taskRun = _task.run();
  _sensor.direct(taskRun);
  _task.run(taskRun);
}