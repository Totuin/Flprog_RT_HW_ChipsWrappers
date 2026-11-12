#include "pcf8575.h"

/**
 * @brief Настраивает I2C-подключение и устанавливает исходные режимы линий расширителя.
 */
FLProgPCF8575::FLProgPCF8575(uint8_t address, uint8_t bus, uint32_t speed)
{
  RT_HW_Base.i2cSetParam(_sensor.dvi, address, bus, speed);
  _sensor.flagInvIn = 0;
  _sensor.flagInvOut = 0;
  _sensor.cnfgA = 0xFF;
  _sensor.cnfgB = 0xFF;
  _sensor.custom = 1;
}

/**
 * @brief Настраивает I2C-подключение и устанавливает исходные режимы линий расширителя.
 */
FLProgPCF8575::FLProgPCF8575(uint8_t address, uint8_t bus, uint32_t speed, uint8_t expander, uint8_t channel)
{
  RT_HW_Base.i2cSetParam(_sensor.dvi, address, bus, speed, expander, channel);
  _sensor.flagInvIn = 0;
  _sensor.flagInvOut = 0;
  _sensor.cnfgA = 0xFF;
  _sensor.cnfgB = 0xFF;
  _sensor.custom = 1;
}

/**
 * @brief Синхронизирует регистровую модель расширителя с аппаратным драйвером RT_HW.
 */
void FLProgPCF8575::workPool()
{
  uint8_t taskRun = _task.run();
  _sensor.direct(taskRun);
  _task.run(taskRun);
  _sensor.fresh = 0;
}

/**
 * @brief Устанавливает направление выбранной линии, проверяя допустимость номера пина и режима.
 */
void FLProgPCF8575::pinMode(uint8_t pin, uint8_t mode)
{
  if (pin > 15)
  {
    return;
  }
  if ((mode != INPUT) && (mode != OUTPUT))
  {
    return;
  }
  bool value = mode == INPUT;
  if (pin < 8)
  {
    bitWrite(_sensor.cnfgA, pin, value);
    return;
  }
  uint8_t pinB = pin - 8;
  bitWrite(_sensor.cnfgB, pinB, value);
}

/**
 * @brief Записывает логический уровень в буфер соответствующего порта расширителя.
 */
void FLProgPCF8575::write(uint8_t pin, bool value)
{
  if (pin > 15)
  {
    return;
  }
  if (pin < 8)
  {
    bitWrite(_sensor.extA, pin, value);
    return;
  }
  uint8_t pinB = pin - 8;
  bitWrite(_sensor.extB, pinB, value);
}

/**
 * @brief Читает кэшированное состояние указанной линии, не обращаясь напрямую к шине.
 */
bool FLProgPCF8575::read(uint8_t pin)
{
  if (pin > 15)
  {
    return false;
  }
  if (pin < 8)
  {
    return bitRead(_sensor.inA, pin);
  }
  uint8_t pinB = pin - 8;
  return bitRead(_sensor.inB, pinB);
}