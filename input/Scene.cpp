#include "Scene.h"

#include "calc/TypeDefs.h"

#include "sampling/Material.h"

Scene::Scene(std::vector<Triangle>&& t, std::vector<int>&& triangleIdxToObj, std::vector<Object>&& objs):
	triangles(std::move(t)),
	triangleIdxToObject(std::move(triangleIdxToObj)),
	objects(std::move(objs))
{}

IntersectionData Scene::intersect(const Ray& ray) const
{
	IntersectionData intersectionData{};

	for (int i = 0; i < triangles.size(); i++) {
		const Triangle& triangle = triangles[i];
		if (triangle.intersect(ray, intersectionData)) {
			intersectionData.color = samplePixelColor(triangle, i);
		}
	}

	return intersectionData;
}

Vec3 Scene::samplePixelColor(const Triangle& triangle, int triangleIdx) const
{
	const int objIdx = triangleIdxToObject[triangleIdx];
	const Object& obj = objects[objIdx];

	return obj.getMaterial().albedo;
}
