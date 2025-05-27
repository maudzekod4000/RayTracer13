#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Triangle.h"
#include "IntersectionData.h"

/// Represents the loaded scene file.
class Scene final {
public:
	Scene(std::vector<Triangle>&&);

	// TODO: Think about the class design here. Which method we need and which have to be deleted.
	Scene(Scene&&) = default;

	// Scene is too expensive to copy.
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;

	IntersectionData intersect(const Ray&) const;
private:
	std::vector<Triangle> triangles;

	std::
};

#endif // !SCENE_H
