#pragma once
#include "flprogUtilites.h"
#include "RT_HW_spx_74HC595.hpp"

class FLProgAbstract74HC595 : public AbstractSPIorSPNDevice
{
public:
  void setChipCount(uint8_t count);                       //  -- Установка количества подключенных чипов 74HC595. Принимает значение от 1 до 4
  void setBitOrder(uint8_t order) { _bitOrder = order; }; //  -- Установка порядка битов при чтении данных (0 - младший бит первым, 1 - старший бит первым)

  void writeByte(uint8_t chip, uint8_t data);                 //  -- Запись байта данных на указанный чип 74HC595. Принимает номер чипа от 1 до количества подключенных чипов и байт данных для записи. Записывает указанный байт данных на указанный чип 74HC595.
  void writeBtiOnChip(uint8_t bit, uint8_t chip, bool value); //  -- Запись бита данных на указанный чип 74HC595. Принимает номер чипа от 1 до количества подключенных чипов, номер бита от 0 до 7 и значение бита (true - высокий уровень, false - низкий уровень) для записи. Записывает указанный бит данных на указанный чип 74HC595.

protected:
  void init() { _status = FLPROG_READY_STATUS; };
  virtual uint32_t extVar() = 0;
  virtual void extVar(uint32_t value) = 0;
  virtual void privateBitWrite(uint8_t bit, bool value) = 0;

  uint8_t _qnt = 1;      // Количество подключенных чипов 74HC165. Принимает значение от 1 до 8
  uint8_t _bitOrder = 0; // Порядок битов при чтении данных (0 - младший бит первым, 1 - старший бит первым)
  uint8_t _pinOE = 255;  // Номер пина OE для 74HC595. 255 - не используется
};

class FLProgSpi74HC595 : public FLProgAbstract74HC595
{
public:
  FLProgSpi74HC595(uint8_t bus, uint8_t csPin, uint8_t pinOE, uint32_t speed = RT_HW_SPI_SPEED); // -- Конструктор с указанием шины, пина CS, пина OE и скорости SPI

  void workPool();

protected:
  uint32_t extVar() { return _sensor.extVar; };
  void extVar(uint32_t value) { _sensor.extVar = value; };
  void privateBitWrite(uint8_t bit, bool value) { bitWrite(_sensor.extVar, bit, value); };

  RT_HW_spi_74HC595 _sensor;
};

class FLProgSpn74HC595 : public FLProgAbstract74HC595
{
public:
  FLProgSpn74HC595(uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin, uint8_t csPin, uint8_t pinOE, uint8_t speed = 100); // -- Конструктор с указанием пинов SCK, MISO, MOSI, CS, OE и скорости SPN в процентах (10%-100% )

  void workPool();

protected:
  uint32_t extVar() { return _sensor.extVar; };
  void extVar(uint32_t value) { _sensor.extVar = value; };
  void privateBitWrite(uint8_t bit, bool value) { bitWrite(_sensor.extVar, bit, value); };

  RT_HW_spn_74HC595 _sensor;
};
