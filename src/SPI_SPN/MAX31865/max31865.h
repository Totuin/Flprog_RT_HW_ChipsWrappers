#pragma once
#include "flprogUtilites.h"
// #include "RT_HW_spi_MAX31865.hpp"

/**
 * @brief Заготовка интерфейса MAX31865 через SPI.
 * @details Намеренная заготовка: драйвер RT_HW ещё не разработан. Аппаратный обмен не выполняется, getTemperature() возвращает 0.0.
 */
class FLProgSpiMAX31865 : public AbstractSPIorSPNDevice
{
public:
  FLProgSpiMAX31865(uint8_t bus, uint8_t csPin, uint32_t speed = RT_HW_SPI_SPEED); // -- Конструктор с указанием шины, пина CS и скорости SPI

  void workPool();
  // float getTemperature() { return _sensor.vTemp; }; //  -- Получение текущей температуры в градусах Цельсия
  // ЗАГЛУШКА: пока датчик не опрашивается, это НЕ измеренная температура!
  float getTemperature() { return 0.0; };
  // Пока не реализована диагностика, true не подтверждает связь с MAX31865:
  // текущая реализация — заготовка без драйвера.
  bool getOk() { return true; };

protected:
  void init() { _status = FLPROG_READY_STATUS; };

  // RT_HW_MAX31865 _sensor;
};

/**
 * @brief Заготовка интерфейса MAX31865 через SPN.
 * @details Намеренная заготовка: драйвер RT_HW ещё не разработан. Аппаратный обмен не выполняется, getTemperature() возвращает 0.0.
 */
class FLProgSpnMAX31865 : public AbstractSPIorSPNDevice
{
public:
  FLProgSpnMAX31865(uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin, uint8_t csPin, uint8_t speed = 100); // -- Конструктор с указанием SCK, MISO, MOSI, CS и скорости SPN в процентах (10%-100%).

  void workPool();
  // ЗАГЛУШКА: пока датчик не опрашивается, это НЕ измеренная температура!
  float getTemperature() { return 0.0; };
  // float getTemperature() { return _sensor.vTemp; }; //  -- Получение текущей температуры в градусах Цельсия
  // Пока не реализована диагностика, true не подтверждает связь с MAX31865:
  // текущая реализация — заготовка без драйвера.
  bool getOk() { return true; };

protected:
  void init() { _status = FLPROG_READY_STATUS; };

  // RT_HW_MAX31865_SPN _sensor;
};
