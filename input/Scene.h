#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "sampling/Triangle.h"
#include "sampling/IntersectionData.h"
#include "calc/TypeDefs.h"
#include "sampling/Material.h"
#include "sampling/Light.h"

class Scene {
public:
	inline Scene(std::vector<Triangle>&& t, std::vector<Light>&& l):
		triangles(std::move(t)),
		lights(std::move(l))
	{}

	Scene(Scene&&) = default;

	// Scene is too expensive to copy.
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	inline Vec3 trace(const Ray& ray) const {
		IntersectionData intersectionData{};

		for (auto& triangle : triangles) {
			triangle.intersect(ray, intersectionData);
		}



		return intersectionData;
	}

	inline Vec3 calculatePixelColor(const IntersectionData& triangle) const {
		return triangle.material.albedo;
	}

	std::vector<Triangle> triangles;
	std::vector<Light> lights;
};

#endif // !SCENE_H
