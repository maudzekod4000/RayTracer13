#ifndef RENDER_CONFIG_H
#define RENDER_CONFIG_H

#include "Settings.h"
#include "Scene.h"

/// When the render configuration file is parsed it becomes a RenderConfig.
class RenderConfig final {
public:
	RenderConfig(Settings&& s, Scene&& sc) : settings(std::move(s)), scene(std::move(sc)) {}
	
	RenderConfig(RenderConfig&&) = default;

	RenderConfig(const RenderConfig&) = delete;
	RenderConfig& operator=(const RenderConfig&) = delete;

	Settings settings;
	Scene scene;
};

#endif // !RENDER_CONFIG_H
