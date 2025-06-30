#include <iostream>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>

#include "output/ppm/PPMImageMeta.h"
#include "output/ppm/PPMImage.h"
#include "output/ppm/PPMColor.h"
#include "output/ppm/PPMImageFileWriter.h"
#include "sampling/Camera.h"
#include "sampling/IntersectionData.h"
#include "input/RenderConfigDecoder.h"
#include "input/RenderConfigDecoderJSON.h"
#include "input/FileReader.h"
#include "input/Scene.h"
#include "input/CameraSettings.h"
#include "input/ImageSettings.h"

constexpr uint16_t MAX_COLOR = 255;

int main() {
  // Read the scene file
  const auto fileContentExp = FileReader::readFile("../scenes/shading-2/scene1.crtscene");

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

  // TODO: It would be cool to have separate projects for outputting images, sampling,
  // optim. algorithms, multithreading etc.

  for (uint16_t i = 0; i < width; i++) {
    for (uint16_t j = 0; j < height; j++) {
      const Ray r = camera.generateRay(i, j);
      Vec3 color = renderConfig.scene.trace(r);
      image.writePixel(j, i, PPMColor(static_cast<uint16_t>(color.r * MAX_COLOR), static_cast<uint16_t>(color.g * MAX_COLOR), static_cast<uint16_t>(color.b * MAX_COLOR)));
    }
  }

  std::cout << "Rendering done..." << std::endl;

  std::cout << "Writing image..." << std::endl;
  PPMImageFileWriter writer(image, "rt.ppm");
  writer.write();
  return 0;
}
