#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <cstdint>

#include "Triangle.h"
#include "TypeDefs.h"

class ImageSettings final {
public:
	explicit ImageSettings(uint16_t w, uint16_t h) noexcept;

	uint16_t getWidth() const noexcept;
private:
	uint16_t width;
	uint16_t height;
};

class Settings {
public:

private:
	// The color of the background when the ray goes to infinity.
	Vec3 backgroundColor;
};

class Scene {
public:
	Scene(std::vector<Triangle>&&);

	Scene(Scene&&) = default;
private:
	std::vector<Triangle> triangles;
};

#endif // !SCENE_H
