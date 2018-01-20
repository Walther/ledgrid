#include <cstdint>
#include <Arduino.h>

// Currently lots of the codebase assume single 8x8 led panel
// and framebuffer size of 16x16 for 2x supersampling AA
const uint32_t rowCount = 8;
const uint32_t columnCount = 8;

// row in terms of led matrix pins, positive
uint32_t rows[] = {5, 2, 7, 1, 12, 8, 14, 9};
// column in terms of led matrix pins, ground
uint32_t columns[] = {13, 3, 4, 10, 6, 11, 15, 16};

uint32_t on;
uint32_t off;

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