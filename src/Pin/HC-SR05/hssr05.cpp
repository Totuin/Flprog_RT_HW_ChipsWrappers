#include "hcsr05.h"

FLProgHC_SR05::FLProgHC_SR05(uint8_t trigPin, uint8_t echoPin)
{
  _sensor.setTrig(trigPin);
  _sensor.setEcho(echoPin);
}

FLProgHC_SR05::FLProgHC_SR05(uint8_t trigPin)
{
  _sensor.setTrig(trigPin);
}

void FLProgHC_SR05::workPool()
{
  uint8_t taskRun = _task.run();
  _sensor.direct(taskRun);
  _task.run(taskRun);
  _sensor.fresh = 0;
}