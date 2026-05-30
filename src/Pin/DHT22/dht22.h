#pragma once
#include "flprogUtilites.h"
#include "RT_HW_pin_DHT22.hpp"

class FLProgDHT22 : public AbstractTaskDevice
{
public:
  FLProgDHT22(uint8_t dataPin); // -- Конструктор с указанием пина данных

  float getTemperature() { return _sensor.vT / 10.0; }; //  -- Получение текущей температуры в градусах Цельсия
  float getHumidity() { return _sensor.vH / 10.0; };    //  -- Получение текущей влажности в процентах

  bool getOk() { return _sensor.ok > 0; };
  void setDataPin(uint8_t pin) { _dataPin = pin; };

protected:
  void init() { _status = FLPROG_READY_STATUS; };
  void workPool();

  uint8_t _dataPin = 255;
  RT_HW_DHT22 _sensor;
};