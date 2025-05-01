#include "Scene.h"

Scene::Scene(std::vector<Object>&& o): objects(std::move(o)) {}

