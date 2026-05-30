#include "dht22.h"

FLProgDHT22::FLProgDHT22(uint8_t dataPin)
{

  _dataPin = dataPin;
}

void FLProgDHT22::workPool()
{
  if (_sensor.custom == 0)
  {
    _sensor.pin = _dataPin;
    _sensor.custom = 1;
  }
  uint8_t taskRun = _task.run();
  _sensor.direct(taskRun);
  _task.run(taskRun);
  _sensor.fresh = 0;
}