#pragma once
#include "flprogUtilites.h"
#include "RT_HW_i2c_BMP180.hpp"

class FLProgBMP180 : public AbstractTaskDevice
{
public:
  FLProgBMP180(uint8_t address, uint8_t bus, uint32_t speed = RT_HW_I2C_SPEED);                  // -- Конструктор с указанием адреса, шины и скорости I2C
  FLProgBMP180(uint8_t address, uint8_t bus, uint32_t speed, uint8_t expander, uint8_t channel); // -- Конструктор с указанием адреса, шины, скорости I2C, а также параметров расширителя и канала

  float getTemperature() { return _sensor.vTemp; }; //  -- Получение текущей температуры в градусах Цельсия
  float getPressure() { return _sensor.vPress; };   //  -- Получение текущего давления в гПа

protected:
  void init() { _status = FLPROG_READY_STATUS; };
  void workPool();

  RT_HW_BMP180 _sensor;
};
