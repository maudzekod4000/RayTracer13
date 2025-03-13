#include <iostream>
#include <cstdint>

#include "output/ppm/PPMImageMeta.h"
#include "output/ppm/PPMImage.h"
#include "output/ppm/PPMColor.h"
#include "output/ppm/PPMImageFileWriter.h"
#include "Camera.h"
#include "TypeDefs.h"

#define WIDTH 640
#define HEIGHT 480
#define MAX_COLOR 255

int main() {
    const uint16_t width = WIDTH;
    const uint16_t height = HEIGHT;
    PPMImageMeta metadata(width, height, MAX_COLOR);
    PPMImage image(metadata);
    Camera camera(Vec3(0.0f), glm::identity<Mat3>(), width, height, -1.0);

    const Vec3 forward(0.0, 0.0, -1.0);

    for (uint16_t i = 0; i < width; i++) {
        for (uint16_t j = 0; j < height; j++) {
            const Ray r = camera.generateRay(i, j);
            const float angle = glm::abs(glm::dot(forward, r.dir));
            image.writePixel(j, i, PPMColor(angle * MAX_COLOR, angle * MAX_COLOR, angle * MAX_COLOR));
        }
    }

    PPMImageFileWriter writer(image, "rt.ppm");
    writer.write();
    return 0;
}
