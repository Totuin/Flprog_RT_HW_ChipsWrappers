#pragma once
#include "flprogUtilites.h"
#include "RT_HW_pin_DHT22.hpp"

class FLProgDHT22 : public AbstractTaskDevice
{
public:
  FLProgDHT22(uint8_t dataPin); // -- Конструктор с указанием пина данных

  float getTemperature() { return _sensor.vT / 10.0; }; //  -- Получение текущей температуры в градусах Цельсия
  float getHumidity() { return _sensor.vH / 10.0; };    //  -- Получение текущей влажности в процентах

protected:
  void init() { _status = FLPROG_READY_STATUS; };
  void workPool();

  RT_HW_DHT22 _sensor;
};