#pragma once
#include "flprogUtilites.h"
#include "RT_HW_i2c_MCP4725.hpp"

class FLProgMCP4725 : public AbstractI2CDevice
{
public:
  FLProgMCP4725(uint8_t address, uint8_t bus, uint32_t speed = RT_HW_I2C_SPEED);                  // -- Конструктор с указанием адреса, шины и скорости I2C
  FLProgMCP4725(uint8_t address, uint8_t bus, uint32_t speed, uint8_t expander, uint8_t channel); // -- Конструктор с указанием адреса, шины, скорости I2C, а также параметров расширителя и канала

  void setValue(uint16_t value) { _sensor.extVar = value; };     //  -- Установка значения для MCP4725. Принимает значение от 0 до 4095
  void setIsSaveToEEPROM(bool save) { _sensor.vEEPROM = save; }; //  -- Установка флага сохранения значения в EEPROM (true - сохранять, false - не сохранять)

protected:
  void init() { _status = FLPROG_READY_STATUS; };
  void workPool();

  RT_HW_i2c_MCP4725 _sensor;
};