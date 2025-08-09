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

int main(int argc, char** argv) {
		if (argc < 2) {
			std::cerr << "Enter scene" << std::endl;
			return -1;
		}
		// Read the scene file
		const auto fileContentExp = FileReader::readFile(argv[1]);

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
						IntersectionData id = renderConfig.scene.intersect(r);
						if (id.intersection) {
								const Vec3 color = id.color;
								image.writePixel(j, i, PPMColor(static_cast<uint16_t>(color.r * 255.0f), static_cast<uint16_t>(color.g * 255.0f), static_cast<uint16_t>(color.b * 255.0f)));
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
