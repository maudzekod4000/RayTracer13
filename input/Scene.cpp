#include "Scene.h"

Scene::Scene(std::vector<Triangle>&& t):
	triangles(std::move(t))
{
}
