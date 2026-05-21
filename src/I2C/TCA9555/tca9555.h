#pragma once
#include "flprogUtilites.h"

#define FLPROG_TCA9555_INPUT_PORT_REGISTER_0 0x00  //  read()
#define FLPROG_TCA9555_INPUT_PORT_REGISTER_1 0x01  //
#define FLPROG_TCA9555_OUTPUT_PORT_REGISTER_0 0x02 //  write()
#define FLPROG_TCA9555_OUTPUT_PORT_REGISTER_1 0x03 //
#define FLPROG_TCA9555_POLARITY_REGISTER_0 0x04    //  get/setPolarity()
#define FLPROG_TCA9555_POLARITY_REGISTER_1 0x05    //
#define FLPROG_TCA9555_CONFIGURATION_PORT_0 0x06   //  pinMode()
#define FLPROG_TCA9555_CONFIGURATION_PORT_1 0x07   //

class FLProgTca9555 : public AbstractFLProgI2CDevice
{
public:
  FLProgTca9555(uint8_t address, uint8_t bus);                                                    //  -- Конструктор с указанием адреса и шины (скорость I2C по умолчанию 800000)
  FLProgTca9555(uint8_t address, uint8_t bus, uint32_t speed);                                    //  -- Конструктор с указанием адреса, шины и скорости I2C
  FLProgTca9555(uint8_t address, uint8_t bus, uint8_t expander, uint8_t channel);                 //  -- Конструктор с указанием адреса, шины, а также параметров расширителя и канала (скорость I2C по умолчанию 800000)
  FLProgTca9555(uint8_t address, uint8_t bus, uint32_t speed, uint8_t expander, uint8_t channel); //  -- Конструктор с указанием адреса, шины, скорости I2C, а также параметров расширителя и канала

  void pinMode(uint8_t pin, uint8_t mode);                 //  -- Установка режима работы пина (INPUT или OUTPUT)
  void setInvertInput(bool invert) { _invIn = invert; };   //  -- Установка инверсии для входов (true - инвертировать, false - не инвертировать)
  void setInvertOutput(bool invert) { _invOut = invert; }; //  -- Установка инверсии для выходов (true - инвертировать, false - не инвертировать)
  void write(uint8_t pin, bool value);                     //  -- Запись значения на пин
  bool read(uint8_t pin);                                  //  -- Чтение значения с пина


protected:
  virtual void init();
  void privatePinMode(uint8_t pin, uint8_t mode);
  virtual void workPool();

  uint8_t _modes[16] = {INPUT};
  bool _values[16] = {false};
  bool _hasOutputs = false;
  bool _hasInputs = false;
  bool _invIn = false;
  bool _invOut = false;
};