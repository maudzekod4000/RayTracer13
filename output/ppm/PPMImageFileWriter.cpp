#include "PPMImageFileWriter.h"

/// Assumes proper output path with permissions. Will abort on write if file cannot be opened.
PPMImageFileWriter::PPMImageFileWriter(const PPMImage& image, std::string_view outputPath):
  PPMImageWriter(image), outputPath(outputPath) {}

void PPMImageFileWriter::write() const
{
  std::ofstream ppmFile;
  ppmFile.open(outputPath.data());
  if (!ppmFile.is_open()) abort(); // TODO: Do proper logging
  PPMImageWriter::write(ppmFile);
}
