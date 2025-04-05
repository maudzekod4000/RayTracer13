#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Triangle.h"

class Scene {
public:
	Scene(std::vector<Triangle>&&);

	Scene(Scene&&) = default;
private:
	std::vector<Triangle> triangles;
};

#endif // !SCENE_H
