#ifndef RENDER_CONFIG_H
#define RENDER_CONFIG_H

#include "Settings.h"
#include "Scene.h"
#include "ImageSettings.h"
#include "CameraSettings.h"

/// When the render configuration file is parsed it becomes a RenderConfig.
class RenderConfig final {
public:
	RenderConfig(CameraSettings&& cms, Settings&& s, ImageSettings&& is,  Scene&& sc):
		camSettings(std::move(cms)), settings(std::move(s)), imageSettings(std::move(is)), scene(std::move(sc)) {}
	
	RenderConfig(RenderConfig&&) = default;

	RenderConfig(const RenderConfig&) = delete;
	RenderConfig& operator=(const RenderConfig&) = delete;

	const CameraSettings& getCameraSettings() const { return camSettings; }
	const Settings& getSettings() const { return settings; }
	const ImageSettings& getImageSettings() const { return imageSettings; }
	const Scene& getScene() const { return scene; }
private:
	CameraSettings camSettings;
	Settings settings;
	ImageSettings imageSettings;
	Scene scene;
};

#endif // !RENDER_CONFIG_H
