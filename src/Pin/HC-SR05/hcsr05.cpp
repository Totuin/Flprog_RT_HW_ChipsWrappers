#include "hcsr05.h"

/**
 * @brief Запоминает пины подключения датчика для последующей инициализации драйвера.
 */
FLProgHC_SR05::FLProgHC_SR05(uint8_t trigPin, uint8_t echoPin)
{
  _pinTrg = trigPin;
  _pinEcho = echoPin;
}

/**
 * @brief Запоминает пины подключения датчика для последующей инициализации драйвера.
 */
FLProgHC_SR05::FLProgHC_SR05(uint8_t trigPin)
{
  _pinTrg = trigPin;
}

/**
 * @brief Выполняет шаг периодического опроса датчика через интерфейс RT_HW.
 */
void FLProgHC_SR05::workPool()
{
  if (_sensor.custom == 0)
  {
    _sensor.setTrig(_pinTrg);
    _sensor.setEcho(_pinEcho);
    _sensor.custom = 1;
  }
  uint8_t taskRun = _task.run();
  _sensor.direct(taskRun);
  _task.run(taskRun);
  _sensor.fresh = 0;
}