#ifndef SETTINGS_H
#define SETTINGS_H

#include "TypeDefs.h"
#include "ImageSettings.h"
#include "CameraSettings.h"

class Settings {
public:
	Settings(Vec3&& backgroundColor, ImageSettings&&, CameraSettings&&) noexcept;

	const ImageSettings& getImgSettings() const noexcept;
	const CameraSettings& getCamSettings() const noexcept;
private:
	// The color of the pixel when the ray does not hit anything.
	Vec3 backgroundColor;
	ImageSettings imageSettings;
	CameraSettings camSettings;
};

#endif // !SETTINGS_H
