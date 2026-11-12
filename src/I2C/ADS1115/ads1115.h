#pragma once
#include "flprogUtilites.h"
#include "RT_HW_i2c_ADS1115.hpp"

#define FLPROG_ADS1115_6_V_MEASUREMENT_SCALE 0    //  -- Установка шкалы измерения для ADS1115. 6.144 Вольт
#define FLPROG_ADS1115_4_V_MEASUREMENT_SCALE 1    //  -- Установка шкалы измерения для ADS1115. 4.096 Вольта
#define FLPROG_ADS1115_2_V_MEASUREMENT_SCALE 2    //  -- Установка шкалы измерения для ADS1115. 2.048 Вольта
#define FLPROG_ADS1115_1_V_MEASUREMENT_SCALE 3    //  -- Установка шкалы измерения для ADS1115. 1.024 Вольт
#define FLPROG_ADS1115_0_5_V_MEASUREMENT_SCALE 4  //  -- Установка шкалы измерения для ADS1115. 0.512 Вольта
#define FLPROG_ADS1115_0_25_V_MEASUREMENT_SCALE 5 //  -- Установка шкалы измерения для ADS1115. 0.256 Вольта

#define FLPROG_ADS1115_8_MEASUREMENT_RARE 0   //  -- Установка частоты измерений для ADS1115. 8 измерений в секунду
#define FLPROG_ADS1115_16_MEASUREMENT_RARE 1  //  -- Установка частоты измерений для ADS1115. 16 измерений в секунду
#define FLPROG_ADS1115_32_MEASUREMENT_RARE 2  //  -- Установка частоты измерений для ADS1115. 32 измерения в секунду
#define FLPROG_ADS1115_64_MEASUREMENT_RARE 3  //  -- Установка частоты измерений для ADS1115. 64 измерения в секунду
#define FLPROG_ADS1115_128_MEASUREMENT_RARE 4 //  -- Установка частоты измерений для ADS1115. 128 измерений в секунду
#define FLPROG_ADS1115_250_MEASUREMENT_RARE 5 //  -- Установка частоты измерений для ADS1115. 250  измерений в секунду
#define FLPROG_ADS1115_475_MEASUREMENT_RARE 6 //  -- Установка частоты измерений для ADS1115. 475 измерений в секунду
#define FLPROG_ADS1115_860_MEASUREMENT_RARE 7 //  -- Установка частоты измерений для ADS1115. 860 измерений в секунду

#define FLPROG_ADS1115_A0_A1_MEASUREMENT_CHANNEL 0  //  -- Установка канала измерения для ADS1115. Канал A0-A1
#define FLPROG_ADS1115_A0_A3_MEASUREMENT_CHANNEL 1  //  -- Установка канала измерения для ADS1115. Канал A0-A3
#define FLPROG_ADS1115_A1_A3_MEASUREMENT_CHANNEL 2  // --  Установка канала измерения для ADS1115. Канал A1-A3
#define FLPROG_ADS1115_A2_A3_MEASUREMENT_CHANNEL 3  //  -- Установка канала измерения для ADS1115. Канал A2-A3
#define FLPROG_ADS1115_A0_GND_MEASUREMENT_CHANNEL 4 //  -- Установка канала измерения для ADS1115. Канал A0-GND
#define FLPROG_ADS1115_A1_GND_MEASUREMENT_CHANNEL 5 //  -- Установка канала измерения для ADS1115. Канал A1-GND
#define FLPROG_ADS1115_A2_GND_MEASUREMENT_CHANNEL 6 //  -- Установка канала измерения для ADS1115. Канал A2-GND
#define FLPROG_ADS1115_A3_GND_MEASUREMENT_CHANNEL 7 //  -- Установка канала измерения для ADS1115. Канал A3-GND

/**
 * @brief АЦП ADS1115: настройка диапазона и частоты измерений, выбор каналов и чтение результатов.
 * @details Измерения выполняются драйвером RT_HW во время периодического вызова pool().
 */
class FLProgADS1115 : public AbstractTaskDevice
{
public:
  FLProgADS1115(uint8_t address, uint8_t bus, uint32_t speed = RT_HW_I2C_SPEED);                  // -- Конструктор с указанием адреса, шины и скорости I2C
  FLProgADS1115(uint8_t address, uint8_t bus, uint32_t speed, uint8_t expander, uint8_t channel); // -- Конструктор с указанием адреса, шины, скорости I2C, а также параметров расширителя и канала

  void setMeasurementScale(uint8_t scale) { _sensor.codePGA = scale; }; // Выбор диапазона ADS1115: используйте FLPROG_ADS1115_*_MEASUREMENT_SCALE.
  void setMeasurementRate(uint8_t rate) { _sensor.codeSPS = rate; }; // Частота преобразования: значения FLPROG_ADS1115_*_MEASUREMENT_RARE (историческое имя RARE).
  void setMeasurementChannel(uint8_t channel) { _sensor.setPermit(channel); }; // Разрешает выбранный канал АЦП; используются FLPROG_ADS1115_*_MEASUREMENT_CHANNEL.

  // Индексы каналов ADS1115 кодируются значениями 0..7.
  // Вне этого диапазона возвращаем 0, а не читаем за границей meterVar.
  int16_t getChanelADSValue(uint8_t channel) { return (channel < 8) ? _sensor.meterVar[channel] : 0; }; // Сырое значение канала 0..7; для неверного индекса возвращается 0.
  int16_t getChanelSysValue(uint8_t channel) { return _sensor.getVar_sys(getChanelADSValue(channel)); }; // Значение измерения в единицах RT_HW.
  float getChanelVoltValue(uint8_t channel) { return _sensor.getVar_V(getChanelADSValue(channel)); }; // Результат измерения в вольтах.
  float getChanelMilliVoltValue(uint8_t channel) { return _sensor.getVar_mV(getChanelADSValue(channel)); }; // Результат измерения в милливольтах.

  bool getOk() { return _sensor.ok > 0; };

protected:
  void init()
  {
    _status = FLPROG_READY_STATUS;
  };
  void workPool();

  RT_HW_i2c_ADS1115 _sensor;
};