#include "flprogPCF8575.h"

FLProgPCF7585 lamp1(0x20, 0);

uint32_t timerStartTime;
uint8_t workPin = 0;

void setup()
{
  RT_HW_Base.i2cSetPins(20, 21, 0);
  for (uint8_t i = 0; i < 16; i++)
  {
    lamp1.pinMode(i, OUTPUT);
  }
  timerStartTime = millis();
}

void loop()
{
  lamp1.pool();
  if (flprog::isTimer(timerStartTime, 1000))
  {
    timerStartTime = millis();
    lamp1.write(workPin, 0);
    workPin++;
    if (workPin > 15)
    {
      workPin = 0;
    }
    lamp1.write(workPin, 1);
  }
}