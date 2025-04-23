#include <iostream>
#include <cstdint>
#include <filesystem>
#include <string>
#include <fstream>
#include <vector>
#include <memory>

#include "output/ppm/PPMImageMeta.h"
#include "output/ppm/PPMImage.h"
#include "output/ppm/PPMColor.h"
#include "output/ppm/PPMImageFileWriter.h"
#include "Camera.h"
#include "TypeDefs.h"
#include "Vertex.h"
#include "Triangle.h"
#include "IntersectionData.h"
#include "input/VRSceneDecoder.h"
#include "input/SceneDecoderJSON.h"
#include "input/FileReader.h"
#include "input/Scene.h"
#include "input/CameraSettings.h"
#include "input/ImageSettings.h"
#include "input/Settings.h"

constexpr uint16_t MAX_COLOR = 255;

int main() {
    // Read the scene file
    const std::filesystem::path filePath = "../scenes/basic/scene0.crtscene";
    std::expected<std::vector<char>, std::string> fileContentExp = FileReader::readFile(filePath);

    if (fileContentExp.has_value() == false) {
        std::cerr << fileContentExp.error() << std::endl;
        return 1;
    }

    std::unique_ptr<SceneDecoder> decoder(new SceneDecoderJSON);

    auto sceneExp = decoder->decode((uint8_t*)(fileContentExp.value().data()), fileContentExp.value().size());

    if (sceneExp.has_value() == false) {
        std::cerr << sceneExp.error() << std::endl;
        return 1;
    }

    const Scene& scene = sceneExp.value();
    const ImageSettings& imgSettings = scene.getSettings().getImgSettings();
    const CameraSettings& camSettings = scene.getSettings().getCamSettings();
    const uint16_t width = imgSettings.getWidth();
    const uint16_t height = imgSettings.getHeight();
    
    const PPMImageMeta metadata(width, height, MAX_COLOR);
    PPMImage image(metadata);
    Camera camera(Vec3(0.0f), glm::identity<Mat3>(), width, height, -1.0);

    // create some sample triangle
    Vertex a(Vec3(-0.5f, 0.5f, -2.0f));
    Vertex b(Vec3(-0.0f, -0.5f, -2.0f));
    Vertex c(Vec3(0.5f, 0.5f, -2.0f));

    Triangle tr(std::move(a), std::move(b), std::move(c));

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
