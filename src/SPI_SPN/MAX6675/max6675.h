#pragma once
#include "flprogUtilites.h"
#include "RT_HW_spi_MAX6675.hpp"

class FLProgSpiMAX6675 : public AbstractSPIorSPNDevice
{
public:
  FLProgSpiMAX6675(uint8_t bus, uint8_t csPin, uint32_t speed = RT_HW_SPI_SPEED); // -- Конструктор с указанием шины, пина CS, пина OE и скорости SPI

  void workPool();
  float getTemperature() { return _sensor.vTemp; }; //  -- Получение текущей температуры в градусах Цельсия
  bool getOk() { return _sensor.ok > 0; };

protected:
  void init() { _status = FLPROG_READY_STATUS; };

  RT_HW_MAX6675 _sensor;
};

class FLProgSpnMAX6675 : public AbstractSPIorSPNDevice
{
public:
  FLProgSpnMAX6675(uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin, uint8_t csPin, uint8_t speed = 100); // -- Конструктор с указанием пинов SCK, MISO, MOSI, CS, OE и скорости SPN в процентах (10%-100% )

  void workPool();
  float getTemperature() { return _sensor.vTemp; }; //  -- Получение текущей температуры в градусах Цельсия
  bool getOk() { return _sensor.ok > 0; };

protected:
  void init() { _status = FLPROG_READY_STATUS; };

  RT_HW_MAX6675_SPN _sensor;
};
