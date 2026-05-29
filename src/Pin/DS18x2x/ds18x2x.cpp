#include "ds18x2x.h"

FLProgDs18x2x::FLProgDs18x2x(uint8_t dataPin, uint8_t *adress, bool isExternalArray)
{
  uint8_t _pin = dataPin;
  if (isExternalArray)
  {
    _addressMode = FLPROG_DS18X2X_ARRAY_ADDRESS_MODE;
  }
  else
  {
    _addressMode = FLPROG_DS18X2X_EXTERNAL_ARRAY_ADDRESS_MODE;
  }
  _adress = adress;
}

FLProgDs18x2x::FLProgDs18x2x(uint8_t dataPin)
{
  uint8_t _pin = dataPin;
}

void FLProgDs18x2x::workPool()
{
  if (!_sensor.custom)
  {
    _sensor.pin = _pin;
    if (_addressMode != FLPROG_DS18X2X_AUTO_ADDRESS_MODE)
    {
      if (_adress != 0)
      {
        for (uint8_t i = 0; i < 8; i++)
        {
          _sensor.addr[i] = _adress[i];
        }
      }
    }
    _sensor.custom = 1;
  }
  uint8_t taskRun = _task.run();
  if (taskRun)
  {
    if (_addressMode = FLPROG_DS18X2X_EXTERNAL_ARRAY_ADDRESS_MODE)
    {
      if (_adress != 0)
      {
        for (uint8_t i = 0; i < 8; i++)
        {
          if (_sensor.addr[i] != _adress[i])
          {
            _sensor.custom = 0;
            _sensor.dir = 0;
            return;
          }
        }
      }
    }
  }
  _sensor.direct(taskRun);
  _task.run(taskRun);
}