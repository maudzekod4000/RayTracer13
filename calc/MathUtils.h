#pragma once

#include "calc/TypeDefs.h"

namespace MathUtils {
	inline float area(const Vec& a, const Vec& b, const Vec& c)
	{
		return glm::length(glm::cross(b - a, c - a)) / 2.0f;
	}

	inline Vec normal(const Vec& a, const Vec& b, const Vec& c)
	{
		return glm::normalize(glm::cross(b - a, c - a));
	}
}
