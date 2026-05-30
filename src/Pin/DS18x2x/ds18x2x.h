#pragma once
#include "flprogUtilites.h"
#include "RT_HW_pin_DS1820.hpp"

#define FLPROG_DS18X2X_AUTO_ADDRESS_MODE 0
#define FLPROG_DS18X2X_ARRAY_ADDRESS_MODE 1
#define FLPROG_DS18X2X_EXTERNAL_ARRAY_ADDRESS_MODE 2

class FLProgDs18x2x : public AbstractTaskDevice
{
public:
  FLProgDs18x2x(uint16_t busIndex, uint8_t dataPin, uint8_t *adress, bool isExternalArray = false);
  FLProgDs18x2x(uint16_t busIndex, uint8_t dataPin);

  float getTemperature() { return _sensor.getTemp(_sensor.raw); }; //  -- Получение текущей температуры в градусах Цельсия
  uint16_t pool(uint16_t useBus);
  void parasitePower(bool value) ;

  bool getOk() { return _sensor.ok > 0; };

protected:
  void
  init() { _status = FLPROG_READY_STATUS; };
  void workPool() {};
  void checkAddress();
  void copyAddress();

  uint8_t _pin = 255;
  uint8_t _addressMode = FLPROG_DS18X2X_AUTO_ADDRESS_MODE;
  uint8_t *_adress = 0;
  uint16_t _busIndex;
  OneWire _oneWare;
  RT_HW_DS1820 _sensor;
};