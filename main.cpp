#include <iostream>

#include "output/ppm/PPMImageMeta.h"
#include "output/ppm/PPMImage.h"
#include "output/ppm/PPMColor.h"
#include "output/ppm/PPMImageFileWriter.h"
#include "Camera.h"
#include "TypeDefs.h"
#include "Raygen.h"

int main() {
    int width = 1920;
    int height = 1640;
    PPMImageMeta metadata(width, height, 255);
    PPMImage image(metadata);
    Camera camera(Vec3(0.0f), glm::identity<Mat3>());
    Raygen rayGen(width, height, camera, -1.0f);

    Vec3 forward(0.0, 0.0, -1.0);
    camera.pan(20);

    for (size_t i = 0; i < width; i++) {
        for (size_t j = 0; j < height; j++) {
            Ray r = rayGen.gen(i, j);
            float angle = glm::abs(glm::dot(forward, r.dir));
            image.writePixel(j, i, PPMColor(angle * 255, angle * 255, angle * 255));
        }
    }

    PPMImageFileWriter writer(image, "rt.ppm");
    writer.write();
    return 0;
}
