#include "tca8418.h"

FLProgTca8418::FLProgTca8418(uint8_t address, uint8_t bus, uint8_t rows, uint8_t columns)
{
  (void)rows;    //  not used, left for compatibility
  (void)columns; //  not used, left for compatibility
  RT_HW_Base.i2cSetParam(_device, address, bus, RT_HW_I2C_SPEED);
  _status = FLPROG_NOT_REDY_STATUS;
}

FLProgTca8418::FLProgTca8418(uint8_t address, uint8_t bus, uint32_t speed)
{
  RT_HW_Base.i2cSetParam(_device, address, bus, speed);
  _status = FLPROG_NOT_REDY_STATUS;
}

FLProgTca8418::FLProgTca8418(uint8_t address, uint8_t bus, uint8_t rows, uint8_t columns, uint8_t expander, uint8_t channel)
{
  (void)rows;    //  not used, left for compatibility
  (void)columns; //  not used, left for compatibility
  RT_HW_Base.i2cSetParam(_device, address, bus, RT_HW_I2C_SPEED, expander, channel);
  _status = FLPROG_NOT_REDY_STATUS;
}

FLProgTca8418::FLProgTca8418(uint8_t address, uint8_t bus, uint32_t speed, uint8_t expander, uint8_t channel)
{
  RT_HW_Base.i2cSetParam(_device, address, bus, speed, expander, channel);
  _status = FLPROG_NOT_REDY_STATUS;
}

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

uint8_t FLProgTca8418::getEvent()
{
  uint8_t event = readRegister(FLPROG_TCA8418_REG_KEY_EVENT_A);
  return event;
}

uint8_t FLProgTca8418::flush()
{
  uint8_t count = 0;
  while (getEvent() != 0)
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

void FLProgTca8418::init()
{

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

uint8_t FLProgTca8418::available()
{
  uint8_t eventCount = readRegister(FLPROG_TCA8418_REG_KEY_LCK_EC);
  eventCount &= 0x0F; //  lower 4 bits only
  return eventCount;
}

void FLProgTca8418::workPool()
{
  if (available() == 0)
  {
    return;
  }
  privateReadData();
}

void FLProgTca8418::interruptReadData()
{
  while (available())
  {
    privateReadData();
  }
}

void FLProgTca8418::privateReadData()
{
  uint16_t k = getEvent();
  bool position = false;
  if (k & 0x80)
  {
    position = true;
  }
  k &= 0x7F;
  k--;
  uint8_t row = k / 10;
  uint8_t col = k % 10;
  if (row > 7)
  {
    return;
  }
  if (col > 9)
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
    _pressetButtonsCount--;
    if (_pressetButtonsCount == 0)
    {
      _pressetButtonCol = 255;
      _pressetButtonRow = 255;
    }
  }
}

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

void FLProgTca8418::enableInterrupts()
{
  uint8_t value = readRegister(FLPROG_TCA8418_REG_CFG);
  value |= (FLPROG_TCA8418_REG_CFG_GPI_IEN | FLPROG_TCA8418_REG_CFG_KE_IEN);
  writeRegister(FLPROG_TCA8418_REG_CFG, value);
}

void FLProgTca8418::disableInterrupts()
{
  uint8_t value = readRegister(FLPROG_TCA8418_REG_CFG);
  value &= ~(FLPROG_TCA8418_REG_CFG_GPI_IEN | FLPROG_TCA8418_REG_CFG_KE_IEN);
  writeRegister(FLPROG_TCA8418_REG_CFG, value);
}