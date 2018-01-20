#include <cstdint>

uint32_t clear_16_16[16][16];
uint32_t framebuffer_16_16[16][16];
void clearFramebuffer(uint32_t framebuffer_16_16[16][16]);
void drawFramebuffer(uint32_t framebuffer_16_16[16][16]);
void setIntensity(uint32_t framebuffer_16_16[16][16], uint32_t row, uint32_t column, uint32_t intensity);
uint32_t getIntensity(uint32_t framebuffer_16_16[16][16], uint32_t row, uint32_t column);
