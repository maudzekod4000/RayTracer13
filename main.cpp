#include <iostream>

#include "output/ppm/PPMImageMeta.h"
#include "output/ppm/PPMImage.h"
#include "output/ppm/PPMColor.h"
#include "output/ppm/PPMImageFileWriter.h"

int main() {
    const int width = 640;
    const int height = 480;
    PPMImageMeta metadata(width, height, 255);
    PPMImage image(metadata);

    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            image.writePixel(j, i, PPMColor(255, 0, 0));
        }
    }

    PPMImageFileWriter writer(image, "rt.ppm");
    writer.write();
    return 0;
}
