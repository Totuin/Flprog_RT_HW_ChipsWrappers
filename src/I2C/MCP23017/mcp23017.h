#pragma once
#include "flprogUtilites.h"
#include "RT_HW_i2c_MCP23017.hpp"

class FLProgMCP23017 : public AbstractTaskDevice
{
public:
  FLProgMCP23017(uint8_t address, uint8_t bus, uint32_t speed = RT_HW_I2C_SPEED);                  // -- Конструктор с указанием адреса, шины и скорости I2C
  FLProgMCP23017(uint8_t address, uint8_t bus, uint32_t speed, uint8_t expander, uint8_t channel); // -- Конструктор с указанием адреса, шины, скорости I2C, а также параметров расширителя и канала

  void pinMode(uint8_t pin, uint8_t mode);                        //  -- Установка режима работы пина (INPUT или OUTPUT)
  void setInvertInput(bool invert) { _sensor.invIn = invert; };   //  -- Установка инверсии для входов (true - инвертировать, false - не инвертировать)
  void setInvertOutput(bool invert) { _sensor.invOut = invert; }; //  -- Установка инверсии для выходов (true - инвертировать, false - не инвертировать)
  void write(uint8_t pin, bool value);                            //  -- Запись значения на пин
  bool read(uint8_t pin);                                         //  -- Чтение значения с пина

protected:
  virtual void init() { _status = FLPROG_READY_STATUS; };
  virtual void workPool();

  RT_HW_i2c_MCP23017 _sensor;
};