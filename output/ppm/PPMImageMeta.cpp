#include "PPMImageMeta.h"

#include <string>

PPMImageMeta::PPMImageMeta(
	uint16_t imgWidth,
	uint16_t imgHeight,
	uint16_t maxColor
): imgWidth(imgWidth), imgHeight(imgHeight), maxColor(maxColor) {}

void PPMImageMeta::writeHeaders(std::ostream& os) const {
	os << "P3" << '\n'
		<< imgWidth << " " << imgHeight << '\n'
		<< std::to_string(maxColor) << '\n';
}
