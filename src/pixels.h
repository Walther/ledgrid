#include <cstdint>
class Pixels
{
  //TODO: generalize to arbitrary size
  uint32_t rows[8];
  uint32_t columns[8];

public:
  void clear();
  void initialize(uint32_t pinRows[8], uint32_t pinColumns[8]);
  void draw(Framebuffer_16_16 framebuffer_16_16);
  void drawPixel(uint32_t row, uint32_t column, uint32_t intensity);
};
