#include "Settings.h"

Settings::Settings(Vec3&& backgroundColor):
	backgroundColor(std::move(backgroundColor))
{
}
