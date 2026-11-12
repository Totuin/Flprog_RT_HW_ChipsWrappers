#include "bmp180.h"

/**
 * @brief Настраивает адрес, номер I2C-шины и скорость, включая необязательный канал расширителя.
 */
FLProgBMP180::FLProgBMP180(uint8_t address, uint8_t bus, uint32_t speed)
{
  RT_HW_Base.i2cSetParam(_sensor.dvi, address, bus, speed);
}

/**
 * @brief Настраивает адрес, номер I2C-шины и скорость, включая необязательный канал расширителя.
 */
FLProgBMP180::FLProgBMP180(uint8_t address, uint8_t bus, uint32_t speed, uint8_t expander, uint8_t channel)
{
  RT_HW_Base.i2cSetParam(_sensor.dvi, address, bus, speed, expander, channel);
}

/**
 * @brief Передаёт сигнал выполнения запланированной задачи в драйвер RT_HW и возвращает планировщику результат шага.
 */
void FLProgBMP180::workPool()
{
  uint8_t taskRun = _task.run();
  _sensor.direct(taskRun);
  _task.run(taskRun);
  _sensor.fresh = 0;
}