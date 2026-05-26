#include "dht22.h"

FLProgDHT22::FLProgDHT22(uint8_t dataPin)
{
  _sensor.pin = dataPin;
  _sensor.custom = 1;
}

void FLProgDHT22::workPool()
{
  uint8_t taskRun = _task.run();
  _sensor.direct(taskRun);
  _task.run(taskRun);
  _sensor.fresh = 0;
}