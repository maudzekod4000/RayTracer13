#ifndef PPM_IMAGE_H
#define PPM_IMAGE_H

#include <vector>
#include <stdint.h>

#include "PPMColor.h"
class PPMImageMeta;

/// <summary>
/// In-memory representation of an image in the PPM format.
/// Can be used for displaying in a frame buffer or post-processing.
/// </summary>
class PPMImage {
public:
  friend class PPMImageWriter;
  PPMImage(const PPMImageMeta& metadata);

  void writePixel(uint16_t row, uint16_t col, const PPMColor& color);
private:
  std::vector<std::vector<PPMColor>> buffer;
  const PPMImageMeta& metadata;
};

#endif // !PPM_IMAGE_H
