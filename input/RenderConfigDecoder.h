#ifndef VRSCENE_DECODER_H
#define VRSCENE_DECODER_H

#include <cstdint>
#include <expected>
#include <string>

#include "RenderConfig.h"

// A decoder is used when a user selects a scene, i.e. rarely - not on a hot path.
// Lets go with abstract classes and inheritance
class RenderConfigDecoder {
public:
	/// @brief Converts raw bytes to a scene
	/// @param b Byte array pointer
	/// @param s Length of the byte array pointer
	/// @return Either a RenderConfig object or an error message
	virtual std::expected<RenderConfig, std::string> decode(const uint8_t* b, size_t s) = 0;
};

#endif // !VRSCENE_DECODER_H
