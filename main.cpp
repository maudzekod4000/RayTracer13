#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <execution>
#include <ranges>

#include "output/ppm/PPMImageMeta.h"
#include "output/ppm/PPMImage.h"
#include "output/ppm/PPMColor.h"
#include "output/ppm/PPMImageFileWriter.h"
#include "sampling/Camera.h"
#include "input/RenderConfigDecoder.h"
#include "input/RenderConfigDecoderJSON.h"
#include "input/FileReader.h"
#include "input/Scene.h"
#include "input/CameraSettings.h"
#include "input/ImageSettings.h"

constexpr uint16_t MAX_COLOR = 255;

int main() {
  // Read the scene file
  const auto fileContentExp = FileReader::readFile("../scenes/final/scene5.crtscene");

  if (!fileContentExp.has_value()) {
    std::cerr << fileContentExp.error() << std::endl;
    return 1;
  }

  std::unique_ptr<RenderConfigDecoder> decoder(new RenderConfigDecoderJSON);

  auto renderConfExp = decoder->decode((uint8_t*)(fileContentExp.value().data()), fileContentExp.value().size());

  if (!renderConfExp.has_value()) {
    std::cerr << renderConfExp.error() << std::endl;
    return 1;
  }

  const RenderConfig& renderConfig = renderConfExp.value();
  const uint16_t width = renderConfig.imageSettings.getWidth();
  const uint16_t height = renderConfig.imageSettings.getHeight();

  const PPMImageMeta metadata(width, height, MAX_COLOR);
  PPMImage image(metadata);
  Camera camera(renderConfig.camSettings.getPos(), renderConfig.camSettings.getTransform(), width, height, -1.0);

  std::cout << "Begin rendering..." << std::endl;

  constexpr int AA = 4; // Antialiasing multiplier: x2, x4, x8
  constexpr float offset = 1.0f / (AA + 1);
  constexpr int samplesCount = AA * AA;

  auto rowRange = std::views::iota(0, int(width));
  std::for_each(std::execution::par, rowRange.begin(), rowRange.end(), [&](int i) {
    for (uint16_t j = 0; j < height; j++) {
      // Simple anti aliasing
      Vec averagedColor(0);
      for (int waa = 1; waa <= AA; waa++) {
        for (int haa = 1; haa <= AA; haa++) {
          float currentOffset = haa * offset;
          const Ray r = camera.generateRay(i + currentOffset, j + currentOffset);
          averagedColor += renderConfig.scene.trace(r);
        }
      }
      averagedColor /= samplesCount;
      averagedColor = glm::clamp(averagedColor, 0.0f, 1.0f);
      image.writePixel(j, i, PPMColor(static_cast<uint16_t>(averagedColor.r * MAX_COLOR), static_cast<uint16_t>(averagedColor.g * MAX_COLOR), static_cast<uint16_t>(averagedColor.b * MAX_COLOR)));
    }
  });

  std::cout << "Rendering done..." << std::endl;

  std::cout << "Writing image..." << std::endl;
  PPMImageFileWriter writer(image, "rt.ppm");
  writer.write();
  return 0;
}
