#include "ds18x2x.h"

FLProgDs18x2x::FLProgDs18x2x(uint16_t busIndex, uint8_t dataPin, uint8_t *adress, bool isExternalArray)
{
  _pin = dataPin;
  _busIndex =  busIndex;
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

FLProgDs18x2x::FLProgDs18x2x(uint16_t busIndex, uint8_t dataPin)
{
  _pin = dataPin;
  _busIndex =  busIndex;
}

uint16_t FLProgDs18x2x::pool(uint16_t useBus)
{
  if (!_sensor.custom)
  {
    _sensor.pin = _pin;
    if (_addressMode == FLPROG_DS18X2X_AUTO_ADDRESS_MODE)
    {
      _sensor.modeGroup = 0;
    }
    else
    {
      _sensor.modeGroup = 1;
    }
    copyAddress();
    _sensor.custom = 1;
  }
  if ((useBus != 0) && (useBus != _busIndex))
  {
    return useBus;
  }
  if (!_task.isInit())
  {
    _task.setType(_taskMode);
    _task.setQntPass(_taskQntPass);
    _task.setPeriod(_taskPeriod);
  }
  _task.direct(_taskEn);

  uint8_t taskRun = _task.run();
  if (!taskRun)
  {
    return useBus;
  }
  checkAddress();
  _sensor.direct(_oneWare, taskRun);
  _task.run(taskRun);
  if (_sensor.busy)
  {
    return _busIndex;
  }
  return 0;
}

void FLProgDs18x2x::checkAddress()
{
  if (_addressMode != FLPROG_DS18X2X_EXTERNAL_ARRAY_ADDRESS_MODE)
  {
    return;
  }
  if (_adress == 0)
  {
    return;
  }
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

void FLProgDs18x2x::copyAddress()
{
  if (_addressMode == FLPROG_DS18X2X_AUTO_ADDRESS_MODE)
  {
    return;
  }
  if (_adress == 0)
  {
    return;
  }
  for (uint8_t i = 0; i < 8; i++)
  {
    _sensor.addr[i] = _adress[i];
  }
}

void FLProgDs18x2x::parasitePower(bool value)
{
  if (value)
  {
    _sensor.power = 0;
  }
  else
  {
    _sensor.power = 1;
  }
}