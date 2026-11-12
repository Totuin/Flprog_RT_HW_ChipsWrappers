#include "tca8418.h"

/**
 * @brief Сохраняет параметры подключения TCA8418; старые параметры rows/columns сохранены только для совместимости API.
 */
FLProgTca8418::FLProgTca8418(uint8_t address, uint8_t bus, uint8_t rows, uint8_t columns)
{
  (void)rows;    //  not used, left for compatibility
  (void)columns; //  not used, left for compatibility
  RT_HW_Base.i2cSetParam(_device, address, bus, RT_HW_I2C_SPEED);
  _status = FLPROG_NOT_REDY_STATUS;
}

/**
 * @brief Сохраняет параметры подключения TCA8418; старые параметры rows/columns сохранены только для совместимости API.
 */
FLProgTca8418::FLProgTca8418(uint8_t address, uint8_t bus, uint32_t speed)
{
  RT_HW_Base.i2cSetParam(_device, address, bus, speed);
  _status = FLPROG_NOT_REDY_STATUS;
}

/**
 * @brief Сохраняет параметры подключения TCA8418; старые параметры rows/columns сохранены только для совместимости API.
 */
FLProgTca8418::FLProgTca8418(uint8_t address, uint8_t bus, uint8_t rows, uint8_t columns, uint8_t expander, uint8_t channel)
{
  (void)rows;    //  not used, left for compatibility
  (void)columns; //  not used, left for compatibility
  RT_HW_Base.i2cSetParam(_device, address, bus, RT_HW_I2C_SPEED, expander, channel);
  _status = FLPROG_NOT_REDY_STATUS;
}

/**
 * @brief Сохраняет параметры подключения TCA8418; старые параметры rows/columns сохранены только для совместимости API.
 */
FLProgTca8418::FLProgTca8418(uint8_t address, uint8_t bus, uint32_t speed, uint8_t expander, uint8_t channel)
{
  RT_HW_Base.i2cSetParam(_device, address, bus, speed, expander, channel);
  _status = FLPROG_NOT_REDY_STATUS;
}

/**
 * @brief Включает режим обработки аппаратных прерываний и помечает конфигурацию для переинициализации.
 */
void FLProgTca8418::setInterruptMode()
{
  if (_isInterruptMode)
  {
    return;
  }
  _isInterruptMode = true;
  if (_status == FLPROG_NOT_REDY_STATUS)
  {
    _status = FLPROG_WAIT_I2C_DEVICE_INIT;
  }
}
/**
 * @brief Возвращает режим опроса клавиатуры; историческое имя метода сохранено для совместимости.
 */
void FLProgTca8418::resetIntrruptMode()
{
  if (!_isInterruptMode)
  {
    return;
  }
  _isInterruptMode = false;
  if (_status == FLPROG_NOT_REDY_STATUS)
  {
    _status = FLPROG_WAIT_I2C_DEVICE_INIT;
  }
}

/**
 * @brief Читает очередное аппаратное событие нажатия или отпускания из FIFO.
 */
uint8_t FLProgTca8418::getEvent()
{
  uint8_t event = readRegister(FLPROG_TCA8418_REG_KEY_EVENT_A);
  return event;
}

/**
 * @brief Освобождает очередь событий и сбрасывает прерывания; ограничивает число чтений размером FIFO.
 */
uint8_t FLProgTca8418::flush()
{
  uint8_t count = 0;
  // The hardware FIFO holds 10 events. Bound the loop to avoid a
  // permanent hang if an I2C error prevents the FIFO from advancing.
  while ((count < 10) && (getEvent() != 0))
  {
    count++;
  }
  readRegister(FLPROG_TCA8418_REG_GPIO_INT_STAT_1);
  readRegister(FLPROG_TCA8418_REG_GPIO_INT_STAT_2);
  readRegister(FLPROG_TCA8418_REG_GPIO_INT_STAT_3);
  //  clear INT_STAT register
  writeRegister(FLPROG_TCA8418_REG_INT_STAT, 3);
  return count;
}

/**
 * @brief Настраивает GPIO, линии матрицы и разрешённые прерывания перед началом обработки событий.
 */
void FLProgTca8418::init()
{
  RT_HW_Base.i2cInitDevice(_device);
  //  GPIO
  //  set default all GIO pins to INPUT
  writeRegister(FLPROG_TCA8418_REG_GPIO_DIR_1, 0x00);
  writeRegister(FLPROG_TCA8418_REG_GPIO_DIR_2, 0x00);
  writeRegister(FLPROG_TCA8418_REG_GPIO_DIR_3, 0x00);

  //  add all pins to key events
  writeRegister(FLPROG_TCA8418_REG_GPI_EM_1, 0xFF);
  writeRegister(FLPROG_TCA8418_REG_GPI_EM_2, 0xFF);
  writeRegister(FLPROG_TCA8418_REG_GPI_EM_3, 0xFF);

  //  set all pins to FALLING interrupts
  writeRegister(FLPROG_TCA8418_REG_GPIO_INT_LVL_1, 0x00);
  writeRegister(FLPROG_TCA8418_REG_GPIO_INT_LVL_2, 0x00);
  writeRegister(FLPROG_TCA8418_REG_GPIO_INT_LVL_3, 0x00);

  //  add all pins to interrupts
  writeRegister(FLPROG_TCA8418_REG_GPIO_INT_EN_1, 0xFF);
  writeRegister(FLPROG_TCA8418_REG_GPIO_INT_EN_2, 0xFF);
  writeRegister(FLPROG_TCA8418_REG_GPIO_INT_EN_3, 0xFF);
  uint8_t mask = 0x00;
  for (int r = 0; r < 8; r++)
  {
    mask <<= 1;
    mask |= 1;
  }
  writeRegister(FLPROG_TCA8418_REG_KP_GPIO_1, mask);
  mask = 0x00;
  for (int c = 0; c < 10 && c < 8; c++)
  {
    mask <<= 1;
    mask |= 1;
  }
  writeRegister(FLPROG_TCA8418_REG_KP_GPIO_2, mask);
  writeRegister(FLPROG_TCA8418_REG_KP_GPIO_3, 0x03);
  flush();
  if (_isInterruptMode)
  {
    enableInterrupts();
  }
  else
  {
    disableInterrupts();
  }
  _status = FLPROG_READY_STATUS;
}

/**
 * @brief Возвращает количество ожидающих событий из младших четырёх бит аппаратного счётчика.
 */
uint8_t FLProgTca8418::available()
{
  uint8_t eventCount = readRegister(FLPROG_TCA8418_REG_KEY_LCK_EC);
  eventCount &= 0x0F; //  lower 4 bits only
  return eventCount;
}

/**
 * @brief В плановом цикле получает одно событие, если очередь не пуста и задача разрешена.
 */
void FLProgTca8418::workPool()
{
  uint8_t taskRun = _task.run();
  if (!taskRun)
  {
    return;
  }
  if (available() == 0)
  {
    return;
  }
  privateReadData();
}

/**
 * @brief В обработке прерывания забирает не больше десяти событий за вызов.
 */
void FLProgTca8418::interruptReadData()
{
  // Read at most one FIFO's worth per invocation, even on bus faults.
  for (uint8_t i = 0; (i < 10) && available(); i++)
  {
    privateReadData();
  }
}

/**
 * @brief Декодирует код 1..80 как позицию в матрице 8x10; повторные переходы не меняют счётчик нажатий.
 */
void FLProgTca8418::privateReadData()
{
  uint16_t k = getEvent();
  bool position = false;
  if (k & 0x80)
  {
    position = true;
  }
  k &= 0x7F;
  // Valid keypad event codes are 1..80; zero is not a key.
  if ((k == 0) || (k > 80))
  {
    return;
  }
  k--;
  uint8_t row = k / 10;
  uint8_t col = k % 10;
  // Duplicate presses/releases should not alter the pressed-key count.
  if (_buttons[row][col] == position)
  {
    return;
  }
  _buttons[row][col] = position;
  if (position)
  {
    _pressetButtonsCount++;
    _pressetButtonCol = col;
    _pressetButtonRow = row;
  }
  else
  {
    if (_pressetButtonsCount > 0)
    {
      _pressetButtonsCount--;
    }
    if (_pressetButtonsCount == 0)
    {
      _pressetButtonCol = 255;
      _pressetButtonRow = 255;
    }
  }
}

/**
 * @brief Возвращает кэшированное состояние кнопки; вне матрицы 8x10 возвращает false.
 */
bool FLProgTca8418::buttonState(uint8_t row, uint8_t col)
{
  if (row > 7)
  {
    return false;
  }
  if (col > 9)
  {
    return false;
  }
  return _buttons[row][col];
}

/**
 * @brief Разрешает формирование прерываний клавиатуры, сохраняя прочие биты CFG.
 */
void FLProgTca8418::enableInterrupts()
{
  uint8_t value = readRegister(FLPROG_TCA8418_REG_CFG);
  value |= (FLPROG_TCA8418_REG_CFG_GPI_IEN | FLPROG_TCA8418_REG_CFG_KE_IEN);
  writeRegister(FLPROG_TCA8418_REG_CFG, value);
}

/**
 * @brief Отключает прерывания клавиатуры, сохраняя прочие биты CFG.
 */
void FLProgTca8418::disableInterrupts()
{
  uint8_t value = readRegister(FLPROG_TCA8418_REG_CFG);
  value &= ~(FLPROG_TCA8418_REG_CFG_GPI_IEN | FLPROG_TCA8418_REG_CFG_KE_IEN);
  writeRegister(FLPROG_TCA8418_REG_CFG, value);
}