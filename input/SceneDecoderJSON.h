#ifndef SCENE_DECODER_JSON_H
#define SCENE_DECODER_JSON_H

#include "VRSceneDecoder.h"

class SceneDecoderJSON final : public SceneDecoder {
public:
	std::expected<Scene, std::string> decode(const uint8_t*, size_t len) override;
};

#endif // !SCENE_DECODER_JSON_H
