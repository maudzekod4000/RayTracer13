#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "sampling/Triangle.h"
#include "sampling/IntersectionData.h"
#include "sampling/Object.h"

struct Ray;

/// Represents the loaded scene file.
class Scene {
public:
	Scene(std::vector<Triangle>&&, std::vector<int>&& triangleIdxToObj, std::vector<Object>&&);

	// TODO: Think about the class design here. Which method we need and which have to be deleted.
	Scene(Scene&&) = default;

	// Scene is too expensive to copy.
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	IntersectionData intersect(const Ray&) const;

	/// All the triangles from the loaded scene.
	/// The triangles will be ordered by object ownership, i.e., the first (in the scene config) object's triangles
	/// will be first in the collection, etc.
	std::vector<Triangle> triangles;

	/// A mapping between the triangle idx in the Triangle collection and
	/// the object index in Object collection.
	/// For example: Get the object index of triangle[10]:
	/// int objIdx = triangleIdxToObject[10];
	/// Get the actual object:
	/// Object o = objects[objIdx];
	/// TODO: This could be more efficient with a range-key map.
	std::vector<int> triangleIdxToObject;

	/// As read from the scene configuration.
	std::vector<Object> objects;

	/// Calculate the color of the pixel at the triangle intersection point.
	/// Uses Scene fields to get information about the object and material associated with the triangle.
	/// @param triangle The triangle that is being hit. Could be useful for getting normal vector.
	/// @param triangleIdx The index of the triangle in the Triangle collection.
	/// @return Returns 3-component color (RGB).
	Vec3 samplePixelColor(const Triangle& triangle, int triangleIdx) const;
};

#endif // !SCENE_H
