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

	uint16_t getWidth() const noexcept;
	uint16_t getHeight() const noexcept;
private:
	uint16_t width;
	uint16_t height;
};

class Settings {
public:
	Settings(Vec3&& backgroundColor, ImageSettings&&, CameraSettings&&) noexcept;

	const ImageSettings& getImgSettings() const noexcept;
	const CameraSettings& getCamSettings() const noexcept;
private:
	// The color of the pixel when the ray does not hit anything.
	Vec3 backgroundColor;
	ImageSettings imageSettings;
	CameraSettings camSettings;
};

/// Represents the loaded scene file.
class Scene {
public:
	Scene(Settings&&, std::vector<Object>&&);

	const Settings& getSettings() const noexcept;

	// TODO: Think about the class design here. Which method we need and which have to be deleted.
	Scene(Scene&&) = default;

	// Scene is too expensive to copy.
	Scene(const Scene&) = delete;
	Scene& operator=(const Scene&) = delete;
private:
	Settings settings;
	std::vector<Object> objects;
};

#endif // !SCENE_H
