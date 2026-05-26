#pragma once
#include "flprogUtilites.h"
#include "RT_HW_pin_SR05.hpp"

class FLProgHC_SR05 : public AbstractTaskDevice
{
public:
  FLProgHC_SR05(uint8_t trigPin, uint8_t echoPin); // -- Конструктор с указанием пинов триггера и эхо
  FLProgHC_SR05(uint8_t trigPin);                  // -- Конструктор с указанием пинов триггера

  int16_t getMcs() { return _sensor.LenMcs; };                             // -- Получение времени задержки эха в микросекундах
  int16_t getMm() { return _sensor.getLenMm(); };                          // -- Получение расстояния в миллиметрах
  int16_t getCm() { return _sensor.getLenCm(); };                          // -- Получение расстояния в сантиметрах
  void setMaxLen(uint16_t maxLen) { _sensor.setMaxLen(maxLen); };          // -- Установка максимального измеряемого расстояния
  void setIntTempAir(int16_t value) { _sensor.setTempAir(value); };        // -- Установка  поправки на температуру воздуха типа int16_t
  void setFloatTempAir(float value) { _sensor.setTempAir(value); };        // -- Установка  поправки на температуру воздуха типа float
  void setTrig(uint8_t pin) { _sensor.setTrig(pin); };                     // -- Задание пина Trg
  void setEcho(uint8_t pin) { _sensor.setEcho(pin); };                     // -- Задание пина Echo
  bool pinIdStatus() { return RT_HW_Base.pinDigitalRead(_sensor.idPin); }; // -- Получение статуса рабочего пина
  uint8_t getState() { return _sensor.state; };                            // -- Получение текущего шоага исполнения алгоритма
  void setState(uint8_t state) { _sensor.state = state; };                 // -- Установка текущего шага исполнения алгоритма
  void timeBegin(uint32_t time) { _sensor.timeBegin = time; };             // -- Установка времени начала измерения
  void timeEnd(uint32_t time) { _sensor.timeEnd = time; };                 // -- Установка времени окончания измерения
  int8_t numIRQ() { return _sensor.numIRQ; };                              // -- Получение номера прерывания

protected:
  void
  init() { _status = FLPROG_READY_STATUS; };
  void workPool();

  RT_HW_SR05 _sensor;
};
