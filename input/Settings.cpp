#include "Settings.h"

Settings::Settings(Vec3&& backgroundColor, ImageSettings&& is, CameraSettings&& cs) noexcept :
	backgroundColor(std::move(backgroundColor)), imageSettings(std::move(is)),
	camSettings(std::move(cs))
{
}

const ImageSettings& Settings::getImgSettings() const noexcept
{
	return imageSettings;
}

const CameraSettings& Settings::getCamSettings() const noexcept
{
	return camSettings;
}
