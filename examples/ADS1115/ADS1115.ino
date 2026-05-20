#include "flprogADS1115.h"

FLProgADS1115 sensor(0x48, 0);
int16_t value;
float voltValue;
float milliVoltValue;

void setup()
{
  RT_HW_Base.i2cSetPins(20, 21, 0);
  sensor.setMeasurementChannel(FLPROG_ADS1115_A0_GND_MEASUREMENT_CHANNEL); // Установка канала измерения для ADS1115. Канал A0-GND
  sensor.setMeasurementScale(FLPROG_ADS1115_4_V_MEASUREMENT_SCALE);        // Установка шкалы измерения для ADS1115. 4.096 Вольта
  sensor.setMeasurementRate(FLPROG_ADS1115_16_MEASUREMENT_RARE);           // Установка частоты измерений для ADS1115. 16 измерений в секунду
}

void loop()
{
  sensor.pool();
  value = sensor.getChanelSysValue(FLPROG_ADS1115_A0_GND_MEASUREMENT_CHANNEL);                // Получение измеренного значения с канала ADS1115 в еденицах системы. Канал A0-GND
  voltValue = sensor.getChanelVoltValue(FLPROG_ADS1115_A0_GND_MEASUREMENT_CHANNEL);           // Получение измеренного значения с канала ADS1115 в Вольтах. Канал A0-GND
  milliVoltValue = sensor.getChanelMilliVoltValue(FLPROG_ADS1115_A0_GND_MEASUREMENT_CHANNEL); // Получение измеренного значения с канала ADS1115 в Милливольтах. Канал A0-GND
}