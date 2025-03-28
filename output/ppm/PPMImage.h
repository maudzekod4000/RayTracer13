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

  void writePixel(int32_t row, int32_t col, const PPMColor& color);
protected:
  std::vector<std::vector<PPMColor>> buffer;
  const PPMImageMeta& metadata;
};

#endif // !PPM_IMAGE_H
