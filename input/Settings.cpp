#include "Settings.h"

Settings::Settings(Vec3&& backgroundColor) noexcept:
	backgroundColor(std::move(backgroundColor))
{
}
