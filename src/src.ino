#include <Arduino.h>
#include <cstdint>

#include "framebuffer.h"
#include "beamchase.h"
#include "pixels.h"

uint32_t tick = 0;
Framebuffer_16_16 framebuffer_16_16;
Pixels pixels;

void setup()
{
  Serial.println("Hello, world!");

  //TODO: cleanup
  // Currently lots of the codebase assume single 8x8 led panel
  // and framebuffer size of 16x16 for 2x supersampling AA

  // row in terms of led matrix pins, positive
  uint32_t rows[] = {5, 2, 7, 1, 12, 8, 14, 9};
  // column in terms of led matrix pins, ground
  uint32_t columns[] = {13, 3, 4, 10, 6, 11, 15, 16};
  // "post-process" the pins:
  // pins 1-8 of matrix are at pins 1-8 on teensy
  // pins 9-16 of matrix are at pins 14-21 on teensy
  for (uint32_t i = 0; i < 8; i++)
  {
    if (rows[i] >= 9)
    {
      rows[i] += 5;
    }
  }
  for (uint32_t i = 0; i < 8; i++)
  {
    if (columns[i] >= 9)
    {
      columns[i] += 5;
    }
  }

  // Initialize actual pixels
  pixels.initialize(rows, columns);
  Serial.println("Pixels initialized!");

  // clear the framebuffer
  framebuffer_16_16.clear();
  Serial.println("Framebuffer cleared!");
}

void loop()
{
  //if (tick < 1)
  if (true)
  {
    // draw the framebuffer
    beamChase(framebuffer_16_16, tick);
    // draw all the pixels
    pixels.draw(framebuffer_16_16);
    // clear the framebuffer
    framebuffer_16_16.clear();
    tick++;
  }
  else
  {
    delay(1000);
  }
}
