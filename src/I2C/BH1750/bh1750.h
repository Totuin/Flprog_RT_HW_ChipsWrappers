#pragma once
#include "flprogUtilites.h"
#include "RT_HW_i2c_BH1750.hpp"

class FLProgBH1750 : public AbstractTaskDevice
{
public:
  FLProgBH1750(uint8_t address, uint8_t bus, uint32_t speed = RT_HW_I2C_SPEED);                  // -- Конструктор с указанием адреса, шины и скорости I2C
  FLProgBH1750(uint8_t address, uint8_t bus, uint32_t speed, uint8_t expander, uint8_t channel); // -- Конструктор с указанием адреса, шины, скорости I2C, а также параметров расширителя и канала

  uint32_t getIlluminance() { return _sensor.vLux; }; //  -- Получение текущей освещенности в люксах

protected:
  void init() { _status = FLPROG_READY_STATUS; };
  void workPool();

  RT_HW_BH1750 _sensor;
};
