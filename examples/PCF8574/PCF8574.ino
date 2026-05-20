#include "flprogPCF7584.h"

FLProgPCF7584 lamp1(0x20, 0);

uint32_t timerStartTime;
uint8_t workPin = 0;

void setup()
{
  RT_HW_Base.i2cSetPins(20, 21, 0);
  for (uint8_t i = 0; i < 8; i++)
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
    if (workPin > 7)
    {
      workPin = 0;
    }
    lamp1.write(workPin, 1);
  }
}