#include <cstdint>
#include <Arduino.h>
#include "framebuffer.h"

uint32_t on;
uint32_t off;

class Pixels
{
  //TODO: generalize to arbitrary size
  uint32_t rows[8];
  uint32_t columns[8];

public:
  void clear();
  void initialize(uint32_t pinRows[], uint32_t pinColumns[]);
  void draw(Framebuffer_16_16 framebuffer_16_16);
  void drawPixel(uint32_t row, uint32_t column, uint32_t intensity);
};

void Pixels::clear()
{
  // initialize the "positive volt" pins
  for (uint32_t i = 0; i < 8; i++)
  {
    pinMode(rows[i], OUTPUT);
    digitalWrite(rows[i], LOW);
  }
  // initialize the "programmable ground" pins
  for (uint32_t i = 0; i < 8; i++)
  {
    pinMode(columns[i], OUTPUT);
    digitalWrite(columns[i], HIGH);
  }
}

void Pixels::initialize(uint32_t pinRows[8], uint32_t pinColumns[8])
{
  memcpy(rows, pinRows, sizeof(pinRows) * 8);
  memcpy(columns, pinColumns, sizeof(pinColumns) * 8);
  // Serial.print("rows: ");
  // for (int i = 0; i < 8; i++)
  // {
  //   Serial.print(rows[i]);
  //   Serial.print(",");
  // }
  // Serial.print("\n");
  // Serial.print("columns: ");
  // for (int i = 0; i < 8; i++)
  // {
  //   Serial.print(columns[i]);
  //   Serial.print(",");
  // }
  // Serial.print("\n");
  clear();
}

void Pixels::drawPixel(uint32_t row, uint32_t column, uint32_t intensity)
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
      // only draw positive values
      on > 0
      // check bounds
      && row < 8 && row >= 0
      // check bounds
      && column < 8 && column >= 0)
  {
    // Serial.print(rows[row]);
    // Serial.print(",");
    // Serial.print(columns[column]);
    // Serial.println();
    // Serial.print("intensity: ");
    // Serial.println(on);
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

void Pixels::draw(Framebuffer_16_16 framebuffer_16_16)
{
  for (uint32_t row = 0; row < 8; row++)
  {
    for (uint32_t column = 0; column < 8; column++)
    {
      ///Serial.println(framebuffer_16_16.getIntensity(row, column));
      drawPixel(row, column, framebuffer_16_16.getIntensity(row, column));
    }
  }
};