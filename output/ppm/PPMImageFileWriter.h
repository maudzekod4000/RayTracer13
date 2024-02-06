#ifndef PPM_IMAGE_FILE_WRITER
#define PPM_IMAGE_FILE_WRITER

#include <string_view>

#include "PPMImageWriter.h"

class PPMImageFileWriter final : private PPMImageWriter {
public:
  PPMImageFileWriter(const PPMImage& image, std::string_view outputPath);

  void write() const;
private:
  std::string_view outputPath;
};

#endif // !PPM_IMAGE_FILE_WRITER
