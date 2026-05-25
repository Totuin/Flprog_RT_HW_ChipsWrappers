#pragma once
#include "flprogUtilites.h"
#include "RT_HW_i2c_SHT3x.hpp"

class FLProgSHT3x : public AbstractTaskDevice
{
public:
  FLProgSHT3x(uint8_t address, uint8_t bus, uint32_t speed = RT_HW_I2C_SPEED);                  // -- Конструктор с указанием адреса, шины и скорости I2C
  FLProgSHT3x(uint8_t address, uint8_t bus, uint32_t speed, uint8_t expander, uint8_t channel); // -- Конструктор с указанием адреса, шины, скорости I2C, а также параметров расширителя и канала

  float getTemperature() { return _sensor.vTemp; }; //  -- Получение текущей температуры в градусах Цельсия
  float getHumidity() { return _sensor.vHum; };     //  -- Получение текущей влажности в процентах

protected:
  void init() { _status = FLPROG_READY_STATUS; };
  void workPool();
  RT_HW_SHT3x _sensor;
};
