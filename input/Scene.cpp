#include "Scene.h"

Scene::Scene(Settings&& s, std::vector<Object>&& o):
	settings(std::move(s)), objects(std::move(o))
{
}

const Settings& Scene::getSettings() const noexcept
{
	return settings;
}

ImageSettings::ImageSettings(uint16_t w, uint16_t h) noexcept:
	width(w), height(h)
{
}

uint16_t ImageSettings::getWidth() const noexcept
{
	return width;
}

uint16_t ImageSettings::getHeight() const noexcept
{
	return height;
}

Settings::Settings(Vec3&& backgroundColor, ImageSettings&& is, CameraSettings&& cs) noexcept:
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

CameraSettings::CameraSettings(Mat3&& tm, Vec3&& pos) noexcept:
	tm(std::move(tm)), pos(std::move(pos))
{
}
