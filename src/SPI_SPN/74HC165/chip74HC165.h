#pragma once
#include "flprogUtilites.h"
#include "RT_HW_spx_74HC165.hpp"

class FLProgAbstract74HC165 : public AbstractSPIorSPNDevice
{
public:
  void setChipCount(uint8_t count);                            //  -- Установка количества подключенных чипов 74HC165. Принимает значение от 1 до 4
  void setBitOrder(uint8_t order) { _bitOrder = order; };      //  -- Установка порядка битов при чтении данных (0 - младший бит первым, 1 - старший бит первым)
  void setInvertInput(bool invert) { _invertInput = invert; }; //  -- Установка инверсии для входов (true - инвертировать, false - не инвертировать)

  uint8_t readChipByte(uint8_t chip);            //  -- Чтение байта данных с указанного чипа 74HC165. Принимает номер чипа от 1 до количества подключенных чипов. Возвращает байт данных, считанный с указанного чипа.
  bool readBitOnChip(uint8_t bit, uint8_t chip); //  -- Чтение бита данных с указанного чипа 74HC165. Принимает номер чипа от 1 до количества подключенных чипов и номер бита от 0 до 7. Возвращает значение бита (0 или 1) с указанного чипа.

protected:
  void init() { _status = FLPROG_READY_STATUS; };
  virtual uint32_t extVar() = 0;

  uint8_t _qnt = 1;          // Количество подключенных чипов 74HC165. Принимает значение от 1 до 8
  uint8_t _bitOrder = 0;     // Порядок битов при чтении данных (0 - младший бит первым, 1 - старший бит первым)
  bool _invertInput = false; // Инверсия для входов (true - инвертировать, false - не инвертировать)
};

class FLProgSpi74HC165 : public FLProgAbstract74HC165
{
public:
  FLProgSpi74HC165(uint8_t bus, uint8_t csPin, uint32_t speed = RT_HW_SPI_SPEED); // -- Конструктор с указанием шины, пина CS и скорости SPI
  void workPool();

protected:
  uint32_t extVar() { return _sensor.extVar; };

  RT_HW_spi_74HC165 _sensor;
};

class FLProgSpn74HC165 : public FLProgAbstract74HC165
{
public:
  FLProgSpn74HC165(uint8_t sckPin, uint8_t misoPin, uint8_t mosiPin, uint8_t csPin, uint8_t speed = 100); // -- Конструктор с указанием пинов SCK, MISO, MOSI, CS и скорости SPN в процентах (10%-100% )
  void workPool();

protected:
  uint32_t extVar() { return _sensor.extVar; };

  RT_HW_spn_74HC165 _sensor;
};
