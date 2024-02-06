#include "PPMImage.h"

#include "PPMImageMeta.h"

PPMImage::PPMImage(const PPMImageMeta& metadata): metadata(metadata)
{
  image.resize(metadata.imgHeight); // TODO: Test with big allocations

  for (int32_t i = 0; i < metadata.imgHeight; i++) {
    image[i].resize(metadata.imgWidth);
  }
}

void PPMImage::writePixel(int32_t row, int32_t col, const PPMColor& color)
{
  image[row][col] = color;
}
