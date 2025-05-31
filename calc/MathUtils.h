#pragma once

#include "calc/TypeDefs.h"

namespace MathUtils {
	inline float area(const Vec3& a, const Vec3& b, const Vec3& c)
	{
		return glm::length(glm::cross(b - a, c - a)) / 2.0f;
	}

	inline Vec3 normal(const Vec3& a, const Vec3& b, const Vec3& c)
	{
		return glm::normalize(glm::cross(b - a, c - a));
	}
}
