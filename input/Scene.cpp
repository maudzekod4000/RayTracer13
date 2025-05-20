#include "Scene.h"

Scene::Scene(std::vector<Triangle>&& t): triangles(std::move(t)) {}

IntersectionData Scene::intersect(const Ray& r) const
{
	IntersectionData i{};

	for (const Triangle& t : triangles) {
		t.intersect(r, i);
	}

	return i;
}
