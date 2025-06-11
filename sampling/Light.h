#pragma once

#include "calc/TypeDefs.h"

struct Light {
	Vec3 pos;
  Vec3 albedo = Vec3(1.0f); /// White light
	int intensity;
};
