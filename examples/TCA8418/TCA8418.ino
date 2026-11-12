#include "flprogTca8418.h"

FLProgTca8418 keyboad(0x34, 0, 8, 10);
bool buttons[8][10] = {false};
bool state = 0;

void setup()
{
  RT_HW_Base.i2cSetPins(4, 5, 0); // SDA, SCL и номер шины задаются для используемой платы.
  Serial.begin(115200);
}

void loop()
{
  keyboad.pool();
  // Полная матрица TCA8418: 8 строк (0..7), 10 столбцов (0..9).
  for (uint8_t r = 0; r < 8; r++)
  {
    for (uint8_t c = 0; c < 10; c++)
    {
      state = keyboad.buttonState(r, c);
      if (state != buttons[r][c])
      {
        buttons[r][c] = state;
        Serial.print("ROW:");
        Serial.print(r);
        Serial.print(" COL:");
        Serial.print(c);
        Serial.print(" State:");
        if (state)
        {
          Serial.print("ON");
        }
        else
        {
          Serial.print("OFF");
        }
        Serial.println();
      }
    }
  }
}
