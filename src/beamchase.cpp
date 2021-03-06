// This file is the equivalent of a signed distance field in raymarching
#include <Arduino.h>
#include <cstdint>
#include <vector>
#include "trig.h"
#include "framebuffer.h"

const int square8_16_16[16][16] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

std::vector<int32_t> matrixRotate(int32_t x, int32_t y, int degrees)
{
  int32_t x2 = xcos(x, degrees) - xsin(y, degrees);
  int32_t y2 = xsin(x, degrees) + xcos(y, degrees);

  return std::vector<int32_t>{x2, y2};
}

void beamChase(Framebuffer_16_16 &framebuffer_16_16, uint32_t tick)
{
  // draw framebuffer
  for (uint32_t row = 0; row < 16; row++)
  {
    for (uint32_t column = 0; column < 16; column++)
    {
      // rotate the coordinates
      std::vector<int32_t> newCoords = matrixRotate(row, column, tick);
      if (
          newCoords[0] >= 0 && newCoords[0] < 16 && newCoords[1] >= 0 && newCoords[1] < 16)
      {
        // Serial.print(newCoords[0]);
        // Serial.print(",");
        // Serial.print(newCoords[1]);
        // Serial.println();
        // Serial.println(125 * square8_16_16[row][column]);
        framebuffer_16_16.setIntensity(newCoords[0], newCoords[1], 125 * square8_16_16[row][column]);
      }
    }
  }
}