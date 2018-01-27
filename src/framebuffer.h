#include <cstdint>

class Framebuffer_16_16
{
  uint32_t data[16][16];

public:
  void clear();
  uint32_t getIntensity(uint32_t row, uint32_t column);
  void setIntensity(uint32_t row, uint32_t column, uint32_t intensity);
  void draw();
};