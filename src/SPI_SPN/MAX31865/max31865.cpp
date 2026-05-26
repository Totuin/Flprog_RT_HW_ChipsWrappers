#include "max31865.h"

FLProgSpiMAX31865::FLProgSpiMAX31865(uint8_t bus, uint8_t csPin, uint32_t speed)
{
  _csPin = csPin;
  _bus = bus;
  _speed = speed;
}

FLProgSpnMAX31865::FLProgSpnMAX31865(uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin, uint8_t csPin, uint8_t speed)
{
  _sckPin = sckPin;
  _misoPin = misoPin;
  _mosiPin = mosiPin;
  _csPin = csPin;
  _speed = speed;
}

void FLProgSpiMAX31865::workPool()
{
  /*
  if (!_sensor.custom)
  {
    _sensor.custom = 1;
    RT_HW_Base.spiSetParam(_sensor.dvS, _csPin, _bus, _speed);
  }
  uint8_t taskRun = _task.run();
  _sensor.direct(taskRun);
  _task.run(taskRun);
  */
}

void FLProgSpnMAX31865::workPool()
{
  /*
   if (!_sensor.custom)
   {
     _sensor.custom = 1;
     RT_HW_Base.spnSetParam(_sensor.dvS, _sckPin, _misoPin, _mosiPin, _csPin, (uint8_t)_speed);
   }
   uint8_t taskRun = _task.run();
   _sensor.direct(taskRun);
   _task.run(taskRun);
   */
}