#include <Arduino.h>
#include <cstdint>
#include <vector>

#include "trig.h"

#define elements(x) (sizeof(x) / sizeof((x)[0]))

// row in terms of led matrix pins, positive
uint32_t rows[] = {5, 2, 7, 1, 12, 8, 14, 9};
// column in terms of led matrix pins, ground
uint32_t columns[] = {13, 3, 4, 10, 6, 11, 15, 16};

const uint32_t rowCount = elements(rows);
const uint32_t columnCount = elements(columns);

uint32_t tick = 0;
uint32_t on;
uint32_t off;

void setup()
{
  // "post-process" the pins:
  // pins 1-8 of matrix are at pins 1-8 on teensy
  // pins 9-16 of matrix are at pins 14-21 on teensy
  for (uint32_t i = 0; i < rowCount; i++)
  {
    if (rows[i] >= 9)
    {
      rows[i] += 5;
    }
  }
  for (uint32_t i = 0; i < columnCount; i++)
  {
    if (columns[i] >= 9)
    {
      columns[i] += 5;
    }
  }

  // initialize the "positive volt" pins
  for (uint32_t i = 0; i < rowCount; i++)
  {
    pinMode(rows[i], OUTPUT);
    digitalWrite(rows[i], LOW);
  }
  // initialize the "programmable ground" pins
  for (uint32_t i = 0; i < columnCount; i++)
  {
    pinMode(columns[i], OUTPUT);
    digitalWrite(columns[i], HIGH);
  }
}

const int heart[rowCount][columnCount] = {
    // note: upside down here in editor
    // because 0,0 is bottom left of the grid
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {0, 1, 1, 0, 0, 1, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}};

const int square[rowCount][columnCount] = {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
};

auto matrixRotate(int32_t x, int32_t y, float radians)
{
  float x2 = x * cos(radians) - y * sin(radians);
  float y2 = x * sin(radians) + y * cos(radians);

  return std::vector<float>{x2, y2};
}

void drawPixel(uint32_t row, uint32_t column, uint32_t intensity)
{
  // safety measure: make sure 0 < intensity < 125
  intensity = max(0, intensity);
  intensity = min(125, intensity);

  // ASSUMPTION: 8x8 = 64 leds, aka single panel only
  // ASSUMPTION: 1 frame per second
  // time per pixel = 0.015625 seconds = 15625 microseconds
  // Factors of 15625: 5 x 3125, 25 x 625, 125 x 125
  // ASSUMPTION: 125 fps
  // time per pixel = 125 microseconds. Convenient!

  on = intensity;
  off = 125 - on;

  if (
      on > 0
      // safety measure: draw only existing leds
      && row < rowCount && row >= 0 && column < columnCount && column >= 0)
  {
    digitalWrite(rows[row], HIGH);
    digitalWrite(columns[column], LOW);
    delayMicroseconds(on);

    digitalWrite(rows[row], LOW);
    digitalWrite(columns[column], HIGH);
    delayMicroseconds(off);
  }
  else
  {
    delayMicroseconds(on + off);
  }
}

void loop()
{
  // draw all pixels
  for (uint32_t row = 0; row < rowCount; row++)
  {
    for (uint32_t column = 0; column < columnCount; column++)
    {
      // draw a heart
      //drawPixel(row, column, 64 * heart[row][column]);

      // draw a gradient
      //drawPixel(row, column, row*column +1);

      // draw a square
      //drawPixel(row, column, 64 * square[row][column]);

      // draw a rotating square
      // note the -4 and +4 for moving the square around
      std::vector<float> newCoords = matrixRotate(row - 4, column - 4, 0.05 * tick);

      drawPixel(
          (uint32_t)floor(4 + newCoords[0]),
          (uint32_t)floor(4 + newCoords[1]),
          125 * square[row][column]);
    }
  }
  tick++;
}
