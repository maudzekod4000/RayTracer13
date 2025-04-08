#include "Scene.h"

Scene::Scene(std::vector<Triangle>&& t):
	triangles(std::move(t))
{
}

ImageSettings::ImageSettings(uint16_t w, uint16_t h):
	width(w), height(h)
{
}

uint16_t ImageSettings::getWidth() const noexcept
{
	return width;
}

Settings::Settings(Vec3&& backgroundColor, ImageSettings&& is) noexcept:
	backgroundColor(std::move(backgroundColor)), imageSettings(std::move(is))
{
}

CameraSettings::CameraSettings(Mat3&& tm, Vec3&& pos) noexcept:
	tm(std::move(tm)), pos(std::move(pos))
{
}
