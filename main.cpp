#include <iostream>
#include <cstdint>

#include "output/ppm/PPMImageMeta.h"
#include "output/ppm/PPMImage.h"
#include "output/ppm/PPMColor.h"
#include "output/ppm/PPMImageFileWriter.h"
#include "Camera.h"
#include "TypeDefs.h"
#include "Vertex.h"
#include "Triangle.h"
#include "IntersectionData.h"

constexpr uint16_t WIDTH = 1920;
constexpr uint16_t HEIGHT = 1080;
constexpr uint16_t MAX_COLOR = 255;

int main() {
    const uint16_t width = WIDTH;
    const uint16_t height = HEIGHT;
    const PPMImageMeta metadata(width, height, MAX_COLOR);
    PPMImage image(metadata);
    Camera camera(Vec3(0.0f), glm::identity<Mat3>(), width, height, -1.0);

    const Vec3 forward(0.0, 0.0, -1.0);

    // create some sample triangle
    const Vertex a{ Vec3(-0.5f, 0.5f, -2.0f), Vec3() };
    const Vertex b{ Vec3(-0.0f, -0.5f, -2.0f), Vec3() };
    const Vertex c{ Vec3(0.5f, 0.5f, -2.0f), Vec3() };

    const Triangle tr(a, b, c);

    std::cout << "Begin rendering..." << std::endl;

    for (uint16_t i = 0; i < width; i++) {
        for (uint16_t j = 0; j < height; j++) {
            const Ray r = camera.generateRay(i, j);
            IntersectionData id{};
            const bool intersects = tr.intersect(r, id);
            if (intersects) {
                image.writePixel(j, i, PPMColor(MAX_COLOR, 0, 0));
            }
            else {
                image.writePixel(j, i, PPMColor(0, MAX_COLOR, 0));
            }
        }
    }

    std::cout << "Rendering done..." << std::endl;

    std::cout << "Writing image..." << std::endl;
    PPMImageFileWriter writer(image, "rt.ppm");
    writer.write();
    return 0;
}
