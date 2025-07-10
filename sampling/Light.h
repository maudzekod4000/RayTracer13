#pragma once

#include "calc/TypeDefs.h"

struct Light {
	Vec pos;
  Vec albedo = Vec(1.0f); /// White light
	int intensity;
};
