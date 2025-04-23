#include "Scene.h"

Scene::Scene(Settings&& s, std::vector<Object>&& o):
	settings(std::move(s)), objects(std::move(o))
{
}

const Settings& Scene::getSettings() const noexcept
{
	return settings;
}
