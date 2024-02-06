#include "PPMImageMeta.h"

#include <string>

PPMImageMeta::PPMImageMeta(
	uint16_t imgWidth,
	uint16_t imgHeight,
	uint16_t maxColor
): colorFormat("P3"), imgWidth(imgWidth), imgHeight(imgHeight), maxColor(maxColor) {}

// This method is called only once per image, so it is okay to use ostream's <<
// instead of string's '+' operator, even though it is less efficient.
void PPMImageMeta::writeHeaders(std::ostream& os) const {
	os << colorFormat << '\n'
		<< imgWidth << " " << imgHeight << '\n'
		<< std::to_string(maxColor) << '\n';
}
