#ifndef SCENE_DECODER_JSON_H
#define SCENE_DECODER_JSON_H

#include "RenderConfigDecoder.h"
#include "RenderConfig.h"

class RenderConfigDecoderJSON final : public RenderConfigDecoder {
public:
	std::expected<RenderConfig, std::string> decode(const uint8_t*, size_t len) override;
};

#endif // !SCENE_DECODER_JSON_H
