#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <cstdint>
#include <iostream>
#include "Object.h"
#include "TypeDefs.h"

class CameraSettings final {
public:
	explicit CameraSettings(Mat3&& tm, Vec3&& pos) noexcept;
private:
	/// Transformation matrix
	Mat3 tm;
	/// Camera position in space.
	Vec3 pos;
};

class ImageSettings final {
public:
	explicit ImageSettings(uint16_t w, uint16_t h) noexcept;

	ImageSettings(const ImageSettings& is) noexcept {
		std::cout << "copy constructor" << std::endl;
		this->width = is.width;
		this->height = is.height;
	}

	ImageSettings(ImageSettings&& is) noexcept {
		std::cout << "move constructor" << std::endl;
		this->width = is.width;
		this->height = is.height;
		is.width = 0;
		is.height = 0;
	}

	uint16_t getWidth() const noexcept;
private:
	uint16_t width;
	uint16_t height;
};

class Settings {
public:
	Settings(Vec3&& backgroundColor, ImageSettings&&) noexcept;
private:
	// The color of the background when the ray goes to infinity.
	Vec3 backgroundColor;
	ImageSettings imageSettings;
};

class Scene {
public:
	Scene(CameraSettings&&, ImageSettings&&, Settings&&, std::vector<Object>&&);

	// TODO: Think about the class design here. Which method we need and which have to be deleted.
	Scene(Scene&&) = default;
private:
	CameraSettings cameraSettings;
	ImageSettings imageSettings;
	Settings settings;
	std::vector<Object> objects;
};

#endif // !SCENE_H
