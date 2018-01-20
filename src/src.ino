#include <Arduino.h>
#include <cstdint>

#include "framebuffer.h"
#include "beamchase.h"
#include "pixels.h"

uint32_t tick = 0;

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

void loop()
{
  // draw the framebuffer
  beamChase(framebuffer_16_16, tick);
  // draw all the pixels
  drawFramebuffer(framebuffer_16_16);
  // clear the framebuffer
  clearFramebuffer(framebuffer_16_16);
  tick++;
}
