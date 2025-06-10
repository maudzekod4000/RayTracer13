#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "sampling/Triangle.h"
#include "sampling/IntersectionData.h"
#include "sampling/Object.h"
#include "calc/TypeDefs.h"
#include "sampling/Material.h"

/// Represents the loaded scene file.
class Scene {
public:
	inline Scene(std::vector<Triangle>&& t): triangles(std::move(t)) {}

	// TODO: Think about the class design here. Which method we need and which have to be deleted.
	Scene(Scene&&) = default;

	// Scene is too expensive to copy.
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	inline IntersectionData intersect(const Ray& ray) const {
		IntersectionData intersectionData{};

		for (auto& triangle : triangles) {
			if (triangle.intersect(ray, intersectionData)) {
				intersectionData.color = samplePixelColor(triangle);
			}
		}

		return intersectionData;
	}

	/// All the triangles from the loaded scene.
	/// The triangles will be ordered by object ownership, i.e., the first (in the scene config) object's triangles
	/// will be first in the collection, etc.
	std::vector<Triangle> triangles;

	/// Calculate the color of the pixel at the triangle intersection point.
	/// Uses Scene fields to get information about the object and material associated with the triangle.
	/// @param triangle The triangle that is being hit. Could be useful for getting normal vector.
	/// @param triangleIdx The index of the triangle in the Triangle collection.
	/// @return Returns 3-component color (RGB).
	inline Vec3 samplePixelColor(const Triangle& triangle) const {
		return triangle.material.albedo;
	}
};

#endif // !SCENE_H
