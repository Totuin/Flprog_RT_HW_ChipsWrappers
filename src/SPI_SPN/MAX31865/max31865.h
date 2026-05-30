#pragma once
#include "flprogUtilites.h"
// #include "RT_HW_spi_MAX31865.hpp"

class FLProgSpiMAX31865 : public AbstractSPIorSPNDevice
{
public:
  FLProgSpiMAX31865(uint8_t bus, uint8_t csPin, uint32_t speed = RT_HW_SPI_SPEED); // -- Конструктор с указанием шины, пина CS, пина OE и скорости SPI

  void workPool();
  // float getTemperature() { return _sensor.vTemp; }; //  -- Получение текущей температуры в градусах Цельсия
  float getTemperature() { return 0.0; };
  bool getOk() { return true; };

protected:
  void init() { _status = FLPROG_READY_STATUS; };

  // RT_HW_MAX31865 _sensor;
};

class FLProgSpnMAX31865 : public AbstractSPIorSPNDevice
{
public:
  FLProgSpnMAX31865(uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin, uint8_t csPin, uint8_t speed = 100); // -- Конструктор с указанием пинов SCK, MISO, MOSI, CS, OE и скорости SPN в процентах (10%-100% )

  void workPool();
  float getTemperature() { return 0.0; };
  // float getTemperature() { return _sensor.vTemp; }; //  -- Получение текущей температуры в градусах Цельсия
  bool getOk() { return true; };

protected:
  void init() { _status = FLPROG_READY_STATUS; };

  // RT_HW_MAX31865_SPN _sensor;
};
