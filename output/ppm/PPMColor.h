#ifndef PPM_COLOR_H
#define PPM_COLOR_H

#include <cstdint>

struct PPMColor {
  inline PPMColor(uint8_t r, uint8_t g, uint8_t b) :r(r), g(g), b(b) {}
  inline PPMColor() : PPMColor(0, 0, 0) {}
  uint8_t r;
  uint8_t g;
  uint8_t b;
};

#endif // !PPM_COLOR_H
