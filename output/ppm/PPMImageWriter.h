#ifndef PPM_IMAGE_WRITER_H
#define PPM_IMAGE_WRITER_H

#include <fstream>

class PPMImage;

class PPMImageWriter {
public:
  PPMImageWriter(const PPMImage& image);

	void write(std::ostream& output) const;
private:
  const PPMImage& image;
};

#endif // !PPM_IMAGE_WRITER_H
