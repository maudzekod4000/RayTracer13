#include "PPMImage.h"

#include "PPMImageMeta.h"

PPMImage::PPMImage(const PPMImageMeta& metadata): metadata(metadata)
{
  buffer.resize(metadata.imgHeight); // TODO: Test with big allocations

  for (int16_t i = 0; i < metadata.imgHeight; i++) {
    buffer[i].resize(metadata.imgWidth);
  }
}

void PPMImage::writePixel(uint16_t row, uint16_t col, const PPMColor& color)
{
  buffer[row][col] = color;
}
