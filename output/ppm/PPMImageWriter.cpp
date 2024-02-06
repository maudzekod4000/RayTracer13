#include "PPMImageWriter.h"

#include <fstream>
#include <string>
#include <assert.h>

#include "PPMImageMeta.h"
#include "PPMImage.h"

PPMImageWriter::PPMImageWriter(const PPMImage& image): image(image) {}

__declspec(safebuffers)
void PPMImageWriter::write(std::ostream& output) const
{
  image.metadata.writeHeaders(output);

  std::string buffer;
  // Each color takes maximum of '255 255 255' -> 11 characters and there is a space after each color so 12.
  assert(image.image.size() > 0);
  buffer.reserve(12 * image.image.size() * image.image[0].size());
  for (const auto& row : image.image) {
    for (const auto& color : row) {
      buffer += std::to_string(color.r) + " " + std::to_string(color.g) + " " + std::to_string(color.b) + " ";
    }
    buffer += '\n';
  }

  output << buffer;
}
