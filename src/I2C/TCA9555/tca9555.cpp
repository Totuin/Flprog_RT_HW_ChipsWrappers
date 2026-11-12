#include "flprogTca9555.h"

/**
 * @brief Настраивает адрес, шину и скорость I2C; варианты конструктора поддерживают маршрутизацию через расширитель.
 */
FLProgTca9555::FLProgTca9555(uint8_t address, uint8_t bus)
{
  RT_HW_Base.i2cSetParam(_device, address, bus, 400000UL);
}

/**
 * @brief Настраивает адрес, шину и скорость I2C; варианты конструктора поддерживают маршрутизацию через расширитель.
 */
FLProgTca9555::FLProgTca9555(uint8_t address, uint8_t bus, uint32_t speed)
{
  RT_HW_Base.i2cSetParam(_device, address, bus, speed);
}

/**
 * @brief Настраивает адрес, шину и скорость I2C; варианты конструктора поддерживают маршрутизацию через расширитель.
 */
FLProgTca9555::FLProgTca9555(uint8_t address, uint8_t bus, uint32_t speed, uint8_t expander, uint8_t channel)
{
  RT_HW_Base.i2cSetParam(_device, address, bus, speed, expander, channel);
}

/**
 * @brief Настраивает адрес, шину и скорость I2C; варианты конструктора поддерживают маршрутизацию через расширитель.
 */
FLProgTca9555::FLProgTca9555(uint8_t address, uint8_t bus, uint8_t expander, uint8_t channel)
{
  RT_HW_Base.i2cSetParam(_device, address, bus, 400000UL, expander, channel);
}

/**
 * @brief Применяет режимы 16 пинов и запоминает, какие направления требуют опроса или передачи.
 */
void FLProgTca9555::init()
{
  _hasOutputs = false;
  _hasInputs = false;
  for (uint8_t i = 0; i < 16; i++)
  {
    privatePinMode(i, _modes[i]);
    if (_modes[i] == OUTPUT)
    {
      _hasOutputs = true;
    }
    if (_modes[i] == INPUT)
    {
      _hasInputs = true;
    }
  }
  _status = FLPROG_READY_STATUS;
}

/**
 * @brief Изменение направления пина вызывает повторную инициализацию устройства.
 */
void FLProgTca9555::pinMode(uint8_t pin, uint8_t mode)
{
  if (pin > 15)
  {
    return;
  }
  if ((mode != INPUT) && (mode != OUTPUT))
  {
    return;
  }
  _modes[pin] = mode;
  if (_status == FLPROG_READY_STATUS)
  {
    _status = FLPROG_WAIT_I2C_DEVICE_INIT;
  }
}

/**
 * @brief Меняет один бит аппаратного регистра конфигурации, сохраняя режим остальных линий.
 */
void FLProgTca9555::privatePinMode(uint8_t pin, uint8_t mode)
{
  uint8_t configReg = FLPROG_TCA9555_CONFIGURATION_PORT_0;
  if (pin > 7)
  {
    configReg = FLPROG_TCA9555_CONFIGURATION_PORT_1;
    pin -= 8;
  }
  uint8_t val = readRegister(configReg);
  uint8_t prevVal = val;
  uint8_t mask = 1 << pin;
  if (mode == INPUT)
    val |= mask;
  else
    val &= ~mask;
  if (val != prevVal)
  {
    writeRegister(configReg, val);
  }
}

/**
 * @brief Обновляет кэш выхода с учётом инверсии и помечает значения для отправки на устройство.
 */
void FLProgTca9555::write(uint8_t pin, bool value)
{
  if (pin > 15)
  {
    return;
  }
  bool temp = value;
  if (_invOut)
  {
    temp = !temp;
  }
  if (_values[pin] == temp)
  {
    return;
  }
  _task.reset();
  _values[pin] = temp;
  // workPool() clears this flag after writing the output registers.
  // Arm the next write when an output value actually changes.
  if (_modes[pin] == OUTPUT)
  {
    _hasOutputs = true;
  }
}

/**
 * @brief Возвращает последнее кэшированное значение входа/выхода с учётом настройки инверсии.
 */
bool FLProgTca9555::read(uint8_t pin)
{
  if (pin > 15)
  {
    return false;
  }
  bool temp = _values[pin];
  if (_invIn)
  {
    temp = !temp;
  }
  return temp;
}

/**
 * @brief Периодически опрашивает входы и отправляет изменённые выходы в регистры обоих портов.
 */
void FLProgTca9555::workPool()
{
  uint8_t taskRun = _task.run();
  if (!taskRun)
  {
    return;
  }
  if (_hasInputs)
  {
    uint8_t inRreg = readRegister(FLPROG_TCA9555_INPUT_PORT_REGISTER_0);
    uint8_t inRreg1 = readRegister(FLPROG_TCA9555_INPUT_PORT_REGISTER_1);
    for (uint8_t i = 0; i < 16; i++)
    {
      uint8_t inPin = i;
      if (inPin > 7)
      {
        inPin -= 8;
      }
      if (_modes[i] == INPUT)
      {
        uint8_t mask = 1 << inPin;
        if (i > 7)
        {
          _values[i] = (inRreg1 & mask) != 0;
        }
        else
        {
          _values[i] = (inRreg & mask) != 0;
        }
      }
    }
  }
  if (_hasOutputs)
  {
    // Флаг сбрасываем после формирования задания. write() снова установит его
    // при следующем изменении выходного значения.
    _hasOutputs = false;
    uint8_t reg0 = readRegister(FLPROG_TCA9555_OUTPUT_PORT_REGISTER_0);
    uint8_t reg1 = readRegister(FLPROG_TCA9555_OUTPUT_PORT_REGISTER_1);
    for (uint8_t i = 0; i < 16; i++)
    {
      if (_modes[i] == OUTPUT)
      {
        uint8_t pin = i;
        if (pin > 7)
        {
          pin -= 8;
        }
        uint8_t mask = 1 << pin;

        if (_values[i])
        {
          if (i > 7)
          {
            reg1 |= mask;
          }
          else
          {
            reg0 |= mask;
          }
        }
        else
        {
          if (i > 7)
          {
            reg1 &= ~mask;
          }
          else
          {
            reg0 &= ~mask;
          }
        }
      }
    }
    writeRegister(FLPROG_TCA9555_OUTPUT_PORT_REGISTER_0, reg0);
    writeRegister(FLPROG_TCA9555_OUTPUT_PORT_REGISTER_1, reg1);
  }
}