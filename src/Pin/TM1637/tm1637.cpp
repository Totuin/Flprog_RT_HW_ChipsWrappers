#include "tm1637.h"

/**
 * @brief Запоминает линии CLK/DIO и режим четырёхразрядного индикатора.
 */
FLProgTM1637::FLProgTM1637(uint8_t clkPin, uint8_t dioPin, uint8_t mode)
{
  _clkPin = clkPin;
  _dioPin = dioPin;
  _mode = mode;
}

/**
 * @brief При первом вызове настраивает драйвер и выводит требуемое представление данных в одном из четырёх режимов.
 */
void FLProgTM1637::workPool()
{
  if (!isReady())
  {
    _sensor.setParam(_clkPin, _dioPin);
    if (isReady())
    {
      _sensor.setBrightON();
      for (uint8_t i = 0; i < 4; i++)
      {
        _sensor.setDot(i, _points[i]);
      }
    }
    else
    {
      return;
    }
  }
  if (_task.run())
  {
    if (_mode == FLPROG_TM1637_RIGHT_NUMBER_MODE)
    {
      _sensor.displayNumberRight(_numberValue);
      return;
    }
    if (_mode == FLPROG_TM1637_ADD_ZERO_MODE)
    {
      _sensor.displayNumberZero(_numberValue);
      return;
    }
    if (_mode == FLPROG_TM1637_TIME_MODE)
    {
      _sensor.displayTime(_timeValue[0], _timeValue[1]);
      return;
    }
    if (_mode == FLPROG_TM1637_TEXT_MODE)
    {
      _sensor.displayText(_textValue);
      return;
    }
  }
}

/**
 * @brief Ограничивает яркость диапазоном 0..7; повторная установка того же уровня не создаёт обмен.
 */
void FLProgTM1637::setBrightness(uint8_t brightness)
{
  if (brightness > 7)
  {
    brightness = 7;
  }
  if (_sensor.bright == brightness)
  {
    return;
  }
  _sensor.bright = brightness;
  if (isReady())
  {
    _sensor.setBrightON();
  }
}

/**
 * @brief Настраивает дополнительную точку в позиции 0..3 и игнорирует некорректный индекс.
 */
void FLProgTM1637::setPoint(uint8_t pos, bool state)
{
  if (pos > 3)
  {
    return;
  }
  if (_points[pos] == state)
  {
    return;
  }
  _points[pos] = state;
  if (isReady())
  {
    _sensor.setDot(pos, state);
  }
}

/**
 * @brief Проверяет режим и допустимый диапазон числа, затем запрашивает перерисовку.
 */
void FLProgTM1637::setNumber(int16_t number)
{
  if ((_mode != FLPROG_TM1637_RIGHT_NUMBER_MODE) && (_mode != FLPROG_TM1637_ADD_ZERO_MODE))
  {
    return;
  }
  if (number < -999 || number > 9999)
  {
    return;
  }
  if (_numberValue == number)
  {
    return;
  }
  _numberValue = number;
  _task.run(5);
}

/**
 * @brief Принимает время в диапазонах 0..23 и 0..59, обновляет буфер и инициирует перерисовку.
 */
void FLProgTM1637::setTime(uint8_t hours, uint8_t minutes)
{
  if (_mode != FLPROG_TM1637_TIME_MODE)
  {
    return;
  }

  if ((hours > 23) || (minutes > 59))
  {
    return;
  }
  if ((_timeValue[0] == hours) && (_timeValue[1] == minutes))
  {
    return;
  }
  _timeValue[0] = hours;
  _timeValue[1] = minutes;
  _task.run(5);
}

/**
 * @brief Формирует ровно четыре символа с выравниванием вправо и завершающим нулём.
 */
void FLProgTM1637::setText(String text)
{
  if (_mode != FLPROG_TM1637_TEXT_MODE)
  {
    return;
  }
  if (_textValueString.equals(text))
  {
    return;
  }
  _textValueString = text;
  for (int16_t i = 0; i < 4; i++)
  {
    _textValue[i] = ' ';
  }
  _textValue[4] = '\0';
  uint16_t len = _textValueString.length();
  uint16_t copyLen = (len < 4) ? len : 4;
  // toCharArray(..., copyLen) reserves one byte for '\0' and
  // would discard the final character. Copy only the visible positions.
  for (uint8_t i = 0; i < copyLen; i++)
  {
    _textValue[4 - copyLen + i] = _textValueString[i];
  }
  _task.run(5);
}
