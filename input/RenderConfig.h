#pragma once

#include "input/Settings.h"
#include "input/Scene.h"
#include "input/ImageSettings.h"
#include "input/CameraSettings.h"

struct RenderConfig {
	RenderConfig(CameraSettings&& cms, Settings&& s, ImageSettings&& is,  Scene&& sc):
		camSettings(std::move(cms)), settings(std::move(s)), imageSettings(std::move(is)), scene(std::move(sc)) {}

	CameraSettings camSettings;
	Settings settings;
	ImageSettings imageSettings;
	Scene scene;
	
	RenderConfig(RenderConfig&&) = default;

	RenderConfig(const RenderConfig&) = delete;
	RenderConfig& operator=(const RenderConfig&) = delete;
};
