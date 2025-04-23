#include "ImageSettings.h"

ImageSettings::ImageSettings(uint16_t w, uint16_t h) noexcept :
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
