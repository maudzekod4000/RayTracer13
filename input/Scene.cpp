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
