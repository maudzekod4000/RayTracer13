#ifndef SCENE_H
#define SCENE_H

#include <vector>

#include "Object.h"
#include "Settings.h"

/// Represents the loaded scene file.
class Scene final {
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
